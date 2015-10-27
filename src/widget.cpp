#include "widget.h"
#include "ui_widget.h"

#include <GJsonAux>

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
}

void Widget::saveControl() {
  QJsonObject json;

  json["widget"] << *this;

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
  qDebug() << "on_pbOpen_clicked()";
}

void Widget::on_pbClose_clicked()
{
  qDebug() << "on_pbClose_clicked()";
}

void Widget::on_pbOption_clicked()
{
  qDebug() << "on_pbOption_clicked()";
}
