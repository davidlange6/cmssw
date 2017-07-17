#ifndef ThingsTSelector_h
#define ThingsTSelector_h
/** \class ThingsTSelector
 *
 * Simple interactive analysis example based on TSelector
 * accessing EDM data
 *
 * \author Luca Lista, INFN
 *
 */
#include "FWCore/TFWLiteSelector/interface/TFWLiteSelectorBasic.h"
#include "FWCore/Utilities/interface/propagate_const.h"
#include "TH1.h"

namespace tfwliteselectortest {
class ThingsTSelector : public TFWLiteSelectorBasic {
 public:
  ThingsTSelector() : h_a(nullptr), h_refA(nullptr) {}
  void begin(TList*&);
  void preProcessing(const TList*, TList&);
  void process(const edm::Event&);
  void postProcessing(TList&);
  void terminate(TList&);

 private:
  /// histograms
  edm::propagate_const<TH1F*> h_a;
  edm::propagate_const<TH1F*> h_refA;

  ThingsTSelector(ThingsTSelector const&);
  ThingsTSelector operator=(ThingsTSelector const&);

  ClassDef(ThingsTSelector, 2)
};
}
#endif
