#include <netinet/in.h>
#include "cookiesniffer.h"

// ----------------------------------------------------------------------------
// CookieSnifferThread
// ----------------------------------------------------------------------------
void CookieSnifferThread::run() {
  CookieSniffer* cs = dynamic_cast<CookieSniffer*>(parent());
  Q_ASSERT(cs != nullptr);
  cs->run();
};

// ----------------------------------------------------------------------------
// CookieSniffer
// ----------------------------------------------------------------------------
CookieSniffer::CookieSniffer() {
}

CookieSniffer::~CookieSniffer() {
  close();
}

bool CookieSniffer::doOpen() {
  if (pcap_ != nullptr) {
    SET_ERR(GStdErr(GErr::VALUE_IS_NOT_NULL, "pcap is not null"));
    return false;
  }
  char errBuf[PCAP_ERRBUF_SIZE];
  pcap_ = pcap_open_live(qPrintable(dev_), snapLen_, flags_, readTimeout_, errBuf);
  if (pcap_ == nullptr) {
    SET_ERR(GStdErr(GErr::RETURN_NULL, errBuf));
    return false;
  }

  bool filtering = false;
  int dataLink = pcap_datalink(pcap_);
  switch (dataLink) {
    case DLT_NULL:
    case DLT_EN10MB:
      filtering = true;
  }
  if (filtering && filter_ != "") {
    if (!pcapProcessFilter(nullptr)) // gilgil temp 2015.10.28
      return false;
  }

  Q_ASSERT(!thread_.isRunning());
  thread_.start();

  return true;
}

bool CookieSniffer::doClose() {
  thread_.quit();
  if (!thread_.wait(10000)) {
    qCritical() << "captureThread_.wait return false" << metaObject()->className();
    thread_.terminate();
    return false;
  }

  if (pcap_ != nullptr) {
    pcap_close(pcap_);
    pcap_ = nullptr;
  }

  return true;
}

void CookieSniffer::run() {
  qDebug() << "stt"; // gilgil temp 2015.10.28
  while (active()) {
    pcap_pkthdr* hdr;
    const u_char* data;
    int res = pcap_next_ex(pcap_, &hdr, &data);
    if (res == 0) continue;
    if (res < 0) break;
    processPacket(hdr, data);
  }
  qDebug() << "end"; // gilgil temp 2015.10.28
  emit closed();
}

bool CookieSniffer::pcapProcessFilter(pcap_if_t* dev)
{
  u_int uNetMask;
  bpf_program code;

  if (dev != NULL && dev->addresses != NULL && dev->addresses->netmask != NULL)
    uNetMask = ((struct sockaddr_in*)(dev->addresses->netmask))->sin_addr.s_addr;
  else
    uNetMask = 0xFFFFFFFF;
  if (pcap_compile(pcap_, &code, qPrintable(filter_), 1, uNetMask) < 0)
  {
    SET_ERR(GStdErr(GErr::UNKNOWN, QString("error in pcap_compile(%1)").arg(pcap_geterr(pcap_))));
    return false;
  }
  if (pcap_setfilter(pcap_, &code) < 0)
  {
    SET_ERR(GStdErr(GErr::UNKNOWN, QString("error in pcap_setfilter(%1)").arg(pcap_geterr(pcap_))));
    return false;
  }
  return true;
}

void CookieSniffer::processPacket(pcap_pkthdr* hdr, const u_char* data) {
  (void)data;
  qDebug() << "caplen=" << hdr->caplen;
}
