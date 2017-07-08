#ifndef DPGAnalysis_SiStripTools_DigiVtxPosCorrHistogramMaker_H
#define DPGAnalysis_SiStripTools_DigiVtxPosCorrHistogramMaker_H

#include <string>
#include <map>
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"


namespace edm {
  class ParameterSet;
  class Event;
}
class TH2F;
class TProfile;
class TFileDirectory;

class DigiVtxPosCorrHistogramMaker {

 public:
  DigiVtxPosCorrHistogramMaker(edm::ConsumesCollector&& iC);
  DigiVtxPosCorrHistogramMaker(const edm::ParameterSet& iConfig, edm::ConsumesCollector&& iC);

  ~DigiVtxPosCorrHistogramMaker();

  void book(const std::string& dirname, const std::map<unsigned int, std::string>& labels);
  void book(const std::string& dirname);
  void beginRun(const unsigned int nrun);
  void fill(const edm::Event& iEvent, const std::map<unsigned int,int>& ndigi);

 private:

  edm::EDGetTokenT<edm::HepMCProduct> m_mcvtxcollectionToken;
  std::string m_hitname;
  const int m_nbins;
  const int m_scalefact;
  std::map<unsigned int,int> m_binmax;
  std::map<unsigned int, std::string> m_labels;

  std::map<unsigned int,TH2F*> m_nmultvsvtxpos;
  std::map<unsigned int,TProfile*> m_nmultvsvtxposprof;
  std::map<unsigned int,TFileDirectory*> m_subdirs;

};


#endif //  DPGAnalysis_SiStripTools_DigiVtxPosCorrHistogramMaker_H
