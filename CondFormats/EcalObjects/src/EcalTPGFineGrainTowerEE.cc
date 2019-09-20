#include "CondFormats/EcalObjects/interface/EcalTPGFineGrainTowerEE.h"
#include <cstdint>

EcalTPGFineGrainTowerEE::EcalTPGFineGrainTowerEE() {}

EcalTPGFineGrainTowerEE::~EcalTPGFineGrainTowerEE() {}

void EcalTPGFineGrainTowerEE::setValue(const uint32_t& id, const uint32_t& lut) { map_[id] = lut; }
