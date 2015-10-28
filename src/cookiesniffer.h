#pragma once

#include <GStateObj>

struct CookieSniffer : GStateObj {
  Q_OBJECT
  Q_PROPERTY(QString adapterName MEMBER adapterName_)

public:
  CookieSniffer();

public:
  QString adapterName_;
};
