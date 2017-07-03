#ifndef TrackingMaterialProducer_h
#define TrackingMaterialProducer_h
#include <string>
#include <vector>
 
#include "SimG4Core/Watcher/interface/SimProducer.h"
#include "SimG4Core/Notification/interface/Observer.h"

#include "G4LogicalVolume.hh"

#include "SimDataFormats/ValidationFormats/interface/MaterialAccountingTrack.h"

#include "TProfile.h"
#include "TFile.h"

class BeginOfJob;
class EndOfJob;
class BeginOfEvent;
class BeginOfTrack;
class EndOfTrack;
class G4Step;

class G4StepPoint;
class G4VTouchable;
class G4VPhysicalVolume;
class G4LogicalVolume;

class TrackingMaterialProducer : public SimProducer,
                                 public Observer<const BeginOfJob*>,
                                 public Observer<const EndOfJob*>,
                                 public Observer<const BeginOfEvent*>,
                                 public Observer<const BeginOfTrack*>,
                                 public Observer<const G4Step*>,
                                 public Observer<const EndOfTrack*>
{
public:
  TrackingMaterialProducer(const edm::ParameterSet&);
  ~TrackingMaterialProducer() override;
  
private:
  void update(const BeginOfJob*) override;
  void update(const BeginOfEvent*) override;
  void update(const BeginOfTrack*) override;
  void update(const G4Step*) override;
  void update(const EndOfTrack*) override;
  void update(const EndOfJob*) override;
  void produce(edm::Event&, const edm::EventSetup&) override;
 
  bool isSelected( const G4VTouchable* touch );
  bool isSelectedFast( const G4TouchableHistory* touch );

private:
  bool                                  m_primaryTracks;
  std::vector<std::string>              m_selectedNames; 
  std::vector<const G4LogicalVolume *>  m_selectedVolumes;
  MaterialAccountingTrack               m_track;
  std::vector<MaterialAccountingTrack>* m_tracks;  
  TFile * output_file_;
  TProfile *  radLen_vs_eta_;
};

#endif // TrackingMaterialProducer_h
