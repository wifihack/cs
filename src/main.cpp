#include "widget.h"
#include <QApplication>
#include <QDebug>
#include <GBase>
#include <GApp>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  GApp::initialize();
  qInfo() << G_VERSION;

  Widget w;
  w.show();

  return a.exec();
}
