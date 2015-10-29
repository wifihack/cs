#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <GJsonAux>
#include <GPropWidget>

// ----------------------------------------------------------------------------
// CookieTreeWidgetItem
// ----------------------------------------------------------------------------
struct CookieTreeWidgetItem : QTreeWidgetItem {
  CookieTreeWidgetItem(QTreeWidget* widget, Cookies cookies)
    : QTreeWidgetItem(widget), cookies_(cookies) {}
  Cookies cookies_;
};

// ----------------------------------------------------------------------------
// Widget
// ----------------------------------------------------------------------------
Widget::Widget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Widget)
{
  ui->setupUi(this);
  QObject::connect(&cs_, SIGNAL(captured(Cookies)), this, SLOT(processCookies(Cookies)), Qt::BlockingQueuedConnection);
  loadControl();
  setControl();
}

Widget::~Widget()
{
  ui->pbClose->click();
  saveControl();
  delete ui;
}

void Widget::loadControl() {
  QJsonObject json = GJson::instance().loadFromFile();

  json["widget"] >> this;
  json["splitter"] >> ui->splitter;
  json["cs"] >> cs_;
}

void Widget::saveControl() {
  QJsonObject json;

  json["widget"] << this;
  json["splitter"] << ui->splitter;
  json["cs"] << cs_;

  GJson::instance().saveToFile(json);
}

void Widget::setControl() {
  bool active = cs_.active();
  ui->pbOpen->setEnabled(!active);
  ui->pbClose->setEnabled(active);
  ui->pbOption->setEnabled(!active);
}

void Widget::processCookies(Cookies cookies) {
  if (isDuplicate(cookies))
    return;
  cookiesMgr_.push_back(cookies);
  CookieTreeWidgetItem* item = new CookieTreeWidgetItem(ui->twCookie, cookies);
  item->setFlags(item->flags() | Qt::ItemIsEditable);
  item->setText(0, cookies.ip);
  item->setText(1, cookies.host);
}

bool Widget::isDuplicate(Cookies cookies) {
  foreach (Cookies _cookies, cookiesMgr_) {
    if (_cookies.ip != cookies.ip)
      continue;
    if (_cookies.host != cookies.host)
      continue;

    int count = _cookies.count();
    if (count != cookies.count())
      continue;

    bool isEqualCookie = true;
    for (int i = 0; i < count; i++) {
      Cookie _cookie = _cookies.at(i);
      Cookie cookie = cookies.at(i);
      if (_cookie.name != cookie.name) {
        isEqualCookie = false;
        break;
      }
      if (_cookie.value != cookie.value) {
        isEqualCookie = false;
        break;
      }
    }
    if (!isEqualCookie)
      continue;

    return true;
  }
  return false;
}


void Widget::on_pbOpen_clicked()
{
  qDebug() << "on_pbOpen_clicked()"; // gilgil temp 2015.10.28
  if (!cs_.open()) {
    Q_ASSERT(cs_.err != nullptr);
    QMessageBox::warning(this, "Error", cs_.err->msg());
  }
  setControl();
}

void Widget::on_pbClose_clicked()
{
  cs_.close();
  setControl();
}

void Widget::on_pbOption_clicked()
{
  static GPropWidget* widget{nullptr};
  if (widget == nullptr)
    widget = new GPropWidget;
  widget->setObject(&cs_);
  widget->setWindowModality(Qt::ApplicationModal);
  widget->show();
}

void Widget::on_pbClear_clicked()
{
  ui->twCookie->clear();
  ui->twDetail->clear();
  cookiesMgr_.clear();
}

void Widget::on_pbAbout_clicked()
{
  AboutDlg dlg{this};
  dlg.show();
  dlg.exec();
}

void Widget::on_twCookie_itemSelectionChanged()
{
  QTreeWidgetItem* item = ui->twCookie->currentItem();
  if (item == nullptr) return;
  CookieTreeWidgetItem* cookieItem = dynamic_cast<CookieTreeWidgetItem*>(item);
  Q_ASSERT(cookieItem != nullptr);
  ui->twDetail->clear();

  Cookies cookies = cookieItem->cookies_;
  foreach (Cookie cookie, cookies) {
    QTreeWidgetItem* newItem = new QTreeWidgetItem(ui->twDetail);
    newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
    newItem->setText(0, cookie.name);
    newItem->setText(1, cookie.value);
  }
}
