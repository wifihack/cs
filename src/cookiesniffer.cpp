#include "cookiesniffer.h"

// ----------------------------------------------------------------------------
// CookieSniffer
// ----------------------------------------------------------------------------
CookieSniffer::CookieSniffer(QObject* parent) : GStateObj(parent) {
  QObject::connect(&pcap_, SIGNAL(captured(GPacket*)), this, SLOT(process(GPacket*)));
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
  qDebug() << "caplen=" << packet->pktHdr->caplen;
}
