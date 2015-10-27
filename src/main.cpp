#include "widget.h"
#include <QApplication>
#include <QDebug>
#include <GBase>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  qInfo() << GVERSION;
  Widget w;
  w.show();

  return a.exec();
}
