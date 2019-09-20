#include "CondFormats/EcalObjects/interface/EcalTPGLutIdMap.h"
#include <cstdint>

EcalTPGLutIdMap::EcalTPGLutIdMap() {}

EcalTPGLutIdMap::~EcalTPGLutIdMap() {}

void EcalTPGLutIdMap::setValue(const uint32_t& id, const EcalTPGLut& value) { map_[id] = value; }
