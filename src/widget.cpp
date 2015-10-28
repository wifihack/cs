#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <GJsonAux>
#include <GPropWidget>

Widget::Widget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Widget)
{
  ui->setupUi(this);
  loadControl();
  setControl();
}

Widget::~Widget()
{
  saveControl();
  delete ui;
}

void Widget::loadControl() {
  QJsonObject json = GJson::instance().loadFromFile();

  json["widget"] >> *this;

  QList<int> sizes;
  json["splitter"] >> sizes;
  if (sizes.count() != 0)
    ui->splitter->setSizes(sizes);

  json["cs"] >> cs_;
}

void Widget::saveControl() {
  QJsonObject json;

  json["widget"] << *this;

  QList<int> sizes = ui->splitter->sizes();
  json["splitter"] << sizes;

  json["cs"] << cs_;
  GJson::instance().saveToFile(json);
}

void Widget::setControl() {
  bool active = cs_.active();
  ui->pbOpen->setEnabled(!active);
  ui->pbClose->setEnabled(active);
  ui->pbOption->setEnabled(!active);
}

void Widget::on_pbOpen_clicked()
{
  qDebug() << "on_pbOpen_clicked()"; // gilgil temp 2015.10.28
  if (!cs_.open()) {
    QMessageBox::warning(this, "Error", cs_.err->msg());
  }
  setControl();
}

void Widget::on_pbClose_clicked()
{
  cs_.close();
  setControl();
  qDebug() << "on_pbClose_clicked()"; // gilgil temp 2015.10.28
}

void Widget::on_pbOption_clicked()
{
  static GPropWidget* widget{nullptr};
  if (widget == nullptr)
    widget = new GPropWidget;
  widget->setObject(&cs_);
  widget->setWindowModality(Qt::ApplicationModal);
  widget->show();
  qDebug() << "on_pbOption_clicked()"; // gilgil temp 2015.10.28
}

void Widget::on_pbAbout_clicked()
{
  AboutDlg dlg{this};
  dlg.show();
  dlg.exec();
}
