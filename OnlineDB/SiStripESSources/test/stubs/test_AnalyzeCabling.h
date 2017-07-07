
#ifndef OnlineDB_SiStripESSources_test_AnalyzeCabling_H
#define OnlineDB_SiStripESSources_test_AnalyzeCabling_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/**
   @class test_AnalyzeCabling 
   @brief Analyzes FEC (and FED) cabling object(s)
*/
class test_AnalyzeCabling : public edm::EDAnalyzer {

 public:
  
  test_AnalyzeCabling( const edm::ParameterSet& ) {;}
  ~test_AnalyzeCabling() override {;}
  
  void beginRun( const edm::Run&, const edm::EventSetup& ) override;
  void analyze( const edm::Event&, const edm::EventSetup& ) override {;}
  
};

#endif // OnlineDB_SiStripESSources_test_AnalyzeCabling_H
