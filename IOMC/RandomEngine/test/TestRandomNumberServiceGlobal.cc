// -*- C++ -*-
//
// Package:    IOMC/RandomEngine
// Class:      TestRandomNumberServiceGlobal
//
/**\class TestRandomNumberServiceGlobal

Description: Used in tests of the RandomNumberGeneratorService.

Implementation:

This module should not be used with the multiprocess mode
that forks processes.

Dumps to std::cout some initialization information from
the RandomNumberGeneratorService so that this output can
be compared to reference files.

This module will create it own random engines and generate
the random numbers it expects the service to produce
and do a comparison with the numbers the engines from
the service actually generate. This does not work to test
multistream replay jobs because we do not know which
streams will contain which events so this check is not done
in those cases.

Creates a set of text file with names like the following:
testRandomService_0_t1.txt where the first number is the stream
and the second number is the module name. It contains the
configured first seed and some random numbers generated by the
module on each event.

For jobs with more than one stream, it writes a file named
testRandomServiceL1E1 or replaytestRandomServiceL1E1.txt where
the numbers in the name change to reflect the lumi and event
numbers. One file is written per event. Note the order each
modules data appears can vary if modules are run concurrently.
This is used to test the replay mode where the engine states
are written into the event and luminosity block for multistream
jobs.

Always writes random numbers generated in the last event in
each module to a file named stream0LastEvent.txt or
replaystream0LastEvent.txt. The filename depends on whether
the "replayMultiStream" configuration parameter has been set. Note
that the order the module data is written can vary
if modules can be run in parallel. This is used in the
test of replay of an event in a multistream job with
the text file containing the states.

*/
//
// Original Author:  Chris Jones, David Dagenhart
//         Created:  Tue Mar  7 11:57:09 EST 2006
//

#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "DataFormats/Provenance/interface/LuminosityBlockAuxiliary.h"
#include "DataFormats/Provenance/interface/ModuleDescription.h"
#include "FWCore/Framework/interface/global/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/propagate_const.h"
#include "IOMC/RandomEngine/src/TRandomAdaptor.h"

#include "CLHEP/Random/RandExponential.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/engineIDulong.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RanecuEngine.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <unistd.h>

namespace {
  std::mutex write_mutex;
}

class TestRandomNumberServiceStreamCache {
public:
  TestRandomNumberServiceStreamCache() :
    serviceEngine_(nullptr),
    countEvents_(0) { }
  edm::propagate_const<CLHEP::HepRandomEngine*> serviceEngine_;
  unsigned int countEvents_;
  std::ofstream outFile_;
  std::string lastEventRandomNumbers_;

  edm::propagate_const<std::shared_ptr<CLHEP::HepRandomEngine>> referenceEngine_;
  std::vector<double> referenceRandomNumbers_;
};

class TestRandomNumberServiceLumiCache {
public:
  TestRandomNumberServiceLumiCache() { }
  edm::propagate_const<std::shared_ptr<CLHEP::HepRandomEngine>> referenceEngine_;
  std::vector<double> referenceRandomNumbers_;
};

class TestRandomNumberServiceGlobal : public edm::global::EDAnalyzer<edm::StreamCache<TestRandomNumberServiceStreamCache>,
                                                                     edm::LuminosityBlockCache<TestRandomNumberServiceLumiCache> > {
public:
  explicit TestRandomNumberServiceGlobal(edm::ParameterSet const&);
  ~TestRandomNumberServiceGlobal() override;

  void analyze(edm::StreamID, edm::Event const&, edm::EventSetup const&) const override;
  void beginJob() override;
  void endJob() override;

  std::shared_ptr<TestRandomNumberServiceLumiCache> globalBeginLuminosityBlock(edm::LuminosityBlock const&,
                                                                                       edm::EventSetup const&) const override;

  void globalEndLuminosityBlock(edm::LuminosityBlock const&,
                                        edm::EventSetup const&) const override { }

  std::unique_ptr<TestRandomNumberServiceStreamCache> beginStream(edm::StreamID) const override;
  void endStream(edm::StreamID) const override;

  void streamBeginRun(edm::StreamID, edm::Run const&, edm::EventSetup const&) const override;
  void streamEndRun(edm::StreamID, edm::Run const&, edm::EventSetup const&) const override;

  void streamBeginLuminosityBlock(edm::StreamID, edm::LuminosityBlock const&, edm::EventSetup const&) const override;
  void streamEndLuminosityBlock(edm::StreamID, edm::LuminosityBlock const&, edm::EventSetup const&) const override;

private:


  std::string engineName_;
  std::vector<unsigned int> seeds_;
  unsigned int offset_;
  unsigned int maxEvents_;
  unsigned int nStreams_;
  std::vector<unsigned int> skippedEvents_;

  // If size larger than 1, the seed for each lumi engine is the
  // entry in this vector, where the lumi number is the index into
  // the vector. Only needed in replay.
  std::vector<unsigned int> seedByLumi_;

  bool multiStreamReplay_;

  // Dump is only intended to be set true for single stream jobs and
  // only for one module. Otherwise there will be issues with
  // interleaved output.
  bool dump_;
};

TestRandomNumberServiceGlobal::TestRandomNumberServiceGlobal(edm::ParameterSet const& pset) :
  engineName_(pset.getUntrackedParameter<std::string>("engineName")),
  seeds_(pset.getUntrackedParameter<std::vector<unsigned int> >("seeds")),
  offset_(pset.getUntrackedParameter<unsigned int>("offset")),
  maxEvents_(pset.getUntrackedParameter<unsigned int>("maxEvents")),
  nStreams_(pset.getUntrackedParameter<unsigned int>("nStreams")),
  skippedEvents_(pset.getUntrackedParameter<std::vector<unsigned int> >("skippedEvents", std::vector<unsigned int>(1, 0))),
  seedByLumi_(pset.getUntrackedParameter<std::vector<unsigned int> >("seedByLumi", std::vector<unsigned int>(1, 0))),
  multiStreamReplay_(pset.getUntrackedParameter<bool>("multiStreamReplay", false)),
  dump_(pset.getUntrackedParameter<bool>("dump", false)) {

  if(dump_) {
    edm::Service<edm::RandomNumberGenerator> rng;
    bool exceptionThrown = true;
    try {
       unsigned int mySeed = rng->mySeed();
       std::cout << "*** TestRandomNumberServiceGlobal constructor " << mySeed << "\n";
       exceptionThrown = false;
    } catch( cms::Exception const&) {
    }
    if(not exceptionThrown) {
       throw cms::Exception("FailedToThrow")<<"RandomNunberGenerator::mySeed did not throw";
    }
  }
}

TestRandomNumberServiceGlobal::~TestRandomNumberServiceGlobal() {
}

void
TestRandomNumberServiceGlobal::analyze(edm::StreamID streamID, edm::Event const& event, edm::EventSetup const&) const {

  // Add some sleep to encourage all the streams to get events to process.
  if(nStreams_ > 1) {
    usleep(25000);
  }

  if(dump_) {
    edm::Service<edm::RandomNumberGenerator> rng;
    std::cout << "*** TestRandomNumberServiceGlobal analyze " << rng->mySeed() << "\n";
    std::cout << rng->getEngine(streamID).name() << "\n";
  }

  TestRandomNumberServiceStreamCache* cache = streamCache(streamID);

  assert(cache->countEvents_ < maxEvents_);

  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine& engine = rng->getEngine(streamID);

  if(cache->serviceEngine_ != &engine){
    throw cms::Exception("TestRandomNumberService")
      << "TestRandomNumberServiceGlobal::analyze: Engines from event and beginStream are not the same";
  }

  // Generate some random numbers for tests purposes
  double randomNumberEvent0_ = engine.flat();
  double randomNumberEvent1_ = engine.flat();
  double randomNumberEvent2_ = engine.flat();

  // Generate one using a distribution to exercise that possibility also
  CLHEP::RandExponential expDist(engine);
  double mean = 10.0;  // Mean of the exponential
  double randomNumberEvent3_ = expDist.fire(mean);

  // Write them to a text file
  cache->outFile_ << rng->mySeed() << "\n";
  cache->outFile_ << randomNumberEvent0_ << "\n";
  cache->outFile_ << randomNumberEvent1_ << "\n";
  cache->outFile_ << randomNumberEvent2_ << "\n";
  cache->outFile_ << randomNumberEvent3_ << "\n";

  if(!multiStreamReplay_) {
    // Compare with the reference numbers when not skipping events at the beginning
    if(skippedEvents_.size() == 1 && skippedEvents_[0] == 0) {
      if(randomNumberEvent0_ != cache->referenceRandomNumbers_.at(0 + 4 * cache->countEvents_) ||
         randomNumberEvent1_ != cache->referenceRandomNumbers_.at(1 + 4 * cache->countEvents_) ||
         randomNumberEvent2_ != cache->referenceRandomNumbers_.at(2 + 4 * cache->countEvents_) ||
         randomNumberEvent3_ != cache->referenceRandomNumbers_.at(3 + 4 * cache->countEvents_)) {
        throw cms::Exception("TestRandomNumberService")
          << "TestRandomNumberServiceGlobal::analyze: Random sequence does not match expected sequence";
      }
    // Now compare when skipping events
    } else if(cache->countEvents_ < skippedEvents_.size()) {
      if(randomNumberEvent0_ != cache->referenceRandomNumbers_.at(0 + 4 * (cache->countEvents_ + skippedEvents_.at(cache->countEvents_))) ||
       randomNumberEvent1_ != cache->referenceRandomNumbers_.at(1 + 4 * (cache->countEvents_ + skippedEvents_.at(cache->countEvents_))) ||
       randomNumberEvent2_ != cache->referenceRandomNumbers_.at(2 + 4 * (cache->countEvents_ + skippedEvents_.at(cache->countEvents_))) ||
       randomNumberEvent3_ != cache->referenceRandomNumbers_.at(3 + 4 * (cache->countEvents_ + skippedEvents_.at(cache->countEvents_)))) {
        throw cms::Exception("TestRandomNumberService")
          << "TestRandomNumberServiceGlobal::analyze: Random sequence does not match expected sequence";
      }
    }
  }

  // Save the random numbers for the last event
  // This string gets overwritten on each event
  // and printed at endStream
  std::ostringstream ss;
  ss << moduleDescription().moduleLabel() << " ";
  ss << engine.name() << " ";
  ss << "Last event random numbers ";
  ss << randomNumberEvent0_ << " ";
  ss << randomNumberEvent1_ << " ";
  ss << randomNumberEvent2_ << " ";
  ss << randomNumberEvent3_ << "\n";
  cache->lastEventRandomNumbers_ = ss.str();

  // Print the numbers to a file for each event
  if(nStreams_ > 1) {
    {
      std::lock_guard<std::mutex> lock(write_mutex);
      std::ostringstream ss;
      if(multiStreamReplay_) {
        ss << "replay";
      }
      ss << "testRandomServiceL" << event.eventAuxiliary().luminosityBlock()
         << "E" << event.eventAuxiliary().event() << ".txt";
      std::string filename = ss.str();

      std::ofstream outFile;
      outFile.open(filename.c_str(), std::ofstream::app);

      outFile << moduleDescription().moduleLabel() << " ";
      outFile << engine.name() << " ";

      outFile << "Event random numbers ";
      outFile << randomNumberEvent0_ << " ";
      outFile << randomNumberEvent1_ << " ";
      outFile << randomNumberEvent2_ << " ";
      outFile << randomNumberEvent3_ << "\n";

      outFile.close();
    }
  }

  ++cache->countEvents_;
}

void TestRandomNumberServiceGlobal::beginJob() {
  if(dump_) {
    bool exceptionThrown = true;
    try {
       edm::Service<edm::RandomNumberGenerator> rng;
       unsigned int mySeed = rng->mySeed();
       std::cout << "*** TestRandomNumberServiceGlobal beginJob " << mySeed << "\n";
       exceptionThrown = false;
    } catch( cms::Exception const&) {
    }
    if(not exceptionThrown) {
       throw cms::Exception("FailedToThrow")<<"RandomNunberGenerator::mySeed did not throw";
    }
  }
}

void TestRandomNumberServiceGlobal::endJob() {
  if(dump_) {
    bool exceptionThrown = true;
    try {
       edm::Service<edm::RandomNumberGenerator> rng;
       unsigned int mySeed = rng->mySeed();
       std::cout << "*** TestRandomNumberServiceGlobal endJob " << mySeed << "\n";
       exceptionThrown = false;
    } catch( cms::Exception const&) {
    }
    if(not exceptionThrown) {
       throw cms::Exception("FailedToThrow")<<"RandomNunberGenerator::mySeed did not throw";
    }
  }
}

std::shared_ptr<TestRandomNumberServiceLumiCache>
TestRandomNumberServiceGlobal::globalBeginLuminosityBlock(edm::LuminosityBlock const& lumi,
                                                          edm::EventSetup const& iES) const {

  if(dump_) {
    edm::Service<edm::RandomNumberGenerator> rng;
    std::cout << "*** TestRandomNumberServiceGlobal beginLuminosityBlock " << rng->mySeed() << "\n";
    std::cout << rng->getEngine(lumi.index()).name() << "\n";
  }

  auto lumiCache = std::make_shared<TestRandomNumberServiceLumiCache>();

  unsigned int seed0 = seeds_.at(0) + nStreams_;
  if(lumi.luminosityBlockAuxiliary().luminosityBlock() < seedByLumi_.size()) {
    seed0 = seedByLumi_.at(lumi.luminosityBlockAuxiliary().luminosityBlock());
  }

  if(engineName_ == "RanecuEngine") {
    lumiCache->referenceEngine_ = std::shared_ptr<CLHEP::HepRandomEngine>(new CLHEP::RanecuEngine()); // propagate_const<T> has no reset() function
    long int seedL[2];
    seedL[0] = static_cast<long int>(seed0);
    seedL[1] = static_cast<long int>(seeds_.at(1));
    lumiCache->referenceEngine_->setSeeds(seedL,0);
  }
  else {
    long int seedL = static_cast<long int>(seed0);
    if(engineName_ == "HepJamesRandom") {
      lumiCache->referenceEngine_ = std::shared_ptr<CLHEP::HepRandomEngine>(new CLHEP::HepJamesRandom(seedL)); // propagate_const<T> has no reset() function
    } else {
      lumiCache->referenceEngine_ = std::shared_ptr<CLHEP::HepRandomEngine>(new edm::TRandomAdaptor(seedL)); // propagate_const<T> has no reset() function
    }
  }

  lumiCache->referenceRandomNumbers_.clear();
  lumiCache->referenceRandomNumbers_.push_back(lumiCache->referenceEngine_->flat());
  lumiCache->referenceRandomNumbers_.push_back(lumiCache->referenceEngine_->flat());

  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine& engine = rng->getEngine(lumi.index());

  if(engine.flat() != lumiCache->referenceRandomNumbers_.at(0) ||
     engine.flat() != lumiCache->referenceRandomNumbers_.at(1)) {
    throw cms::Exception("TestRandomNumberService")
      << "TestRandomNumberServiceGlobal::globalBeginLuminosityBlock: Random sequence does not match expected sequence";
  }

  return lumiCache;
}


std::unique_ptr<TestRandomNumberServiceStreamCache>
TestRandomNumberServiceGlobal::beginStream(edm::StreamID streamID) const {

  auto streamCache = std::make_unique<TestRandomNumberServiceStreamCache>();

  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine& engine = rng->getEngine(streamID);

  // Running this will causing the checking code to throw an exception
  // Uncomment to verify the checking works
  // engine.flat();

  streamCache->serviceEngine_ = &engine;

  std::ostringstream suffix;
  suffix << "_" << streamID.value() << "_" << moduleDescription().moduleLabel();
  std::string outFileName = std::string("testRandomService") + suffix.str() + std::string(".txt");
  streamCache->outFile_.open(outFileName.c_str(), std::ofstream::out);

  if(engineName_ == "RanecuEngine") {
    streamCache->referenceEngine_ = std::shared_ptr<CLHEP::HepRandomEngine>(new CLHEP::RanecuEngine()); // propagate_const<T> has no reset() function
    long int seedL[2];
    seedL[0] = static_cast<long int>(seeds_.at(0) + streamID.value() + offset_);
    seedL[1] = static_cast<long int>(seeds_.at(1));
    streamCache->referenceEngine_->setSeeds(seedL,0);
  }
  else {
    long int seedL = static_cast<long int>(seeds_.at(0) + streamID.value() + offset_);
    if(engineName_ == "HepJamesRandom") {
      streamCache->referenceEngine_ = std::shared_ptr<CLHEP::HepRandomEngine>(new CLHEP::HepJamesRandom(seedL)); // propagate_const<T> has no reset() function
    } else {
      streamCache->referenceEngine_ = std::shared_ptr<CLHEP::HepRandomEngine>(new edm::TRandomAdaptor(seedL)); // propagate_const<T> has no reset() function
    }
  }

  for(unsigned int i = 0; i < maxEvents_; ++i) {
    streamCache->referenceRandomNumbers_.push_back(streamCache->referenceEngine_->flat());
    streamCache->referenceRandomNumbers_.push_back(streamCache->referenceEngine_->flat());
    streamCache->referenceRandomNumbers_.push_back(streamCache->referenceEngine_->flat());
    CLHEP::RandExponential expDist(*streamCache->referenceEngine_);
    double mean = 10.0;
    streamCache->referenceRandomNumbers_.push_back(expDist.fire(mean));
  }
  return streamCache;
}

void
TestRandomNumberServiceGlobal::endStream(edm::StreamID streamID) const {

  TestRandomNumberServiceStreamCache* cache = streamCache(streamID);

  std::ostringstream ss;
  if (multiStreamReplay_) {
    ss << "replay";
  }
  ss << "stream" << streamID.value() << "LastEvent.txt";
  std::string filename = ss.str();

  std::ofstream outFile;
  {
    std::lock_guard<std::mutex> lock(write_mutex);
    outFile.open(filename.c_str(), std::ofstream::app);
    outFile << cache->lastEventRandomNumbers_;
    outFile.close();
  }

  // Running this will causing the checking code to throw an exception
  // Uncomment to verify the checking works
  // edm::Service<edm::RandomNumberGenerator> rng;
  // CLHEP::HepRandomEngine& engine = rng->getEngine(streamID);
  // engine.flat();
}

void
TestRandomNumberServiceGlobal::streamBeginRun(edm::StreamID streamID, edm::Run const&, edm::EventSetup const&) const {
  // Running this will causing the checking code to throw an exception
  // Uncomment to verify the checking works
  // edm::Service<edm::RandomNumberGenerator> rng;
  // CLHEP::HepRandomEngine& engine = rng->getEngine(streamID);
  // engine.flat();
}

void
TestRandomNumberServiceGlobal::streamEndRun(edm::StreamID streamID, edm::Run const&, edm::EventSetup const&) const {
  // Running this will causing the checking code to throw an exception
  // Uncomment to verify the checking works
  // edm::Service<edm::RandomNumberGenerator> rng;
  // CLHEP::HepRandomEngine& engine = rng->getEngine(streamID);
  // engine.flat();
}

void
TestRandomNumberServiceGlobal::streamBeginLuminosityBlock(edm::StreamID streamID, edm::LuminosityBlock const&, edm::EventSetup const&) const {
  // Running this will causing the checking code to throw an exception
  // Uncomment to verify the checking works
  // edm::Service<edm::RandomNumberGenerator> rng;
  // CLHEP::HepRandomEngine& engine = rng->getEngine(streamID);
  // engine.flat();
}

void
TestRandomNumberServiceGlobal::streamEndLuminosityBlock(edm::StreamID streamID, edm::LuminosityBlock const&, edm::EventSetup const&) const {
  // Running this will causing the checking code to throw an exception
  // Uncomment to verify the checking works
  // edm::Service<edm::RandomNumberGenerator> rng;
  // CLHEP::HepRandomEngine& engine = rng->getEngine(streamID);
  // engine.flat();
}

//define this as a plug-in
DEFINE_FWK_MODULE(TestRandomNumberServiceGlobal);
