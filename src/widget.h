#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "aboutdlg.h"
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

private slots:
  void on_pbOpen_clicked();

  void on_pbClose_clicked();

  void on_pbOption_clicked();

  void on_pbAbout_clicked();

private:
  Ui::Widget *ui;
};

#endif // WIDGET_H
