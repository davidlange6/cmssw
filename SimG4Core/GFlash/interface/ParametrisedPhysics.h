#ifndef SimG4Core_GFlash_ParametrisedPhysics_H
#define SimG4Core_GFlash_ParametrisedPhysics_H

#include "G4VPhysicsConstructor.hh"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimG4Core/GFlash/interface/GflashEMShowerModel.h"
#include "SimG4Core/GFlash/interface/GflashHadronShowerModel.h"
#include "G4FastSimulationManagerProcess.hh"

// Joanna Weng 08.2005
// Physics process for Gflash parameterisation
// modified by Soon Yung Jun, Dongwook Jang

class ParametrisedPhysics : public G4VPhysicsConstructor
{
 public:
  ParametrisedPhysics(const std::string& name, const edm::ParameterSet & p);
  virtual ~ParametrisedPhysics();
	
 protected:
  virtual void ConstructParticle();
  virtual void ConstructProcess();

 private:
  edm::ParameterSet theParSet;
  struct ThreadPrivate {
    GflashEMShowerModel *theEMShowerModel;
    GflashEMShowerModel *theHadShowerModel;
    GflashHadronShowerModel *theHadronShowerModel;
    G4FastSimulationManagerProcess *theFastSimulationManagerProcess;
  };
  static G4ThreadLocal ThreadPrivate* tpdata;    
};

#endif

