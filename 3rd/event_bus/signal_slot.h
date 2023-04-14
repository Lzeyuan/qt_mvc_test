#ifndef MUDUO_BASE_SIGNALSLOT_H
#define MUDUO_BASE_SIGNALSLOT_H

#include <functional>
#include <memory>
#include <vector>
#include <mutex>
#include <assert.h>

#include <QDebug>

namespace muduo {

namespace detail {

template <typename Callback>
struct SlotImpl;

template <typename Callback>
struct SignalImpl  {
  typedef std::vector<std::weak_ptr<SlotImpl<Callback> > > SlotList;

  SignalImpl() : slots_(new SlotList) {}

  void copyOnWrite() {
    if (!slots_.use_count() == 1) {
      slots_.reset(new SlotList(*slots_));
    }
    assert(slots_.use_count() == 1);
  }

  void clean() {
    std::lock_guard lock(mutex_);
    copyOnWrite();
    SlotList& list(*slots_);
    typename SlotList::iterator it(list.begin());
    while (it != list.end()) {
      if (it->expired()) {
        it = list.erase(it);
      } else {
        ++it;
      }
    }
  }

  std::mutex mutex_;
  std::shared_ptr<SlotList> slots_;
};

template <typename Callback>
struct SlotImpl {
  typedef SignalImpl<Callback> Data;
  SlotImpl(const std::shared_ptr<Data>& data, Callback&& cb)
      : data_(data), cb_(cb), tie_(), tied_(false) {}

  SlotImpl(const std::shared_ptr<Data>& data, Callback&& cb,
           const std::shared_ptr<void>& tie)
      : data_(data), cb_(cb), tie_(tie), tied_(true) {}

  ~SlotImpl() {
    std::shared_ptr<Data> data(data_.lock());
    if (data) {
      data->clean();
    }
  }

  std::weak_ptr<Data> data_;
  Callback cb_;
  std::weak_ptr<void> tie_;
  bool tied_;
};

}  // namespace detail

/// This is the handle for a slot
///
/// The slot will remain connected to the signal fot the life time of the
/// returned Slot object (and its copies).
typedef std::shared_ptr<void> Slot;

template <typename Signature>
class Signal;

template <typename RET, typename... ARGS>
class Signal<RET(ARGS...)> {
 public:
  typedef std::function<void(ARGS...)> Callback;
  typedef detail::SignalImpl<Callback> SignalImpl;
  typedef detail::SlotImpl<Callback> SlotImpl;

  Signal() : impl_(new SignalImpl) {}

  ~Signal() {}

  Slot connect(Callback&& func) {
    std::shared_ptr<SlotImpl> slotImpl(
        new SlotImpl(impl_, std::forward<Callback>(func)));
    add(slotImpl);
    return slotImpl;
  }

  Slot connect(Callback&& func, const std::shared_ptr<void>& tie) {
    std::shared_ptr<SlotImpl> slotImpl(new SlotImpl(impl_, func, tie));
    add(slotImpl);
    return slotImpl;
  }

  void call(ARGS&&... args) {
    SignalImpl& impl(*impl_);
    std::shared_ptr<SignalImpl::SlotList> slots_temp;
    {
      std::lock_guard lock(impl.mutex_);
      slots_temp = impl.slots_;
    }
    typename SignalImpl::SlotList& s(*slots_temp);
    for (typename SignalImpl::SlotList::const_iterator it = s.begin();
         it != s.end(); ++it) {
      std::shared_ptr<SlotImpl> slotImpl = it->lock();
      if (slotImpl) {
        std::shared_ptr<void> guard;
        if (slotImpl->tied_) {
          guard = slotImpl->tie_.lock();
          if (guard) {
            slotImpl->cb_(args...);
          }
        } else {
          slotImpl->cb_(args...);
        }
      }
    }
  }

 private:
  void add(const std::shared_ptr<SlotImpl>& slot) {
    SignalImpl& impl(*impl_);
    {
      std::lock_guard lock(impl.mutex_);
      impl.copyOnWrite();
      impl.slots_->push_back(slot);
    }
  }

  const std::shared_ptr<SignalImpl> impl_;
};

}  // namespace muduo

#endif  // MUDUO_BASE_SIGNALSLOT_H