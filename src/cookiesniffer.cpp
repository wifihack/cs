#include "cookiesniffer.h"
#include <QRegExp>

// ----------------------------------------------------------------------------
// CookieSniffer
// ----------------------------------------------------------------------------
CookieSniffer::CookieSniffer(QObject* parent) : GStateObj(parent) {
  QObject::connect(&pcap_, SIGNAL(captured(GPacket*)), this, SLOT(process(GPacket*)), Qt::DirectConnection);
}

CookieSniffer::~CookieSniffer() {
  close();
}

bool CookieSniffer::doOpen() {
  bool res = pcap_.open();
  if (!res) {
    err = pcap_.err;
  }
  return res;
}

bool CookieSniffer::doClose() {
  return pcap_.close();
}

void CookieSniffer::process(GPacket* packet) {
  if (packet->proto != IPPROTO_TCP) {
    qDebug() << "not tcp";
    return;
  }
  if (packet->data == nullptr) {
    qDebug() << "data is null";
    return;
  }

  QByteArray ba((const char*)packet->data, packet->dataLen);
  QString http(ba);
  if (!isHttpRequest(http))
    return;
  QString host;
  if (findHost(http, host)) {
    qDebug() << "host=" << host;
  }
}

bool CookieSniffer::isHttpRequest(QString& http) {
  if (http.startsWith("GET ")) {
    http = http.mid(4);
    return true;
  }
  if (http.startsWith("POST ")) {
    http = http.mid(5);
    return true;
  }
  return false;
}

bool CookieSniffer::findHost(QString& http, QString& host) {
  static QRegExp rexHost("\r\nHost: ([^\r]*)");
  int i = rexHost.indexIn(http);
  if (i == -1) return false;
  host = rexHost.cap(1);
  return true;
}
