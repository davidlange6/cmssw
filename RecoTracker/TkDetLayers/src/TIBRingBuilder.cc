#include "TIBRingBuilder.h"

using namespace edm;
using namespace std;

TIBRing* TIBRingBuilder::build(const vector<const GeometricDet*>& detsInRing,
			       const TrackerGeometry* theGeomDetGeometry){
  vector<const GeomDet*> theGeomDets;
  for(auto it : detsInRing){

    const GeomDet* theGeomDet = theGeomDetGeometry->idToDet( it->geographicalID() );
    theGeomDets.push_back(theGeomDet);    
  }
  
  return new TIBRing(theGeomDets);  
}
