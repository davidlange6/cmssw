import FWCore.ParameterSet.Config as cms

cIC= cms.untracked.vstring('keep *', 
        'drop *_recoGenParticles_*_*', 
        'drop recoGenJets_*_*_*', 
        'drop recoGenMETs_*_*_*')

