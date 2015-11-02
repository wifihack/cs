#ifndef WIDGET_H
#define WIDGET_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>
#include "aboutdlg.h"
#include "cookiesniffer.h"

// ----------------------------------------------------------------------------
// Widget
// ----------------------------------------------------------------------------
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

public slots:
  void processCookies(Cookies cookies);
  void processClosed();

public:
  CookieSniffer cs_;
  QString sqliteFileName_{"cookies.sqlite"};

protected:
  QList<Cookies> cookiesMgr_;
  bool isDuplicate(Cookies cookie);

  QSqlDatabase db_;

private slots:
  void on_pbOpen_clicked();

  void on_pbClose_clicked();

  void on_pbOption_clicked();

  void on_pbClear_clicked();

  void on_pbAbout_clicked();

  void on_twCookie_itemSelectionChanged();

  void on_twCookie_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
  Ui::Widget *ui;
};

#endif // WIDGET_H
