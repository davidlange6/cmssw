#include "CondFormats/EcalObjects/interface/EcalTPGStripStatus.h"
#include <cstdint>

EcalTPGStripStatus::EcalTPGStripStatus() {}

EcalTPGStripStatus::~EcalTPGStripStatus() {}

void EcalTPGStripStatus::setValue(const uint32_t& id, const uint16_t& val) { map_[id] = val; }
