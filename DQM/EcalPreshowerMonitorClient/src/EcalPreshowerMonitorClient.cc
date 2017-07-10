#include "DQM/EcalPreshowerMonitorClient/interface/EcalPreshowerMonitorClient.h"

#include "DQM/EcalPreshowerMonitorClient/interface/ESClient.h"
#include "DQM/EcalPreshowerMonitorClient/interface/ESPedestalClient.h"
#include "DQM/EcalPreshowerMonitorClient/interface/ESIntegrityClient.h"
#include "DQM/EcalPreshowerMonitorClient/interface/ESSummaryClient.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include <memory>
#include <iostream>
#include <string>
#include <vector>

EcalPreshowerMonitorClient::EcalPreshowerMonitorClient(const edm::ParameterSet& ps) :
  debug_(ps.getUntrackedParameter<bool>("debug")),
  verbose_(ps.getUntrackedParameter<bool>("verbose")),
  clients_()
{
   std::vector<std::string> enabledClients(ps.getUntrackedParameter<std::vector<std::string> >("enabledClients"));

   if ( verbose_ ) {
      std::cout << " Enabled Clients:";
      for (const auto & enabledClient : enabledClients) {
	 std::cout << " " << enabledClient;
      }
      std::cout << std::endl;
   }

   //Setup Clients
   if ( find(enabledClients.begin(), enabledClients.end(), "Integrity" ) != enabledClients.end() ){
      clients_.push_back( new ESIntegrityClient(ps) );
   }

   if ( find(enabledClients.begin(), enabledClients.end(), "Pedestal" ) != enabledClients.end() ){
      clients_.push_back( new ESPedestalClient(ps) );
   }

   if ( find(enabledClients.begin(), enabledClients.end(), "Summary" ) != enabledClients.end() ){
      clients_.push_back( new ESSummaryClient(ps) );
   }

}

EcalPreshowerMonitorClient::~EcalPreshowerMonitorClient() {

   if ( verbose_ ) std::cout << "Finish EcalPreshowerMonitorClient" << std::endl;

   for (auto & client : clients_) {
      delete client;
   }

}

/*static*/
void
EcalPreshowerMonitorClient::fillDescriptions(edm::ConfigurationDescriptions& _descs)
{
  edm::ParameterSetDescription desc;

  std::vector<std::string> clientsDefault;
  clientsDefault.push_back("Integrity");
  clientsDefault.push_back("Pedestal");
  clientsDefault.push_back("Summary");
  desc.addUntracked<std::vector<std::string> >("enabledClients", clientsDefault);
  desc.addUntracked<edm::FileInPath>("LookupTable");
  desc.addUntracked<std::string>("prefixME", "EcalPreshower");
  desc.addUntracked<bool>("fitPedestal", true);
  desc.addUntracked<bool>("cloneME", true);
  desc.addUntracked<bool>("verbose", false);
  desc.addUntracked<bool>("debug", false);

  _descs.addDefault(desc);
}

void EcalPreshowerMonitorClient::dqmEndJob(DQMStore::IBooker& _ibooker, DQMStore::IGetter& _igetter) {

   if (debug_) { 
      std::cout << "EcalPreshowerMonitorClient: endJob" << std::endl;
   }
   
   for (auto & client : clients_) {
     client->setup(_ibooker);
     client->endJobAnalyze(_igetter);
   }

}

void EcalPreshowerMonitorClient::dqmEndLuminosityBlock(DQMStore::IBooker& _ibooker, DQMStore::IGetter& _igetter, const edm::LuminosityBlock &, const edm::EventSetup &) {

  for (auto & client : clients_) {
    client->setup(_ibooker);
    client->endLumiAnalyze(_igetter);
  }
   
}

//define this as a plug-in
DEFINE_FWK_MODULE(EcalPreshowerMonitorClient);
