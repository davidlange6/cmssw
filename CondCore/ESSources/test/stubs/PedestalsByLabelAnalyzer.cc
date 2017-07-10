/*----------------------------------------------------------------------

Toy EDProducers and EDProducts for testing purposes only.

----------------------------------------------------------------------*/

#include <stdexcept>
#include <string>
#include <iostream>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EventSetup.h"

#include "CondFormats/Calibration/interface/Pedestals.h"
#include "CondFormats/DataRecord/interface/PedestalsRcd.h"

using namespace std;

namespace edmtest
{
  class PedestalsByLabelAnalyzer : public edm::EDAnalyzer
  {
  public:
    explicit  PedestalsByLabelAnalyzer(edm::ParameterSet const& p) 
    { 
      std::cout<<"PedestalsByLabelAnalyzer"<<std::endl;
    }
    explicit  PedestalsByLabelAnalyzer(int i) 
    { std::cout<<"PedestalsByLabelAnalyzer "<<i<<std::endl; }
    virtual ~PedestalsByLabelAnalyzer() {  
      std::cout<<"~PedestalsByLabelAnalyzer "<<std::endl;
    }
    virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
  private:
  };
  
  void
   PedestalsByLabelAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& context){
    using namespace edm::eventsetup;
    // Context is not used.
    std::cout <<" I AM IN RUN NUMBER "<<e.id().run() <<std::endl;
    std::cout <<" ---EVENT NUMBER "<<e.id().event() <<std::endl;
    edm::eventsetup::EventSetupRecordKey recordKey(edm::eventsetup::EventSetupRecordKey::TypeTag::findType("PedestalsRcd"));
    if( recordKey.type() == edm::eventsetup::EventSetupRecordKey::TypeTag()) {
      //record not found
      std::cout <<"Record \"PedestalsRcd"<<"\" does not exist "<<std::endl;
    }
    edm::ESHandle<Pedestals> pPeds;
    std::cout<<"got eshandle"<<std::endl;
    context.get<PedestalsRcd>().get("lab3d",pPeds);
    std::cout<<"got context"<<std::endl;
    const Pedestals* myped=pPeds.product();
    std::cout<<"Pedestals* "<<myped<<std::endl;
    for(const auto & m_pedestal : myped->m_pedestals)
      std::cout << " mean: " <<m_pedestal.m_mean
                << " variance: " <<m_pedestal.m_variance;
    std::cout  << std::endl;
    
  }
  DEFINE_FWK_MODULE(PedestalsByLabelAnalyzer);
}


