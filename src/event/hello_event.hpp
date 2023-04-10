#ifndef EVENT_HELLO_EVENT_H_
#define EVENT_HELLO_EVENT_H_

#include <QDebug>

struct HelloEvent {
  int id;
  bool sex;

  HelloEvent() { qDebug() << "HelloEvent created!"; }
  ~HelloEvent() { qDebug() << "HelloEvent deleted!"; }
};
#endif  // EVENT_HELLO_EVENT_H_