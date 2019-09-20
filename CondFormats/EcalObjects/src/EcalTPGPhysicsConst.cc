#include "CondFormats/EcalObjects/interface/EcalTPGPhysicsConst.h"
#include <cstdint>

EcalTPGPhysicsConst::EcalTPGPhysicsConst() {}

EcalTPGPhysicsConst::~EcalTPGPhysicsConst() {}

void EcalTPGPhysicsConst::setValue(const uint32_t& id, const Item& value) { map_[id] = value; }
