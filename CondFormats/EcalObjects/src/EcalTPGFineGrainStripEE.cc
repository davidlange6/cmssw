#include "CondFormats/EcalObjects/interface/EcalTPGFineGrainStripEE.h"
#include <cstdint>

EcalTPGFineGrainStripEE::EcalTPGFineGrainStripEE() {}

EcalTPGFineGrainStripEE::~EcalTPGFineGrainStripEE() {}

void EcalTPGFineGrainStripEE::setValue(const uint32_t& id, const Item& value) { map_[id] = value; }
