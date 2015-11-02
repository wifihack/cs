#include "widget.h"
#include "ui_widget.h"

#include <QFile>
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
  db_ = QSqlDatabase::addDatabase("QSQLITE");
  QObject::connect(&cs_, SIGNAL(captured(Cookies)), this, SLOT(processCookies(Cookies)), Qt::BlockingQueuedConnection);
  QObject::connect(&cs_, SIGNAL(closed()), this, SLOT(processClosed()), Qt::AutoConnection);
  loadControl();
  setControl();
}

Widget::~Widget()
{
  ui->pbClose->click();
  saveControl();
  db_.close();
  delete ui;
}

void Widget::loadControl() {
  QJsonObject json = GJson::instance().loadFromFile();

  json["widget"] >> this;
  json["splitter"] >> ui->splitter;
  json["cs"] >> cs_;
  QString fileName = json["sqliteFileName"].toString();
  if (fileName != "") sqliteFileName_ = fileName;
}

void Widget::saveControl() {
  QJsonObject json;

  json["widget"] << this;
  json["splitter"] << ui->splitter;
  json["cs"] << cs_;
  json["sqliteFileName"] = sqliteFileName_;

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

void Widget::processClosed() {
  qDebug() << ""; // gilgil temp
  ui->pbClose->click();
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
  if (!cs_.active()) {
    if (sqliteFileName_ == "") {
      QMessageBox::warning(this, "Error", "sqlite file name not pecified");
      return;
    }

    if (!QFile::exists(sqliteFileName_)) {
      QMessageBox::warning(this, "Error", QString("file\" %1\" not exists").arg(sqliteFileName_));
      return;
    }

    db_.setDatabaseName(sqliteFileName_);
    if (!db_.open()) {
      QMessageBox::warning(this, "Error", db_.lastError().text());
      return;
    }
  }

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

void Widget::on_twCookie_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
  (void)column;

  if (item == nullptr) return;
  CookieTreeWidgetItem* cookieItem = dynamic_cast<CookieTreeWidgetItem*>(item);
  Q_ASSERT(cookieItem != nullptr);

  Cookies cookies = cookieItem->cookies_;

  QStringList baseDomains = cookies.host.split('.');
  QString baseDomain;
  while (true) {
    if (baseDomains.count() == 2) {
      baseDomain = baseDomains.join('.');
      break;
    }
    baseDomains.removeFirst();
    if (baseDomains.count() == 0) {
      QMessageBox::warning(this, "Error", "Can not retrieve baseDomain for " + cookies.host);
      return;
    }
  }

  foreach (Cookie cookie, cookies) {
    QSqlQuery query(db_);
    query.exec("select max(id) from moz_cookies;");
    if (!query.exec()) {
      QMessageBox::warning(this, "Error", query.lastError().text());
      return;
    }
    query.next();
    int id = query.value(0).toInt() + 1;

    QString sql = QString(
      "INSERT INTO moz_cookies (id,  baseDomain, appId, inBrowserElement, name, value, host, path, expiry, lastAccessed, creationTime, isSecure, isHttpOnly)"\
      " VALUES                 ( %1, '%2',       0,     0,                '%3', '%4',  '%5', '/', 1446476400, 1446460077220140, 1446460077220140, 0, 0);")
      .arg(QString::number(id), baseDomain, cookie.name, cookie.value, "." + baseDomain);
    qDebug() << sql;
    if (!query.exec(sql)) {
      QMessageBox::warning(this, "Error", query.lastError().text());
      break;
    }
  }
}
