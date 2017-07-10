/*class BasicHepMCValidation
 *  
 *  Class to fill dqm monitor elements from existing EDM file
 *
 */
 
#include "Validation/EventGenerator/interface/BasicHepMCValidation.h"

#include "CLHEP/Units/defs.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "Validation/EventGenerator/interface/DQMHelper.h"
using namespace edm;

BasicHepMCValidation::BasicHepMCValidation(const edm::ParameterSet& iPSet): 
  wmanager_(iPSet,consumesCollector()),
  hepmcCollection_(iPSet.getParameter<edm::InputTag>("hepmcCollection"))
{    
  hepmcCollectionToken_=consumes<HepMCProduct>(hepmcCollection_);
}

BasicHepMCValidation::~BasicHepMCValidation() {}

void BasicHepMCValidation::dqmBeginRun(const edm::Run& r, const edm::EventSetup& c) {
  c.getData( fPDGTable );
}

void BasicHepMCValidation::bookHistograms(DQMStore::IBooker &i, edm::Run const &, edm::EventSetup const &){
  ///Setting the DQM top directories
  DQMHelper dqm(&i); i.setCurrentFolder("Generator/Particles");
  
  // Number of analyzed events
  nEvt = dqm.book1dHisto("nEvt", "n analyzed Events", 1, 0., 1.);
  
  ///Booking the ME's
  ///multiplicity
  // quarks
  particles.emplace_back("u",1,i);
  particles.emplace_back("ubar",-1,i);
  particles.emplace_back("d",2,i);
  particles.emplace_back("dbar",-2,i);
  particles.emplace_back("s",3,i);
  particles.emplace_back("sbar",-3,i);
  particles.emplace_back("c",4,i);
  particles.emplace_back("cbar",-4,i);
  particles.emplace_back("b",5,i);
  particles.emplace_back("bbar",-5,i);
  particles.emplace_back("t",6,i);
  particles.emplace_back("tbar",-6,i);
  
  //leptons
  particles.emplace_back("eminus",11,i);
  particles.emplace_back("eplus",-11,i);
  particles.emplace_back("nue",12,i);
  particles.emplace_back("nuebar",-12,i);
  particles.emplace_back("muminus",13,i);
  particles.emplace_back("muplus",-13,i);
  particles.emplace_back("numu",14,i);
  particles.emplace_back("numubar",-14,i);
  particles.emplace_back("tauminus",15,i);
  particles.emplace_back("tauplus",-15,i);
  particles.emplace_back("nutau",16,i);
  particles.emplace_back("nutaubar",-16,i);
  
  //bosons
  particles.emplace_back("Wplus",24,i);
  particles.emplace_back("Wminus",-24,i);
  particles.emplace_back("Z",23,i);
  particles.emplace_back("gamma",22,i); 
  particles.emplace_back("gluon",21,i); 
  
  //mesons
  particles.emplace_back("piplus",211,i,true);   //log
  particles.emplace_back("piminus",-211,i,true); //log
  particles.emplace_back("pizero",111,i,true);   //log
  particles.emplace_back("Kplus",321,i);
  particles.emplace_back("Kminus",-321,i);
  particles.emplace_back("Klzero",130,i);
  particles.emplace_back("Kszero",310,i);
  
  //baryons
  particles.emplace_back("p",2212,i,true);     //log
  particles.emplace_back("pbar",-2212,i,true); //log
  particles.emplace_back("n",2112,i,true);     //log
  particles.emplace_back("nbar",-2112,i,true); //log
  particles.emplace_back("lambda0",3122,i);
  particles.emplace_back("lambda0bar",-3122,i);
  
  //D mesons
  particles.emplace_back("Dplus",411,i);
  particles.emplace_back("Dminus",-411,i);
  particles.emplace_back("Dzero",421,i);
  particles.emplace_back("Dzerobar",-421,i);
  
  //B mesons
  particles.emplace_back("Bplus",521,i);
  particles.emplace_back("Bminus",-521,i);
  particles.emplace_back("Bzero",511,i);
  particles.emplace_back("Bzerobar",-511,i);
  particles.emplace_back("Bszero",531,i);
  particles.emplace_back("Bszerobar",-531,i);

  //
  otherPtclNumber = dqm.book1dHisto("otherPtclNumber", "Log10(No. other ptcls)", 60, -1, 5,"log_{10}(No. other ptcls)","Number of Events"); //Log
  otherPtclMomentum = dqm.book1dHisto("otherPtclMomentum", "Log10(p) other ptcls", 60, -2, 4,"log10(P^{other ptcls}) (log_{10}(GeV))","Number of Events");
  
  ///other
  genPtclNumber = dqm.book1dHisto("genPtclNumber", "Log10(No. all particles)", 60, -1, 5,"log10(No. all particles)","Number of Events"); //Log
  genVrtxNumber = dqm.book1dHisto("genVrtxNumber", "Log10(No. all vertexs)", 60, -1, 5,"log10(No. all vertexs)","Number of Events"); //Log
  //
  stablePtclNumber= dqm.book1dHisto("stablePtclNumber", "Log10(No. stable particles)", 50, 0, 5,"log10(No. stable particles)","Number of Events"); //Log
  stablePtclPhi = dqm.book1dHisto("stablePtclPhi", "stable Ptcl Phi", 360, -180, 180,"#phi^{stable Ptcl} (rad)","Number of Events");
  stablePtclEta = dqm.book1dHisto("stablePtclEta", "stable Ptcl Eta (pseudo rapidity)", 220, -11, 11,"#eta^{stable Ptcl}","Number of Events");
  stablePtclCharge = dqm.book1dHisto("stablePtclCharge", "stablePtclCharge", 5, -2, 2,"Charge^{stable ptcls}","Number of Events");
  stableChaNumber= dqm.book1dHisto("stableChaNumber", "Log10(No. stable charged particles)", 50, 0, 5,"log_{10}(No. stable charged particles)","Number of Events"); //Log
  stablePtclp = dqm.book1dHisto("stablePtclp", "Log10(p) stable ptcl p", 80, -4, 4,"log_{10}(P^{stable ptcl}) (log_{10}(GeV))","Number of Events"); //Log
  stablePtclpT = dqm.book1dHisto("stablePtclpT", "Log10(pT) stable ptcl pT", 80, -4, 4,"log_{10}(P_{t}^{stable ptcl}) (log_{10}(GeV))","Number of Events"); //Log
  partonNumber = dqm.book1dHisto("partonNumber", "number of partons", 100, 0, 100,"number of partons","Number of Events");
  partonpT = dqm.book1dHisto("partonpT", "Log10(pT) parton pT", 80, -4, 4,"Log10(P_{t}^{parton})","Number of Events"); //Log
  outVrtxStablePtclNumber = dqm.book1dHisto("outVrtxStablePtclNumber", "No. outgoing stable ptcls from vrtx", 10, 0, 10,"No. outgoing stable ptcls from vrtx","Number of Events"); 
  //
  outVrtxPtclNumber = dqm.book1dHisto("outVrtxPtclNumber", "No. outgoing ptcls from vrtx", 30, 0, 30,"No. outgoing ptcls from vrtx","Number of Events");
  vrtxZ = dqm.book1dHisto("VrtxZ", "VrtxZ", 50 , -250, 250,"Z_{Vtx}","Number of Events");
  vrtxRadius = dqm.book1dHisto("vrtxRadius", "vrtxRadius", 50, 0, 50,"R_{vtx}","Number of Events");
  //
  unknownPDTNumber = dqm.book1dHisto("unknownPDTNumber", "Log10(No. unknown ptcls PDT)", 60, -1, 5,"log_{10}(No. unknown ptcls PDT)","Number of Events"); //Log
  genPtclStatus = dqm.book1dHisto("genPtclStatus", "Status of genParticle", 200,0,200.,"","Number of Events");
  //
  Bjorken_x = dqm.book1dHisto("Bjorken_x", "Bjorken_x", 1000, 0.0, 1.0,"Bjorken_{x}","Number of Events");
  //
  status1ShortLived = dqm.book1dHisto("status1ShortLived","Status 1 short lived", 11, 0, 11,"","Number of Events");
  status1ShortLived->setBinLabel(1,"d/dbar");
  status1ShortLived->setBinLabel(2,"u/ubar");
  status1ShortLived->setBinLabel(3,"s/sbar");
  status1ShortLived->setBinLabel(4,"c/cbar");
  status1ShortLived->setBinLabel(5,"b/bbar");
  status1ShortLived->setBinLabel(6,"t/tbar");
  status1ShortLived->setBinLabel(7,"g");
  status1ShortLived->setBinLabel(8,"tau-/tau+");
  status1ShortLived->setBinLabel(9,"Z0");
  status1ShortLived->setBinLabel(10,"W-/W+");
  status1ShortLived->setBinLabel(11,"PDG = 7,8,17,25-99");

  log10DeltaEcms =dqm.book1dHisto("DeltaEcms1log10","log_{10} of deviation from nominal Ecms", 200,-1., 5.,"log_{10}(#DeltaE) (log_{10}(GeV))","Number of Events");  
  DeltaEcms = dqm.book1dHisto("DeltaEcms1","deviation from nominal Ecms", 200,-1., 1.,"#DeltaE (GeV)","Number of Events");
  DeltaPx = dqm.book1dHisto("DeltaPx1","deviation from nominal Px", 200,-1., 1.,"#DeltaP_{x} (GeV)","Number of Events");
  DeltaPy = dqm.book1dHisto("DeltaPy1","deviation from nominal Py", 200,-1., 1.,"#DeltaP_{y} (GeV)","Number of Events");
  DeltaPz = dqm.book1dHisto("DeltaPz1","deviation from nominal Pz", 200,-1., 1.,"#DeltaP_{z} (GeV)","Number of Events");
  return;
}

void BasicHepMCValidation::analyze(const edm::Event& iEvent,const edm::EventSetup& iSetup)
{ 
  ///counters to zero for every event
  int partonNum = 0;
  //
  int outVrtxStablePtclNum = 0; int stablePtclNum = 0; int otherPtclNum = 0; int unknownPDTNum = 0; int stableChaNum = 0;
  //
  double bjorken = 0.;
  //
  double etotal = 0. ; double pxtotal = 0.; double pytotal = 0.; double pztotal = 0.;

  ///Gathering the HepMCProduct information
  edm::Handle<HepMCProduct> evt;
  iEvent.getByToken(hepmcCollectionToken_, evt);

  //Get EVENT
  HepMC::GenEvent const *myGenEvent = evt->GetEvent();

  double weight = wmanager_.weight(iEvent);

  nEvt->Fill(0.5,weight);

  genPtclNumber->Fill(log10(myGenEvent->particles_size()),weight);     
  genVrtxNumber->Fill(log10(myGenEvent->vertices_size()),weight);

  ///Bjorken variable from PDF
  HepMC::PdfInfo const *pdf = myGenEvent->pdf_info();    
  if(pdf){
    bjorken = ((pdf->x1())/((pdf->x1())+(pdf->x2())));
  }
  Bjorken_x->Fill(bjorken,weight);
  //Looping through the VERTICES in the event
  HepMC::GenEvent::vertex_const_iterator vrtxBegin = myGenEvent->vertices_begin();
  HepMC::GenEvent::vertex_const_iterator vrtxEnd = myGenEvent->vertices_end();
  unsigned int nvtx(0);
  for(HepMC::GenEvent::vertex_const_iterator vrtxIt = vrtxBegin; vrtxIt!=vrtxEnd; ++vrtxIt)
    {
      ///Vertices
      HepMC::GenVertex const *vrtx = *vrtxIt;
      outVrtxPtclNumber->Fill(vrtx->particles_out_size(),weight); //std::cout << "all " << vrtx->particles_out_size() << '\n';

      if(nvtx==0){
	vrtxZ->Fill(vrtx->point3d().z(),weight);
	vrtxRadius->Fill(vrtx->point3d().perp(),weight);
      }
      ///loop on vertex particles
      HepMC::GenVertex::particles_out_const_iterator vrtxPtclBegin = vrtx->particles_out_const_begin();
      HepMC::GenVertex::particles_out_const_iterator vrtxPtclEnd = vrtx->particles_out_const_end();
      outVrtxStablePtclNum = 0;
      for(HepMC::GenVertex::particles_out_const_iterator vrtxPtclIt = vrtxPtclBegin; vrtxPtclIt != vrtxPtclEnd; ++vrtxPtclIt)
        {
          HepMC::GenParticle const *vrtxPtcl = *vrtxPtclIt;
          if (vrtxPtcl->status() == 1){
            ++outVrtxStablePtclNum; //std::cout << "stable " << outVrtxStablePtclNum << '\n';
          }
        }
      outVrtxStablePtclNumber->Fill(outVrtxStablePtclNum,weight);
      nvtx++;
    }//vertices

  ///Looping through the PARTICLES in the event
  HepMC::GenEvent::particle_const_iterator ptclBegin = myGenEvent->particles_begin();
  HepMC::GenEvent::particle_const_iterator ptclEnd = myGenEvent->particles_end();
  for(HepMC::GenEvent::particle_const_iterator ptclIt = ptclBegin; ptclIt!=ptclEnd; ++ptclIt)
    {
    
      ///Particles
      HepMC::GenParticle const *ptcl = *ptclIt;
      int Id = ptcl->pdg_id(); // std::cout << Id << '\n'; 
      float Log_p = log10( ptcl->momentum().rho() );
      double charge = 999.;	// for the charge it's needed a HepPDT method
      int status = ptcl->status();
      const HepPDT::ParticleData* PData = fPDGTable->particle(HepPDT::ParticleID(Id));
      if(PData==0) {
        //	    std::cout << "Unknown id = " << Id << '\n';
	    ++unknownPDTNum;
      }
      else
	    charge = PData->charge();

      ///Status statistics
      genPtclStatus->Fill((float)status,weight);

      ///Stable particles
      if(ptcl->status() == 1){
	    ++stablePtclNum;
	    stablePtclPhi->Fill(ptcl->momentum().phi()/CLHEP::degree,weight); //std::cout << ptcl->polarization().phi() << '\n';
	    stablePtclEta->Fill(ptcl->momentum().pseudoRapidity(),weight);
	    stablePtclCharge->Fill(charge,weight); // std::cout << ptclData.charge() << '\n';
	    stablePtclp->Fill(Log_p,weight);
	    stablePtclpT->Fill(log10(ptcl->momentum().perp()),weight);
        if (charge != 0. && charge != 999.) ++stableChaNum;
        if ( std::abs(Id) == 1 ) status1ShortLived->Fill(1,weight);
        if ( std::abs(Id) == 2 ) status1ShortLived->Fill(2,weight);
        if ( std::abs(Id) == 3 ) status1ShortLived->Fill(3,weight);
        if ( std::abs(Id) == 4 ) status1ShortLived->Fill(4,weight);
        if ( std::abs(Id) == 5 ) status1ShortLived->Fill(5,weight);
        if ( std::abs(Id) == 6 ) status1ShortLived->Fill(6,weight);
        if ( Id == 21 ) status1ShortLived->Fill(7,weight);
        if ( std::abs(Id) == 15 ) status1ShortLived->Fill(8,weight);
        if ( Id == 23 ) status1ShortLived->Fill(9,weight);
        if ( std::abs(Id) == 24 ) status1ShortLived->Fill(10,weight);
        if ( std::abs(Id) == 7 || std::abs(Id) == 8 || std::abs(Id) == 17 || (std::abs(Id) >= 25 && std::abs(Id) <= 99) ) status1ShortLived->Fill(11,weight);
        etotal += ptcl->momentum().e(); 
        pxtotal += ptcl->momentum().px(); 
        pytotal += ptcl->momentum().py(); 
        pztotal += ptcl->momentum().pz(); 
      }

      if (abs(Id) < 6 || abs(Id) == 22){
        ++partonNum; partonpT->Fill(Log_p,weight);
      }
      
      bool indentified=false;
      for(unsigned int i=0;i<particles.size();i++){if(particles.at(i).Fill(ptcl,weight)){indentified=true; break;}}
      if(!indentified){	++otherPtclNum; otherPtclMomentum->Fill(Log_p,weight);}
    }//event particles

  // set a default sqrt(s) and then check in the event
  double ecms = 13000.;
  if ( myGenEvent->valid_beam_particles() ) {
    ecms = myGenEvent->beam_particles().first->momentum().e()+myGenEvent->beam_particles().second->momentum().e();
  }
  log10DeltaEcms->Fill(log10(fabs(etotal-ecms)),weight);
  DeltaEcms->Fill(etotal-ecms,weight);
  DeltaPx->Fill(pxtotal,weight);
  DeltaPy->Fill(pytotal,weight);
  DeltaPz->Fill(pztotal,weight);

 
  ///filling multiplicity ME's
  stablePtclNumber->Fill(log10(stablePtclNum+0.1),weight); 
  stableChaNumber->Fill(log10(stableChaNum+0.1),weight); 
  otherPtclNumber->Fill(log10(otherPtclNum+0.1),weight);
  unknownPDTNumber->Fill(log10(unknownPDTNum+0.1),weight);
  //
  partonNumber->Fill(partonNum,weight);
  for(unsigned int i=0;i<particles.size();i++){particles.at(i).FillCount(weight);};

}//analyze
