#pragma once
#include "JassHandle.h"
struct JassWidget :
    public JassHandle
{
  JassWidget(HObject handle) : JassHandle(handle) {  }
};

