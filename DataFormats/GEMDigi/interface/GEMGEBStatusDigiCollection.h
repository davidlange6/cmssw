#ifndef DataFormats_GEMDigi_GEMGEBStatusDigiCollection_h
#define DataFormats_GEMDigi_GEMGEBStatusDigiCollection_h

#include "DataFormats/GEMDigi/interface/GEMGEBStatusDigi.h"
#include "DataFormats/MuonData/interface/MuonDigiCollection.h"
#include <cstdint>

typedef MuonDigiCollection<uint16_t, GEMGEBStatusDigi> GEMGEBStatusDigiCollection;

#endif

