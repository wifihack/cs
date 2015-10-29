#pragma once

#include <GStateObj>
#include <GPcap>

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
  QString ip;
  QString host;

  bool decode(QString& s);
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

  GPcap pcap_;

public slots:
  void process(GPacket* packet);

signals:
  void captured(Cookies cookies);

protected:
  bool isHttpRequest(QString& http);
  bool findHost(QString& http, QString& host);
  bool findCookie(QString& http, Cookies& cookies);
};
