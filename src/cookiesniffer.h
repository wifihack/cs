#pragma once

#include <GStateObj>
#include <GPcap>

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

protected:
  bool isHttpRequest(QString& http);
  bool findHost(QString& http, QString& host);
};
