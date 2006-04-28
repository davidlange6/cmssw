#ifndef TransientInitialStateEstimator_H
#define TransientInitialStateEstimator_H

#include "DataFormats/Common/interface/OwnVector.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"

#include <utility>

class Propagator;
class GeomDet;
class Trajectory;
namespace edm { class EventSetup;};

/// Computes the trajectory state to be used as a starting state for the track fit
/// from the vector of hits. The parameters of this stat are close to the final fit parameters.
/// The error matrix is enlarged in order not to bias the track fit.

class TransientInitialStateEstimator {
public:

  typedef TrajectoryStateOnSurface TSOS;

  TransientInitialStateEstimator( const edm::EventSetup& es);

  std::pair<TrajectoryStateOnSurface, const GeomDet*>
  innerState( const Trajectory& traj) const;


private:

  Propagator* theReversePropagator;
  Propagator* theForwardPropagator;


};

#endif
