#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "cookiesniffer.h"

namespace Ui {
  class Widget;
}

class Widget : public QWidget
{
  Q_OBJECT

public:
  explicit Widget(QWidget *parent = 0);
  ~Widget();

public:
  void loadControl();
  void saveControl();
  void setControl();

public:
  CookieSniffer cs_;

private:
  Ui::Widget *ui;
};

#endif // WIDGET_H
