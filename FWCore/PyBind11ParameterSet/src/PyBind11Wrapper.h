#ifndef FWCore_PyBind11ParameterSet_PyBind11Wrapper_h
#define FWCore_PyBind11ParameterSet_PyBind11Wrapper_h

#include <vector>
#include <string>
#include <pybind11/pybind11.h>

namespace edm {
void
pythonToCppException(const std::string& iType);

//  boost::python::list toPythonList(const std::vector<std::string> & v);
  // utility to translate from an STL vector of strings to
  // a Python list

  template<typename T>
    pybind11::list toPython11List(const std::vector<T> & v) {
    pybind11::list result;
    for(unsigned i = 0; i < v.size(); ++i) {
       result.append(v[i]);
    }
    return result;
  }

  // and back.  Destroys the input via pop()s - well probably not
  template<typename T>
  std::vector<T> toVector(pybind11::list & l)
  {
    std::vector<T> result;
    for(unsigned i = 0; i < l.size(); ++i)
    {
      result.push_back(l[i].cast<T>());
    }
    return result;
  }
}

#endif
