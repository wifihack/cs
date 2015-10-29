#include "cookiesniffer.h"
#include <QRegExp>

// ----------------------------------------------------------------------------
// Cookie
// ----------------------------------------------------------------------------
bool Cookie::decode(QString& s) {
  s = s.trimmed();
  int i = s.indexOf('=');
  if (i == -1)
    return false;
  name = s.left(i);
  value = s.mid(i + 1);
  return true;
}

// ----------------------------------------------------------------------------
// Cookies
// ----------------------------------------------------------------------------
bool Cookies::decode(QString& s) {
  QStringList sl = s.split(';');
  foreach (QString one, sl) {
    Cookie cookie;
    if (!cookie.decode(one))
      break;
    push_back(cookie);
  }
  if (sl.count() == 0)
    return false;
 return true;
}

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
  if (!pcap_.close()) {
    err = pcap_.err;
    return false;
  }
  return true;
}

void CookieSniffer::process(GPacket* packet) {
  if (packet->proto != IPPROTO_TCP)
    return;

  if (packet->data == nullptr)
    return;

  QByteArray ba((const char*)packet->data, packet->dataLen);
  QString http(ba);

  if (!isHttpRequest(http))
    return;

  Cookies cookies;
  if (!findHost(http, cookies.host))
    return;
  // qDebug() << "host=" << host; // gilgil temp 2015.10.29

  if (!findCookie(http, cookies))
    return;

  GIp ip = htonl(packet->ipHdr->ip_src);
  cookies.ip = QString(ip);

  emit captured(cookies);
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

bool CookieSniffer::findCookie(QString& http, Cookies& cookies) {
  static QRegExp rexCookie("\r\nCookie: ([^\r]*)");
  int i = rexCookie.indexIn(http);
  if (i == -1) return false;
  QString s = rexCookie.cap(1);
  return cookies.decode(s);
}
