#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <memory>
#include <cassert>

#include "boost/shared_ptr.hpp"

#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"

#include "Pythia8/Pythia.h"
#include "Pythia8/LesHouches.h"
#include "Pythia8Plugins/HepMC2.h"

#include "SimDataFormats/GeneratorProducts/interface/LesHouches.h"
#include "GeneratorInterface/LHEInterface/interface/LHERunInfo.h"
#include "GeneratorInterface/LHEInterface/interface/LHEEvent.h"

class LHAupLesHouches : public Pythia8::LHAup {
  public:
    LHAupLesHouches() : setScalesFromLHEF_(false),fEvAttributes(0) {;}

    //void loadRunInfo(const boost::shared_ptr<lhef::LHERunInfo> &runInfo)
    void loadRunInfo(lhef::LHERunInfo* runInfo)
      { this->runInfo = runInfo; }

    //void loadEvent(const boost::shared_ptr<lhef::LHEEvent> &event)
    void loadEvent(lhef::LHEEvent* event)
      { this->event = event; }
      
    void setScalesFromLHEF(bool b) { setScalesFromLHEF_ = b; }

    ~LHAupLesHouches() {if(fEvAttributes) delete fEvAttributes;}

  private:

    bool setInit();
    bool setEvent(int idProcIn);

    //boost::shared_ptr<lhef::LHERunInfo> runInfo;
    lhef::LHERunInfo* runInfo;
    //boost::shared_ptr<lhef::LHEEvent>	event;
    lhef::LHEEvent* event;
    
    // Flag to set particle production scales or not.
    bool setScalesFromLHEF_;

    std::map<std::string, std::string> * fEvAttributes;
};
