#ifndef Framework_DepOn2Record_h
#define Framework_DepOn2Record_h
// -*- C++ -*-
//
// Package:     Framework
// Class  :     DepOn2Record
//
/**\class DepOn2Record DepOn2Record.h FWCore/Framework/test/DepOn2Record.h

 Description: A test Record that is dependent on DummyRecord and Dummy2Record

 Usage:
    <usage>

*/
//
// Original Author:  Chris Jones
//         Created:  Wed Jun 22 19:46:32 EDT 2005
//

// system include files
#include "boost/mpl/vector.hpp"

// user include files

// forward declarations
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "FWCore/Framework/test/Dummy2Record.h"
#include "FWCore/Framework/test/DummyRecord.h"

class DepOn2Record
    : public edm::eventsetup::DependentRecordImplementation<
          DepOn2Record, boost::mpl::vector<DummyRecord, Dummy2Record> > {};

#endif
