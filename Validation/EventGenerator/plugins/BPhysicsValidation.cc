///////////////////////////////////////
// 
// class Validation: Class to fill dqm monitor elements from existing EDM file
//
///////////////////////////////////////
 
#include "Validation/EventGenerator/interface/BPhysicsValidation.h"

#include "FWCore/Framework/interface/MakerMacros.h"  

using namespace edm;

BPhysicsValidation::BPhysicsValidation(const edm::ParameterSet& iPSet): 
  genparticleCollection_(iPSet.getParameter<edm::InputTag>("genparticleCollection")),
  // do not include weights right now to allow for running on aod
  name(iPSet.getParameter< std::string>("name")),
  particle(name,iPSet)
{    
  genparticleCollectionToken_=consumes<reco::GenParticleCollection>(genparticleCollection_);
  std::vector<std::string> daughterNames = iPSet.getParameter< std::vector<std::string> >("daughters"); 
  for (auto curSet : daughterNames){ 
    daughters.push_back(ParticleMonitor(name+curSet,iPSet.getUntrackedParameter< ParameterSet >(curSet)));
  } 
}

BPhysicsValidation::~BPhysicsValidation(){}

void BPhysicsValidation::dqmBeginRun(const edm::Run& r, const edm::EventSetup& c) {}

void BPhysicsValidation::bookHistograms(DQMStore::IBooker &i, edm::Run const &, edm::EventSetup const &){
  DQMHelper dqm(&i); i.setCurrentFolder("Generator/BPhysics");
  Nobj   = dqm.book1dHisto("N"+name, "N"+name, 1, 0., 1,"bin","Number of "+name);
  particle.Configure(i);
  for(auto & daughter : daughters){daughter.Configure(i);}
}

void BPhysicsValidation::analyze(const edm::Event& iEvent,const edm::EventSetup& iSetup){ 
  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByToken(genparticleCollectionToken_, genParticles );
  for (const auto & iter : *genParticles) {
    if(abs(iter.pdgId())==abs(particle.PDGID())){
      Nobj->Fill(0.5, 1.0);
      particle.Fill(&iter, 1.0);
      FillDaughters(&iter);
    }
  }
}

void BPhysicsValidation::FillDaughters(const reco::GenParticle* p){
  int mpdgid=p->pdgId();
  for(unsigned int i = 0; i <p->numberOfDaughters(); i++){
    const reco::GenParticle *dau=static_cast<const reco::GenParticle*>(p->daughter(i));
    int pdgid = dau->pdgId();
    for(auto & daughter : daughters){
      if(abs(mpdgid)!=abs(daughter.PDGID()) && daughter.PDGID()==pdgid)daughter.Fill(dau,1.0);
      // note: use abs when comparing to mother to avoid mixing
    }
    FillDaughters(dau);
  }
}


