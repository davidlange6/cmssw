#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/PyBind11ParameterSet/interface/PyBind11ProcessDesc.h"
#include "FWCore/PyBind11ParameterSet/src/initializePyBind11Module.h"
#include "FWCore/PyBind11ParameterSet/src/PyBind11Wrapper.h"

#include <sstream>

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

  theMainNamespace = theMainModule.attr("__dict__");
  theMainNamespace.attr("processDesc") = pybind11::cast(this);
  theMainNamespace.attr("processPSet") = pybind11::cast(&theProcessPSet);
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

  pybind11::handle(PyRun_String(initCommand.c_str(),
				Py_file_input,
				theMainNamespace.ptr(),
				theMainNamespace.ptr()));
  std::string command("process.fillProcessDesc(processPSet)");
  pybind11::handle(PyRun_String(command.c_str(),
				Py_eval_input,
				theMainNamespace.ptr(),
				theMainNamespace.ptr()));
}

void PyBind11ProcessDesc::readString(std::string const& pyConfig) {
  std::string command = pyConfig;
  command += "\nprocess.fillProcessDesc(processPSet)";
  pybind11::handle(PyRun_String(command.c_str(),
				Py_file_input,
				theMainNamespace.ptr(),
				theMainNamespace.ptr()));
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
