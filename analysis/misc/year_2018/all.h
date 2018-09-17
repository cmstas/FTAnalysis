#ifndef YEAR2018ALL
#define YEAR2018ALL

// FIXME many things just point to 2017 - rederive
namespace y2018 {
#include "puw.h"
#include "../year_2017/fake_rates.h"
#include "../year_2017/flip_rates.h"
#include "../year_2017/lepton_sf.h"
#include "../year_2017/trigger_sf.h"
#include "../year_2017/isr_weights.h"
float getLumi() { return 35.5; }
}

#endif
