#include "aboutdlg.h"
#include "ui_aboutdlg.h"

AboutDlg::AboutDlg(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AboutDlg)
{
  ui->setupUi(this);
}

AboutDlg::~AboutDlg()
{
  delete ui;
}

void AboutDlg::on_lblLink1_linkActivated(const QString &link)
{
  ui->lblLink1->setOpenExternalLinks(true);
}

void AboutDlg::on_lblLink2_linkActivated(const QString &link)
{
  ui->lblLink2->setOpenExternalLinks(true);
}
