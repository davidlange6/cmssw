#ifndef FWCore_Framework_limited_EDProducer_h
#define FWCore_Framework_limited_EDProducer_h
// -*- C++ -*-
//
// Package:     FWCore/Framework
// Class  :     edm::limited::EDProducer
//
/**\class edm::limited::EDProducer EDProducer.h "FWCore/Framework/interface/limited/EDProducer.h"

 Description: [one line class summary]

 Usage:
    <usage>

*/
//
// Original Author:  Chris Jones
//         Created:  Thu, 18 Jul 2013 11:51:07 GMT
//

// system include files

// user include files
#include "FWCore/Framework/interface/limited/producerAbilityToImplementor.h"
#include "FWCore/Framework/interface/moduleAbilities.h"

// forward declarations

namespace edm {
  namespace limited {
    template <typename... T>
    class EDProducer : public producer::SpecializeAbilityToImplementor<
                           CheckAbility<edm::module::Abilities::kRunSummaryCache, T...>::kHasIt &
                               CheckAbility<edm::module::Abilities::kEndRunProducer, T...>::kHasIt,
                           CheckAbility<edm::module::Abilities::kLuminosityBlockSummaryCache, T...>::kHasIt &
                               CheckAbility<edm::module::Abilities::kEndLuminosityBlockProducer, T...>::kHasIt,
                           T>::Type...,
                       public virtual EDProducerBase {
    public:
      EDProducer(edm::ParameterSet const& iPSet)
          : EDProducerBase(iPSet),
            producer::SpecializeAbilityToImplementor<
                CheckAbility<edm::module::Abilities::kRunSummaryCache, T...>::kHasIt &
                    CheckAbility<edm::module::Abilities::kEndRunProducer, T...>::kHasIt,
                CheckAbility<edm::module::Abilities::kLuminosityBlockSummaryCache, T...>::kHasIt &
                    CheckAbility<edm::module::Abilities::kEndLuminosityBlockProducer, T...>::kHasIt,
                T>::Type(iPSet)... {}

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------

    private:
      EDProducer(const EDProducer&) = delete;

      const EDProducer& operator=(const EDProducer&) = delete;

      // ---------- member data --------------------------------
    };
  }
}

#endif
