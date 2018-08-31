#include "FWCore/ParameterSetReader/interface/ParameterSetReader.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

std::shared_ptr<edm::ParameterSet> edm::getPSetFromConfig(const std::string &config) {
  return PythonProcessDesc(config).parameterSet();
}


//its really the stuff in MakePythonParameterSets that should be in the different namespace
//I'll do that if this setup is ok
std::shared_ptr<edm::ParameterSet> edm::def::readConfig(std::string const& config, int argc, char* argv[]) {
  return edm::readConfig(config,argc,argv);
}
