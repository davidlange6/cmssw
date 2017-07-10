#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoTrack.h"



void L1Analysis::L1AnalysisRecoTrack::SetTracks(const reco::TrackCollection& trackColl, unsigned maxTrack)
{
   
   track_.nTrk = trackColl.size();
   
   reco::TrackBase::TrackQuality hiPurity = reco::TrackBase::qualityByName("highPurity");
    for(const auto & itk : trackColl){
      if(itk.quality(hiPurity)) track_.nHighPurity++;
    }
    track_.fHighPurity = static_cast<float>(track_.nHighPurity)/static_cast<float>(track_.nTrk);

}

