#ifndef __CFG_H__
#define __CFG_H__

#include <string>
#include <iostream>

struct Config {
    float jetMinPt;
    float bjetMinPt;
};

extern Config cfg;

bool read_config(const std::string&);

#endif  // __CFG_H__
