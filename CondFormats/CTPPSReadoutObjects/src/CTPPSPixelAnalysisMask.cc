/****************************************************************************
 *
 * F.Ferro ferro@ge.infn.it
 *
 ****************************************************************************/

#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelAnalysisMask.h"
#include <cstdint>

void CTPPSPixelAnalysisMask::insert(const uint32_t &sid, const CTPPSPixelROCAnalysisMask &am) {
  analysisMask[sid] = am;
}
