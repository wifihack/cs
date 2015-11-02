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
  QObject::connect(&pcap_, SIGNAL(captured(GPacket*)), this, SLOT(processPacket(GPacket*)), Qt::DirectConnection);
  QObject::connect(&pcap_, SIGNAL(closed()), this, SLOT(processClose()), Qt::AutoConnection);
  // pcap_.dev_ = "DeSniffer0"; // gilgil temp 2015.11.02
}

CookieSniffer::~CookieSniffer() {
  close();
}

bool CookieSniffer::doOpen() {
  bool res = pcap_.open();
  if (!res) {
    err = pcap_.err;
  }
  prevHttp_.clear();
  return res;
}

bool CookieSniffer::doClose() {
  if (!pcap_.close()) {
    err = pcap_.err;
    return false;
  }
  prevHttp_.clear();
  return true;
}

void CookieSniffer::processPacket(GPacket* packet) {
  if (packet->proto != IPPROTO_TCP)
    return;

  if (packet->data == nullptr)
    return;

  QByteArray ba((const char*)packet->data, packet->dataLen);
  QString http(ba);

  GTransportSessionKey key;
  key.ip1 = packet->ipHdr->ip_src;
  key.port1 = packet->tcpHdr->th_sport;
  key.ip2 = packet->ipHdr->ip_dst;
  key.port2 = packet->tcpHdr->th_dport;
  NotCompletedCookiesMap::iterator it = prevHttp_.find(key);
  if (it != prevHttp_.end()) {
    qDebug() << "map found";
    QString prev = *it;
    http = prev + http;
    prevHttp_.remove(key);
  }

  if (!isHttpRequest(http))
    return;

  Cookies cookies;
  if (!findHost(http, cookies.host))
    return;

  if (!findCookie(http, cookies))
    return;

  if (findEndOfHeader(http)) {
    cookies.status = Cookies::Completed;
  } else {
    cookies.status = Cookies::NotCompleted;
    GTransportSessionKey key;
    key.ip1 = packet->ipHdr->ip_src;
    key.port1 = packet->tcpHdr->th_sport;
    key.ip2 = packet->ipHdr->ip_dst;
    key.port2 = packet->tcpHdr->th_dport;
    prevHttp_[key] = http;
    qDebug() << "map inserted";
    return;
  }

  QDateTime time_;
  time_.setTime_t(packet->pktHdr->ts.tv_sec);
  cookies.time = time_;
  GIp ip = htonl(packet->ipHdr->ip_src);
  cookies.ip = QString(ip);

  emit captured(cookies);
}

void CookieSniffer::processClose() {
  emit closed();
}

bool CookieSniffer::isHttpRequest(QString& http) {
  if (http.startsWith("GET ")) {
    return true;
  }
  if (http.startsWith("POST ")) {
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
  qDebug() << "length=" << s.length(); // gilgil temp 2015.11.03
  return cookies.decode(s);
}

bool CookieSniffer::findEndOfHeader(QString& http) {
  static QRegExp rexEndOfHeader("\r\n\r\n");
  return rexEndOfHeader.indexIn(http) != -1;
}
