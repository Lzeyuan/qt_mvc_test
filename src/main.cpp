#include <QApplication>

#include "src/view/main_window.h"
#include "src/base/ioc_container.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
