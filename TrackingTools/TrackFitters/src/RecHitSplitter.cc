#include "TrackingTools/TrackFitters/interface/RecHitSplitter.h"

RecHitSplitter::RecHitContainer
RecHitSplitter::split(const RecHitContainer& hits) const {

  RecHitContainer singles; singles.reserve(2*hits.size());

  for(const auto & hit : hits) {
    if(!(*hit).isValid()) {
      singles.push_back(hit);
    } else {
      RecHitContainer shits = (*hit).transientHits();
      for(RecHitContainer::const_iterator ishit = shits.begin();
	  ishit != shits.end(); ishit++) {
	singles.push_back(*ishit);
      }
    }
  }
  return singles;
}
