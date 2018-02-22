#ifndef __CFG_H__
#define __CFG_H__
#include <string>
#include <iostream>

#include "yaml-cpp/yaml.h"

struct {
    float jetMinPt;
} cfg;

void read_config(const std::string&);

#endif  // __CFG_H__
