#include "main_window.h"

#include <event_bus.h>

#include <QDebug>
#include <memory>

#include "src/dao/student_dao.h"
#include "src/event/hello_event.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  InitView();
  InitController();
}

MainWindow::~MainWindow() {}

void MainWindow::InitView() {
  button_test_ = new QPushButton(this);
  button_test_->setFixedSize(60, 60);
}

void MainWindow::InitController() {
  dao::StudentDao dao2;
  QString numberStr = QString::number(dao2.GetId());
  button_test_->setText("hello!" + numberStr);


   EventBus::Get().Subscribe<std::shared_ptr<HelloEvent>>(
      typeid(HelloEvent).hash_code(),
      [=](std::shared_ptr<HelloEvent> hello_event_ptr) {
        qDebug() << "HelloEvent id: " << QString::number(hello_event_ptr->id);
     });

  auto hello_event_ptr = std::make_shared<HelloEvent>();
  hello_event_ptr->id = 22;
  hello_event_ptr->sex = false;
  qDebug() << "Publish HelloEvent";
  EventBus::Get().Publish(typeid(HelloEvent).hash_code(), hello_event_ptr);

  hello_event_ptr = std::make_shared<HelloEvent>();
  hello_event_ptr->id = 10086;
  hello_event_ptr->sex = true;
  qDebug() << "Publish HelloEvent 2";
  EventBus::Get().Publish(typeid(HelloEvent).hash_code(), hello_event_ptr);
}
