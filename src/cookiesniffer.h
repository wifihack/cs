#pragma once

#include <pcap.h>
#include <QThread>
#include <GStateObj>

// ----------------------------------------------------------------------------
// CookieSnifferThread
// ----------------------------------------------------------------------------
struct CookieSnifferThread : QThread {
  CookieSnifferThread(QObject *parent = nullptr) : QThread(parent) {}
protected:
  void run() override;
};

// ----------------------------------------------------------------------------
// CookieSniffer
// ----------------------------------------------------------------------------
struct CookieSniffer : GStateObj {
  Q_OBJECT
  Q_PROPERTY(QString dev         MEMBER dev_)
  Q_PROPERTY(QString filter      MEMBER filter_)
  Q_PROPERTY(int     snapLen     MEMBER snapLen_)
  Q_PROPERTY(int     flags       MEMBER flags_)
  Q_PROPERTY(int     readTimeout MEMBER readTimeout_)

  friend class CookieSnifferThread;

public:
  CookieSniffer();
  ~CookieSniffer() override;

  bool doOpen() override;
  bool doClose() override;

  QString dev_{"eth0"};// gilgil temp 2015.10.28
  QString filter_{""};
  int     snapLen_{65536}; // gilgil temp 2015.10.28
  int     flags_{1}; // PCAP_OPENFLAG_PROMISCUOUS
  int     readTimeout_{1};

protected:
  void run();
  bool pcapProcessFilter(pcap_if_t* dev);
  void processPacket(pcap_pkthdr* hdr, const u_char* data);

  pcap_t*  pcap_{nullptr};
  CookieSnifferThread thread_{this};
};
