#include "main_window.h"

#include <qapplication.h>
#include <type_event_system.h>
#include <windows.h>

#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QtConcurrent>
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

  slot_ = EventBus::GetInstance().Subscribe<std::shared_ptr<HelloEvent>>(
      "10086", [this](std::shared_ptr<HelloEvent> helloEvent) {
        button_test_->setText("hello!" + QString::number(helloEvent->id));
        qDebug() << "HelloEvent id: " << QString::number(helloEvent->id);
        qDebug() << QThread::currentThread();
      });

  QTimer::singleShot(1000, this, []() {
    QtConcurrent::run([]() {
      qDebug() << "Publish HelloEvent";
      qDebug() << QThread::currentThread();
      auto helloEvent = std::make_shared<HelloEvent>();
      helloEvent->id = 666;
      helloEvent->sex = false;
      EventBus::GetInstance().Publish("10086", helloEvent);
    });
  });

  slot_2_ = EventBus::GetInstance()
                .Subscribe<HelloEvent, std::shared_ptr<HelloEvent>>(
                    [this](std::shared_ptr<HelloEvent> helloEvent) {
                      button_test_->setText("hello!" +
                                            QString::number(helloEvent->id));
                      qDebug() << "HelloEvent id: "
                               << QString::number(helloEvent->id);
                      qDebug() << QThread::currentThread();
                      slot_2_.reset();
                    });

  QTimer::singleShot(2000, this, []() {
    QtConcurrent::run([]() {
      qDebug() << "Publish HelloEvent";
      qDebug() << QThread::currentThread();
      auto helloEvent = std::make_shared<HelloEvent>();
      helloEvent->id = 888;
      helloEvent->sex = false;
      EventBus::GetInstance().Publish<HelloEvent>(helloEvent);
    });
  });

    QTimer::singleShot(3000, this, []() {
    QtConcurrent::run([]() {
      qDebug() << "Publish HelloEvent";
      qDebug() << QThread::currentThread();
      auto helloEvent = std::make_shared<HelloEvent>();
      helloEvent->id = 999;
      helloEvent->sex = false;
      EventBus::GetInstance().Publish<HelloEvent>(helloEvent);
    });
  });

  //{
  //  TypeEventSystem::Get().Subscribe<HelloEvent, std::shared_ptr<HelloEvent>>(
  //      [this](const std::shared_ptr<HelloEvent> &hello_event) {
  //        qDebug() << "HelloEvent id: " << QString::number(hello_event->id);
  //        qDebug() << QThread::currentThread();
  //        button_test_->setText("hello!" + QString::number(hello_event->id));
  //      });
  //}

  // QTimer::singleShot(1000, this, []() {
  //   QtConcurrent::run([]() {
  //     qDebug() << "Publish HelloEvent";
  //     qDebug() << QThread::currentThread();
  //     auto helloEvent = std::make_shared<HelloEvent>();
  //     helloEvent->id = 666;
  //     helloEvent->sex = false;
  //     TypeEventSystem::Get().Publish<HelloEvent>(helloEvent);
  //   });
  // });

  // QTimer::singleShot(1000, this, []() {
  //   QtConcurrent::run([]() {
  //     qDebug() << "Publish HelloEvent";
  //     qDebug() << QThread::currentThread();
  //     auto helloEvent = std::make_shared<HelloEvent>();
  //     helloEvent->id = 2333;
  //     helloEvent->sex = false;
  //     TypeEventSystem::Get().Publish<HelloEvent>(helloEvent);
  //   });
  // });
}
