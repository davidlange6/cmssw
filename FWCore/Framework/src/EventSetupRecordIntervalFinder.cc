// -*- C++ -*-
//
// Package:     Framework
// Class  :     EventSetupRecordIntervalFinder
// 
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris Jones
// Created:     Wed Mar 30 14:27:26 EST 2005
//

// system include files

// user include files
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include <cassert>

//
// constants, enums and typedefs
//
using namespace edm::eventsetup;
namespace edm {
//
// static data member definitions
//

//
// constructors and destructor
//
//EventSetupRecordIntervalFinder::EventSetupRecordIntervalFinder()
//{
//}

// EventSetupRecordIntervalFinder::EventSetupRecordIntervalFinder(const EventSetupRecordIntervalFinder& rhs)
// {
//    // do actual copying here;
// }

EventSetupRecordIntervalFinder::~EventSetupRecordIntervalFinder() noexcept(false)
{
}

//
// assignment operators
//
// const EventSetupRecordIntervalFinder& EventSetupRecordIntervalFinder::operator=(const EventSetupRecordIntervalFinder& rhs)
// {
//   //An exception safe implementation is
//   EventSetupRecordIntervalFinder temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
const ValidityInterval& 
EventSetupRecordIntervalFinder::findIntervalFor(const EventSetupRecordKey& iKey,
                                              const IOVSyncValue& iInstance)
{
   Intervals::iterator itFound = intervals_.find(iKey);
   assert(itFound != intervals_.end()) ;
   if(! itFound->second.validFor(iInstance)) {
      setIntervalFor(iKey, iInstance, itFound->second);
   }
   return itFound->second;
}

void 
EventSetupRecordIntervalFinder::findingRecordWithKey(const EventSetupRecordKey& iKey) {
   intervals_.insert(Intervals::value_type(iKey, ValidityInterval()));
}

void 
EventSetupRecordIntervalFinder::delaySettingRecords()
{
}

//
// const member functions
//
std::set<EventSetupRecordKey> 
EventSetupRecordIntervalFinder::findingForRecords() const
{
   if(intervals_.empty()) {
      //we are delaying our reading
      const_cast<EventSetupRecordIntervalFinder*>(this)->delaySettingRecords();
   }
   
   std::set<EventSetupRecordKey> returnValue;
   
   for(const auto & interval : intervals_) {
      returnValue.insert(returnValue.end(), interval.first);
   }
   return returnValue;
}

//
// static member functions
//
}
