#ifndef FWCore_MessageService_test_UnitTestClient_SLumi_h
#define FWCore_MessageService_test_UnitTestClient_SLumi_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace edm {
  class ParameterSet;
}


namespace edmtest
{

class UTC_SL1
  : public edm::EDAnalyzer
{
public:
  explicit
    UTC_SL1( edm::ParameterSet const & p)
  { 
    identifier = p.getUntrackedParameter<int> ("identifier", 99);
    edm::GroupLogStatistics("grouped_cat");  
  }

  
    ~UTC_SL1() override
  { }

  
    void analyze( edm::Event      const & e
                , edm::EventSetup const & c
                ) override;

private:
  int identifier;
  static bool enableNotYetCalled;
  static int n;
};

class UTC_SL2
  : public edm::EDAnalyzer
{
public:
  explicit
    UTC_SL2( edm::ParameterSet const & p)
  { 
    identifier = p.getUntrackedParameter<int> ("identifier", 98);
  }

  
    ~UTC_SL2() override
  { }

  
    void analyze( edm::Event      const & e
                , edm::EventSetup const & c
                ) override;

private:
  int identifier;
  static int n;
};

class UTC_SLUMMARY
  : public edm::EDAnalyzer
{
public:
  explicit
    UTC_SLUMMARY( edm::ParameterSet const &)
  { 
  }

  
    ~UTC_SLUMMARY() override
  { }

  
    void analyze( edm::Event      const & e
                , edm::EventSetup const & c
                ) override;

  
    void endLuminosityBlock ( edm::LuminosityBlock const & lb
                	    , edm::EventSetup 	   const & c
                	    ) override;

private:
};


}  // namespace edmtest


#endif  // FWCore_MessageService_test_UnitTestClient_SLumi_h
