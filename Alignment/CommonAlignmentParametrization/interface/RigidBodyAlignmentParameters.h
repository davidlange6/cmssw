#ifndef Alignment_CommonAlignment_RigidBodyAlignmentParameters_h
#define Alignment_CommonAlignment_RigidBodyAlignmentParameters_h

#include "Alignment/CommonAlignment/interface/AlignmentParameters.h"
#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"

/// \class RigidBodyAlignmentParameters
///
/// Concrete class for alignment parameters and associated quantities 
/// [derived from AlignmentParameters]. The number of parameters
/// N_PARAM is fixed to 6 (3 translations + 3 rotations)
///
///  $Date: 2007/10/08 15:56:00 $
///  $Revision: 1.12 $
/// (last update by $Author: cklae $)

class Alignable;
class AlignableDetOrUnitPtr;
class TrajectoryStateOnSurface;

class RigidBodyAlignmentParameters : public AlignmentParameters 
{

public:

  /// Give parameters a name
  enum AlignmentParameterName 
	{
	  dx = 0, dy, dz,
	  dalpha, dbeta, dgamma,
	  N_PARAM
	};

  /// Constructor with empty parameters/covariance (if calcMis = false) or with parameters
  /// (no covariance) created from current (mis-)placement of alignable (if calcMis = true).
  RigidBodyAlignmentParameters(Alignable* alignable, bool calcMis);

  /// Constructor for full set of parameters
  RigidBodyAlignmentParameters( Alignable* alignable, 
				const AlgebraicVector& parameters, 
				const AlgebraicSymMatrix& covMatrix );

  /// Constructor for selection 
  RigidBodyAlignmentParameters( Alignable* alignable, const AlgebraicVector& parameters, 
				const AlgebraicSymMatrix& covMatrix, 
				const std::vector<bool>& selection );

  /// Destructor 
  virtual ~RigidBodyAlignmentParameters() {};
  virtual void apply();
  virtual int type() const;

  /// Clone all parameters (for update of parameters)
  virtual RigidBodyAlignmentParameters* clone( const AlgebraicVector& parameters, 
					       const AlgebraicSymMatrix& covMatrix ) const;
 
  /// Clone selected parameters (for update of parameters)
  virtual RigidBodyAlignmentParameters*
    cloneFromSelected(const AlgebraicVector& parameters, const AlgebraicSymMatrix& covMatrix) const;
  
  /// Get all derivatives 
  virtual AlgebraicMatrix derivatives( const TrajectoryStateOnSurface& tsos,
				       const AlignableDetOrUnitPtr & ) const;

  /// Get selected derivatives
  virtual AlgebraicMatrix selectedDerivatives( const TrajectoryStateOnSurface& tsos, 
					       const AlignableDetOrUnitPtr & ) const;

  /// Get translation parameters
  AlgebraicVector translation() const;

  /// Get rotation parameters
  AlgebraicVector rotation() const;

  /// calculate and return parameters in global frame 
  AlgebraicVector globalParameters() const;

  /// print parameters to screen 
  void print() const;

  /// Calculate parameter vector of misplacements (shift+rotation) from alignable.
  /// (If ali=0, return empty AlgebraicVector of proper length.)
  static AlgebraicVector displacementFromAlignable(const Alignable* ali);

};

#endif

