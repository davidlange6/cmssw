#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/PyBind11ParameterSet/interface/PyBind11ProcessDesc.h"
#include "FWCore/PyBind11ParameterSet/src/initializePyBind11Module.h"
#include "FWCore/PyBind11ParameterSet/src/PyBind11Wrapper.h"
#include <pybind11/embed.h>

#include <sstream>
#include <iostream>

PyBind11ProcessDesc::PyBind11ProcessDesc() :
   theProcessPSet(),
   theMainModule(),
   theMainNamespace() {
}

PyBind11ProcessDesc::PyBind11ProcessDesc(std::string const& config) :
   theProcessPSet(),
   theMainModule(),
   theMainNamespace() {
  prepareToRead();
  read(config);
  Py_Finalize();
}

PyBind11ProcessDesc::PyBind11ProcessDesc(std::string const& config, int argc, char* argv[]) :
   theProcessPSet(),
   theMainModule(),
   theMainNamespace() {
  prepareToRead();
  PySys_SetArgv(argc, argv);
  read(config);
  Py_Finalize();
}

void PyBind11ProcessDesc::prepareToRead() {
  edm::python::initializePyBind11Module();
  theMainModule = pybind11::module::import("__main__");
  //theMainNamespace = theMainModule.attr("__dict__");
  //std::cout << "got the dict\n";

  theMainModule.attr("processDesc")=this; 
  std::cout << "bound the desc\n";

  theMainModule.attr("processPSet")=&theProcessPSet; //pybind11::cast(&theProcessPSet);
  std::cout << "bound the pset\n";
}

void PyBind11ProcessDesc::read(std::string const& config) {
  try {
    // if it ends with py, it's a file
    if(config.substr(config.size()-3) == ".py") {
      readFile(config);
    } else {
      readString(config);
    }
  }
  catch(pybind11::error_already_set const&) {
     edm::pythonToCppException("Configuration");
     Py_Finalize();
  }
}

void PyBind11ProcessDesc::readFile(std::string const& fileName) {
  std::string initCommand("import FWCore.ParameterSet.Config as cms\n"
                          "execfile('");
  initCommand += fileName + "')";

  pybind11::exec(initCommand.c_str());
  std::string command("process.fillProcessDesc(processPSet)");
  pybind11::exec(command.c_str());
}

void PyBind11ProcessDesc::readString(std::string const& pyConfig) {
  std::string command = pyConfig;
  command += "\nprocess.fillProcessDesc(processPSet)";
  std::cout << "command is\n";
  std::cout << command << std::endl;
  pybind11::exec(command.c_str());
}

std::shared_ptr<edm::ParameterSet> PyBind11ProcessDesc::parameterSet() const {
  return std::make_shared<edm::ParameterSet>(theProcessPSet.pset());
}

std::string PyBind11ProcessDesc::dump() const {
  std::ostringstream os;
  os << theProcessPSet.dump();
  return os.str();
}

// For backward compatibility only.  Remove when no longer used.
std::shared_ptr<edm::ProcessDesc> PyBind11ProcessDesc::processDesc() const {
  return std::make_shared<edm::ProcessDesc>(parameterSet());
}
