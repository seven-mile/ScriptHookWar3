#pragma once

struct JassHandle
{
  HObject handle;
  JassHandle(HObject const &handle) : handle(handle) {  }
};

