#ifndef VIEW_MAIN_WINDOW_H_
#define VIEW_MAIN_WINDOW_H_

#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <event_bus.h>


class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  void InitView();
  void InitController();

 private:
  QPushButton* button_test_;
  muduo::Slot slot_;
  muduo::Slot slot_2_;
};
#endif  // MAINWINDOW_H
