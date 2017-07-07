#ifndef DTTTrigAnalyzer_H
#define DTTTrigAnalyzer_H

/** \class DTTTrigAnalyzer
 *  Plot the ttrig from the DB
 *
 *  \author S. Bolognesi - INFN Torino
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/MuonDetId/interface/DTWireId.h"

#include <string>
#include <fstream>
#include <map>
#include <vector>

class DTTtrig;
class TFile;
class TH1D;

class DTTTrigAnalyzer : public edm::EDAnalyzer {
public:
  /// Constructor
  DTTTrigAnalyzer(const edm::ParameterSet& pset);

  /// Destructor
  ~DTTTrigAnalyzer() override;

  /// Operations
  //Read the DTGeometry and teh t0 DB
  void beginRun(const edm::Run&,const edm::EventSetup& setup) override;
  void analyze(const edm::Event& event, const edm::EventSetup& setup) override {}
  //Do the real work
  void endJob() override;

protected:

private:
  std::string getHistoName(const DTWireId& lId) const;
  std::string getDistribName(const DTWireId& wId) const;

  // The file which will contain the histos
  TFile *theFile;

  //The t0 map
  const DTTtrig *tTrigMap;

  std::string dbLabel;
 
  //The k factor
  //double kfactor;
  
  // Map of the ttrig, tmean, sigma histos by wheel/sector/SL
  std::map<std::pair<int,int>, TH1D*> theTTrigHistoMap;
  std::map<std::pair<int,int>, TH1D*> theTMeanHistoMap;
  std::map<std::pair<int,int>, TH1D*> theSigmaHistoMap;
  std::map<std::pair<int,int>, TH1D*> theKFactorHistoMap;
 // Map of the ttrig, tmean, sigma distributions by wheel/station/SL
  std::map<std::vector<int>, TH1D*> theTTrigDistribMap;
  std::map<std::vector<int>, TH1D*> theTMeanDistribMap;
  std::map<std::vector<int>, TH1D*> theSigmaDistribMap;
  std::map<std::vector<int>, TH1D*> theKFactorDistribMap;

};
#endif

