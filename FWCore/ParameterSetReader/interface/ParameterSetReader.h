#ifndef FWCore_Framework_ParameterSetReader_h
#define FWCore_Framework_ParameterSetReader_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace edm {

  std::shared_ptr<edm::ParameterSet> getPSetFromConfig(const std::string &config);

//its really the stuff in MakePythonParameterSets that should be in the different namespace
//I'll do that if this setup is ok
  namespace def {
    std::shared_ptr<edm::ParameterSet> readConfig(std::string const& config, int argc, char* argv[]);
  };

};
#endif
