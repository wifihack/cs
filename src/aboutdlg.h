#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>

namespace Ui {
  class AboutDlg;
}

class AboutDlg : public QDialog
{
  Q_OBJECT

public:
  explicit AboutDlg(QWidget *parent = 0);
  ~AboutDlg();

private slots:
  void on_lblLink1_linkActivated(const QString &link);

  void on_lblLink2_linkActivated(const QString &link);

private:
  Ui::AboutDlg *ui;
};

#endif // ABOUTDLG_H
