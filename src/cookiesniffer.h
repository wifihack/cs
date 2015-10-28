#pragma once

#include <GStateObj>

struct CookieSniffer : GStateObj {
  Q_OBJECT
  Q_PROPERTY(QString adapterName MEMBER adapterName_)

public:
  CookieSniffer();

public:
  QString adapterName_{"dum0"}; // gilgil temp 2015.10.28
};
