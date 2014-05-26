import FWCore.ParameterSet.Config as cms

#                                                    
# Full-scale Digitization of the simulated hits      
# in all CMS subdets : Tracker, ECAL, HCAl, Muon's;  
# MixingModule (at least in zero-pileup mode) needs  
# to be included to make Digi's operational, since   
# it's required for ECAL/HCAL & Muon's                
# Defined in a separate fragment
#                                                    
# Tracker Digis (Pixel + SiStrips) are now made in the mixing
# module, so the old "trDigi" sequence has been taken out.
#

# Calorimetry Digis (Ecal + Hcal) - * unsuppressed *
# returns sequence "calDigi"
from SimCalorimetry.Configuration.SimCalorimetry_cff import *
# Muon Digis (CSC + DT + RPC)
# returns sequence "muonDigi"
#
from SimMuon.Configuration.SimMuon_cff import *
#
# TrackingParticle Producer is now part of the mixing module, so
# it is no longer run here.
#
from SimGeneral.Configuration.SimGeneral_cff import *

#
# Gen particle stuff now goes here
from PhysicsTools.HepMCCandAlgos.genParticles_cfi import *
from RecoJets.Configuration.RecoGenJets_cff import *
from RecoMET.Configuration.RecoGenMET_cff import *
from RecoJets.Configuration.GenJetParticles_cff import *
from RecoMET.Configuration.GenMETParticles_cff import *

GeneInfo = cms.Sequence(genParticles)
genJetMET = cms.Sequence(genJetParticles*recoGenJets+genMETParticles*recoGenMET)

genInfo = cms.Sequence(GeneInfo * genJetMET)

doAllDigi = cms.Sequence(calDigi+muonDigi)
pdigi = cms.Sequence(genInfo+cms.SequencePlaceholder("randomEngineStateProducer")*cms.SequencePlaceholder("mix")*doAllDigi*addPileupInfo)
pdigi_valid = cms.Sequence(pdigi)

# for heavy ions
#
#hiGenJets = cms.Sequence(hiGenParticlesForJets*hiRecoGenJets)
#
#from PhysicsTools.HepMCCandAlgos.HiGenParticles_cfi import *
#
#from SimGeneral.MixingModule.MatchVtx_cfi import *
#
#pgen_himix = cms.Sequence(cms.SequencePlaceholder("randomEngineStateProducer")+matchVtx+hiGenParticles+hiGenJets)

