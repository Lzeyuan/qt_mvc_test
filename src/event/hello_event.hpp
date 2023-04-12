#ifndef EVENT_HELLO_EVENT_H_
#define EVENT_HELLO_EVENT_H_

#include <QDebug>

struct HelloEvent {
  int id = 0;
  bool sex = false;

  HelloEvent() { qDebug() << "HelloEvent created!"; }
  HelloEvent(const HelloEvent& t) { 
    id = t.id;
    sex = t.sex;
    qDebug() << "HelloEvent copy created!"; 
  }
  HelloEvent(const HelloEvent&& t) { 
    id = t.id;
    sex = t.sex;
    qDebug() << "HelloEvent move created!"; 
  }

    HelloEvent& operator=(const HelloEvent& t) {
    id = t.id;
    sex = t.sex;
    qDebug() << "HelloEvent = created!";
  }

  HelloEvent& operator=(const HelloEvent&& t) {
    id = t.id;
    sex = t.sex;
    qDebug() << "HelloEvent = move created!"; 
  }

  ~HelloEvent() { qDebug() << "HelloEvent deleted!"; }
};
#endif  // EVENT_HELLO_EVENT_H_