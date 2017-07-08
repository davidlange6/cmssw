#ifndef _SimTracker_SiPhase2Digitizer_PSSDigitizerAlgorithm_h
#define _SimTracker_SiPhase2Digitizer_PSSDigitizerAlgorithm_h

#include "SimTracker/SiPhase2Digitizer/plugins/Phase2TrackerDigitizerAlgorithm.h"

// forward declarations
class TrackerTopology;

class PSSDigitizerAlgorithm :public Phase2TrackerDigitizerAlgorithm {
 public:
  PSSDigitizerAlgorithm(const edm::ParameterSet& conf, CLHEP::HepRandomEngine&);
  virtual ~PSSDigitizerAlgorithm();

  // initialization that cannot be done in the constructor
  void init(const edm::EventSetup& es);
  
  // void initializeEvent();
  // run the algorithm to digitize a single det
  void accumulateSimHits(std::vector<PSimHit>::const_iterator inputBegin,
                         std::vector<PSimHit>::const_iterator inputEnd,
                         size_t inputBeginGlobalIndex,
			 unsigned int tofBin,
                         const Phase2TrackerGeomDetUnit* pixdet,
                         const GlobalVector& bfield);
};
#endif
