#include "DataFormats/METReco/interface/EcalHaloData.h"
/*
  [class]:  EcalHaloData
  [authors]: R. Remington, The University of Florida
  [description]: See EcalHaloData.h
  [date]: October 15, 2009
*/

using namespace reco;
EcalHaloData::EcalHaloData()
{
}

int EcalHaloData::NumberOfHaloSuperClusters(float roundness, float angle ) const 
{

  int n = 0 ; 
  for(const auto & TheSuperClusterRef : TheSuperClusterRefs)
    {
      edm::Ref<SuperClusterCollection> cluster(TheSuperClusterRef) ;
      float r = ShowerShapes_Roundness[cluster];
      float a = ShowerShapes_Angle[cluster];
      
      if( (r > 0 && r < roundness) && ( a > 0 && a < angle ) ) 
	n++ ;
    }

  return  n ;
}
