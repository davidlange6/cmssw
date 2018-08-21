// -*- C++ -*-
//
// Package:     PyBind11ParameterSet
// Class  :     initializePyBind11Module
// 

// system include files

// user include files
#include "FWCore/PyBind11ParameterSet/src/initializePyBind11Module.h"
#include "PyBind11Module.h"


//
// constants, enums and typedefs
//
static bool s_initialized = false;

namespace edm {
   namespace python {
      void initializePyBind11Module() {
         char *libFWCoreParameterSet = const_cast<char *>("libFWCorePyBind11ParameterSet");
         PyImport_AppendInittab(libFWCoreParameterSet, &initlibFWCorePyBind11ParameterSet );
         Py_Initialize();
         if(!s_initialized)
         {
            PyImport_ImportModule(libFWCoreParameterSet);
            s_initialized = true;
         }
         
      }
   }
}
