#ifndef DataFormats_TestObjects_ThingWithMerge_h
#define DataFormats_TestObjects_ThingWithMerge_h

#include "FWCore/Utilities/interface/typedefs.h"
#include <cstdint>

namespace edmtest {

  struct ThingWithMerge {
    ~ThingWithMerge() {}
    ThingWithMerge() : a() {}
    bool mergeProduct(ThingWithMerge const& newThing);
    void swap(ThingWithMerge& iOther);
    cms_int32_t a;
  };

}  // namespace edmtest

#endif
