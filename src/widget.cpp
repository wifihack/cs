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
  connect(&cs_, SIGNAL(captured(Cookies)), this, SLOT(processCookies(Cookies)), Qt::BlockingQueuedConnection);
  connect(&cs_, SIGNAL(closed()), this, SLOT(processClosed()), Qt::AutoConnection);
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
  if (fileName == "") {
    fileName = findFirefoxSqliteFile();
  }
  if (!QFile::exists(fileName)) {
    QMessageBox::warning(this, "Error", QString("File %1 not exists").arg(fileName));
  }
  sqliteFileName_ = fileName;

  QString s = json["twoLevelDomainPrefix"].toString();
  if (s == "") {
    twoLevelDomainPrefix_ << ".com.au";
    twoLevelDomainPrefix_ << ".net.au";
    twoLevelDomainPrefix_ << ".co.nz";
    twoLevelDomainPrefix_ << ".net.nz";
    twoLevelDomainPrefix_ << ".com.sg";
    twoLevelDomainPrefix_ << ".com.hk";
    twoLevelDomainPrefix_ << ".com.cn";
    twoLevelDomainPrefix_ << ".cn.com";
    twoLevelDomainPrefix_ << ".co.jp";
    twoLevelDomainPrefix_ << ".co.kr";
    twoLevelDomainPrefix_ << ".co.in";
    twoLevelDomainPrefix_ << ".com.my";
    twoLevelDomainPrefix_ << ".com.tw";
    twoLevelDomainPrefix_ << ".co.id";
    twoLevelDomainPrefix_ << ".web.id";
    twoLevelDomainPrefix_ << ".com.fj";
    twoLevelDomainPrefix_ << ".com.nf";
    twoLevelDomainPrefix_ << ".com.lk";
    twoLevelDomainPrefix_ << ".co.uk";
    twoLevelDomainPrefix_ << ".uk.com";
    twoLevelDomainPrefix_ << ".com.de";
    twoLevelDomainPrefix_ << ".com.es";
    twoLevelDomainPrefix_ << ".co.nl";
    twoLevelDomainPrefix_ << ".co.at";
    twoLevelDomainPrefix_ << ".com.pl";
    twoLevelDomainPrefix_ << ".com.ua";
    twoLevelDomainPrefix_ << ".com.tr";
    twoLevelDomainPrefix_ << ".co.il";
    twoLevelDomainPrefix_ << ".co.ee";
    twoLevelDomainPrefix_ << ".com.mt";
    twoLevelDomainPrefix_ << ".com.hr";
    twoLevelDomainPrefix_ << ".com.br";
    twoLevelDomainPrefix_ << ".br.com";
    twoLevelDomainPrefix_ << ".us.com";
    twoLevelDomainPrefix_ << ".com.mx";
    twoLevelDomainPrefix_ << ".com.pe";
    twoLevelDomainPrefix_ << ".com.do";
    twoLevelDomainPrefix_ << ".com.pa";
    twoLevelDomainPrefix_ << ".com.ai";
    twoLevelDomainPrefix_ << ".com.ec";
    twoLevelDomainPrefix_ << ".com.ve";
    twoLevelDomainPrefix_ << ".co.ve";
    twoLevelDomainPrefix_ << ".com.ag";
    twoLevelDomainPrefix_ << ".com.gt";
    twoLevelDomainPrefix_ << ".co.gy";
    twoLevelDomainPrefix_ << ".com.gy";
    twoLevelDomainPrefix_ << ".com.cu";
    twoLevelDomainPrefix_ << ".com.pr";
    twoLevelDomainPrefix_ << ".com.ar";
    twoLevelDomainPrefix_ << ".co.cr";
    twoLevelDomainPrefix_ << ".com.jm";
    twoLevelDomainPrefix_ << ".com.sv";
    twoLevelDomainPrefix_ << ".com.bs";
    twoLevelDomainPrefix_ << ".com.co";
    twoLevelDomainPrefix_ << ".com.uy";
    twoLevelDomainPrefix_ << ".com.hn";
    twoLevelDomainPrefix_ << ".com.py";
    twoLevelDomainPrefix_ << ".com.ni";
    twoLevelDomainPrefix_ << ".com.vi";
    twoLevelDomainPrefix_ << ".com.bo";
    twoLevelDomainPrefix_ << ".co.za";
    twoLevelDomainPrefix_ << ".ac.ke";
    twoLevelDomainPrefix_ << ".co.ke";
    twoLevelDomainPrefix_ << ".co.ug";
    twoLevelDomainPrefix_ << ".co.na";
    twoLevelDomainPrefix_ << ".com.cm";
    twoLevelDomainPrefix_ << ".com.na";
    twoLevelDomainPrefix_ << ".co.com";
    twoLevelDomainPrefix_ << ".go.kr";
    twoLevelDomainPrefix_ << ".mil.kr";
    twoLevelDomainPrefix_ << ".ac.kr";
    twoLevelDomainPrefix_ << ".hs.kr";
    twoLevelDomainPrefix_ << ".ms.kr";
    twoLevelDomainPrefix_ << ".es.kr";
    twoLevelDomainPrefix_ << ".kg.kr";
    twoLevelDomainPrefix_ << ".sc.kr";
    twoLevelDomainPrefix_ << ".seoul.kr";
    twoLevelDomainPrefix_ << ".busan.kr";
    twoLevelDomainPrefix_ << ".daegu.kr";
    twoLevelDomainPrefix_ << ".incheon.kr";
    twoLevelDomainPrefix_ << ".gwangju.kr";
    twoLevelDomainPrefix_ << ".daejeon.kr";
    twoLevelDomainPrefix_ << ".ulsan.kr";
    twoLevelDomainPrefix_ << ".gyeonggi.kr";
    twoLevelDomainPrefix_ << ".gangwon.kr";
    twoLevelDomainPrefix_ << ".chungbuk.kr";
    twoLevelDomainPrefix_ << ".chungnam.kr";
    twoLevelDomainPrefix_ << ".jeonbuk.kr";
    twoLevelDomainPrefix_ << ".jeonnam.kr";
    twoLevelDomainPrefix_ << ".gyeongbuk.kr";
    twoLevelDomainPrefix_ << ".gyeongnam.kr";
    twoLevelDomainPrefix_ << ".jeju.kr";
  } else {
    twoLevelDomainPrefix_ = s.split(',');
  }
}

void Widget::saveControl() {
  QJsonObject json;

  json["widget"] << this;
  json["splitter"] << ui->splitter;
  json["cs"] << cs_;
  json["sqliteFileName"] = sqliteFileName_;
  json["twoLevelDomainPrefix"] = twoLevelDomainPrefix_.join(',');

  GJson::instance().saveToFile(json);
}

void Widget::setControl() {
  bool active = cs_.active();
  ui->pbOpen->setEnabled(!active);
  ui->pbClose->setEnabled(active);
  ui->pbOption->setEnabled(!active);
  ui->pbGo->setEnabled(ui->twCookie->selectedItems().count() > 0);
}

void Widget::processCookies(Cookies cookies) {
  if (isDuplicate(cookies))
    return;
  cookiesMgr_.push_back(cookies);
  CookieTreeWidgetItem* item = new CookieTreeWidgetItem(ui->twCookie, cookies);
  item->setFlags(item->flags() | Qt::ItemIsEditable);
  item->setText(0, cookies.time.toString("hh:mm:ss"));
  item->setText(1, cookies.ip);
  item->setText(2, cookies.host);
}

void Widget::processClosed() {
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

#include <QDirIterator>
QString Widget::findFirefoxSqliteFile() {
  QDir path = QDir::homePath() + "/.mozilla";

  QDirIterator it(path, QDirIterator::Subdirectories);
  while (it.hasNext()) {
      if (it.fileName() == "cookies.sqlite")
        return it.filePath();
      it.next();
  }
  QMessageBox::critical(this, "Error", "Can not find cookies.sqlite file");
  return "";
}

QString Widget::getBaseDomain(QString host) {
  int domainLabelCount = 2;
  foreach (QString prefix, twoLevelDomainPrefix_) {
    if (host.endsWith(prefix)) {
      domainLabelCount = 3;
      break;
    }
  }
  qDebug() << "domainLabelCount" << domainLabelCount; // gilgil temp 2015.11.03

  QStringList baseDomains = host.split('.');
  QString res;
  while (true) {
    if (baseDomains.count() == domainLabelCount) {
      res = baseDomains.join('.');
      break;
    }
    baseDomains.removeFirst();
    if (baseDomains.count() == 0) {
      QMessageBox::warning(this, "Error", "Can not retrieve baseDomain for " + host);
      return "";
    }
  }
  return res;
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

#include <QDateTime>
#include <QProcess>
void Widget::on_pbGo_clicked()
{
  if (ui->twCookie->selectedItems().count() == 0)
    return;
  QTreeWidgetItem* item = ui->twCookie->selectedItems().at(0);
  if (item == nullptr) return;
  CookieTreeWidgetItem* cookieItem = dynamic_cast<CookieTreeWidgetItem*>(item);
  Q_ASSERT(cookieItem != nullptr);

  if (!QFile::exists(sqliteFileName_)) {
    QMessageBox::warning(this, "Error", QString("File %1 not exists").arg(sqliteFileName_));
    return;
  }

  Cookies cookies = cookieItem->cookies_;

  QSqlQuery query(db_);

  QString baseDomain = getBaseDomain(cookies.host);
  QString host = "." + baseDomain;
  uint now = QDateTime::currentDateTime().toTime_t();
  uint expiry = now  + 86400;
  QString lastAccessed = QString::number(now) + "000000";
  QString creationTime = QString::number(now) + "000000";

  QString sql = QString("delete from moz_cookies where baseDomain='%1'").arg(baseDomain);
  qDebug() << sql;
  if (!query.exec(sql)) {
    QMessageBox::warning(this, "Error", query.lastError().text());
  }

  foreach (Cookie cookie, cookies) {
    query.exec("select max(id) from moz_cookies;");
    if (!query.exec()) {
      QMessageBox::warning(this, "Error", query.lastError().text());
      return;
    }
    query.next();
    int id = query.value(0).toInt() + 1;

    QString sql = QString(
      "INSERT INTO moz_cookies (id,  baseDomain, appId, inBrowserElement, name, value, host, path, expiry, lastAccessed, creationTime, isSecure, isHttpOnly)"\
      " VALUES                 ( %1, '%2',       0,     0,                '%3', '%4',  '%5', '/', %6, 1446460077220140, 1446460077220140, 0, 0);")
      .arg(QString::number(id), baseDomain, cookie.name, cookie.value, host).arg(expiry).arg(lastAccessed, creationTime);
    qDebug() << sql;
    if (!query.exec(sql)) {
      QMessageBox::warning(this, "Error", query.lastError().text());
      break;
    }
  }

  QStringList commands; commands << QString("http://%1").arg(baseDomain);
  QProcess* process = new QProcess;
  process->start("firefox", commands); // do not delete process

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

  setControl();
}

void Widget::on_twCookie_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
  (void)item;
  (void)column;
  ui->pbGo->click();
}
