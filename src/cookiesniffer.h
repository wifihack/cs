#pragma once

#include <QDateTime>
#include <QMap>
#include <GStateObj>
#include <GPcap>
#include <GPacketKey>

// ----------------------------------------------------------------------------
// Cookie
// ----------------------------------------------------------------------------
struct Cookie {
  QString name;
  QString value;

  bool decode(QString& s);
};

// ----------------------------------------------------------------------------
// Cookies
// ----------------------------------------------------------------------------
struct Cookies : QList<Cookie> {
  typedef enum {
    NotCompleted,
    Completed
  } Status;

  QDateTime time;
  QString ip;
  QString host;
  QString url;
  Status status;

  bool decode(QString& s);
};

// ----------------------------------------------------------------------------
// NotCompletedCookiesMap
// ----------------------------------------------------------------------------
struct NotCompletedCookiesMap: QMap<GTransportSessionKey /*key*/, QString /*http*/> {

};

// ----------------------------------------------------------------------------
// CookieSniffer
// ----------------------------------------------------------------------------
struct CookieSniffer : GStateObj {
  Q_OBJECT
  Q_PROPERTY(QObject* pcap READ getPcap)

  QObject* getPcap() { return &pcap_; }

public:
  CookieSniffer(QObject* parent = nullptr);
  ~CookieSniffer() override;

protected:
  bool doOpen() override;
  bool doClose() override;

public:
  GPcap pcap_;

public slots:
  void processPacket(GPacket* packet);
  void processClose();

signals:
  void captured(Cookies cookies);

protected:
  NotCompletedCookiesMap prevHttp_;

  bool isHttpRequest(QString& http, QString& uri);
  bool findHost(QString& http, QString& host);
  bool findCookie(QString& http, Cookies& cookies);
  bool findEndOfHeader(QString& http);
};
