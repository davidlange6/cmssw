#include "FWCore/PyBind11ParameterSet/interface/MakePyBind11ParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/PyBind11ParameterSet/interface/Python11ParameterSet.h"
#include "FWCore/PyBind11ParameterSet/interface/PyBind11ProcessDesc.h"
#include "initializePyBind11Module.h"
#include <pybind11/embed.h>

static
void
makePSetsFromFile(std::string const& fileName) { //, pybind11::object& mainNamespace) {
  std::string initCommand("from FWCore.ParameterSet.Types import makeCppPSet\n"
                          "execfile('");
  initCommand += fileName + "')";

  pybind11::exec(initCommand);
  pybind11::exec("makeCppPSet(locals(), topPSet)");
}

static
void
makePSetsFromString(std::string const& module) { //, pybind11::object& mainNamespace) {
  std::string command = module;
  command += "\nfrom FWCore.ParameterSet.Types import makeCppPSet\nmakeCppPSet(locals(), topPSet)";
  pybind11::exec(command);
}

namespace edm {

  std::shared_ptr<ParameterSet>
  readConfig(std::string const& config) {
    PyBind11ProcessDesc pythonProcessDesc(config);
    return pythonProcessDesc.parameterSet();
  }

  std::shared_ptr<ParameterSet>
  readConfig(std::string const& config, int argc, char* argv[]) {
    PyBind11ProcessDesc pythonProcessDesc(config, argc, argv);
    return pythonProcessDesc.parameterSet();
  }

  void
  makeParameterSets(std::string const& configtext,
                  std::shared_ptr<ParameterSet>& main) {
    PyBind11ProcessDesc pythonProcessDesc(configtext);
    main = pythonProcessDesc.parameterSet();
  }

  std::shared_ptr<ParameterSet>
  readPSetsFrom(std::string const& module) {
    pybind11::scoped_interpreter guard{};
    python::initializePyBind11Module();

    pybind11::object mainModule = pybind11::module::import("__main__");
    //pybind11::object mainNamespace = mainModule.attr("__dict__");
    Python11ParameterSet theProcessPSet;
    mainModule.attr("topPSet") = pybind11::cast(&theProcessPSet);

    try {
      // if it ends with py, it's a file
      if(module.substr(module.size()-3) == ".py") {
        makePSetsFromFile(module);//,mainNamespace);
      } else {
        makePSetsFromString(module);//,mainNamespace);
      }
    }
    catch( pybind11::error_already_set const& ) {
      pythonToCppException("Configuration");
      Py_Finalize();
    }
    auto returnValue = std::make_shared<ParameterSet>();
    theProcessPSet.pset().swap(*returnValue);
    return returnValue;
  }
} // namespace edm
