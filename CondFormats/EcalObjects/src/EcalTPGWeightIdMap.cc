#include "CondFormats/EcalObjects/interface/EcalTPGWeightIdMap.h"
#include <cstdint>

EcalTPGWeightIdMap::EcalTPGWeightIdMap() {}

EcalTPGWeightIdMap::~EcalTPGWeightIdMap() {}

void EcalTPGWeightIdMap::setValue(const uint32_t& id, const EcalTPGWeights& value) { map_[id] = value; }
