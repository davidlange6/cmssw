#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include <cstdint>

EncodedEventId::EncodedEventId() : id_(0) {}
EncodedEventId::EncodedEventId(uint32_t id) : id_(id) {}
