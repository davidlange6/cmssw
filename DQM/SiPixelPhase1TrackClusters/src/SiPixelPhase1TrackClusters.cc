// -*- C++ -*-
// 
// Package:     SiPixelPhase1TrackClusters
// Class  :     SiPixelPhase1TrackClusters
//

// Original Author: Marcel Schneider

#include "DQM/SiPixelPhase1Common/interface/SiPixelPhase1Base.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/PixelTopology.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "RecoPixelVertexing/PixelLowPtUtilities/interface/ClusterShapeHitFilter.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelClusterShapeCache.h"
#include <string>

namespace {

class SiPixelPhase1TrackClusters final : public SiPixelPhase1Base {


public:
  explicit SiPixelPhase1TrackClusters(const edm::ParameterSet& conf);
  void analyze(const edm::Event&, const edm::EventSetup&) override;

private:
  const bool applyVertexCut_;

  const edm::EDGetTokenT<reco::TrackCollection> tracksToken_;
  const edm::EDGetTokenT<reco::VertexCollection> offlinePrimaryVerticesToken_;
  const edm::EDGetTokenT<SiPixelClusterShapeCache> pixelClusterShapeCacheToken_;

  unsigned int SiPixelPhase1TrackClustersOnTrackCharge=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackSize=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackShape=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackNClusters=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackPositionB=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackPositionF=999;
  unsigned int SiPixelPhase1DigisHitmapOnTrack=999;

  unsigned int SiPixelPhase1TrackClustersNTracks=999;
  unsigned int SiPixelPhase1TrackClustersNTracksInVolume=999;

  unsigned int SiPixelPhase1ClustersSizeVsEtaOnTrackOuter=999;
  unsigned int SiPixelPhase1ClustersSizeVsEtaOnTrackInner=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackChargeOuter=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackChargeInner=999;

  unsigned int SiPixelPhase1TrackClustersOnTrackShapeOuter=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackShapeInner=999;

  unsigned int SiPixelPhase1TrackClustersOnTrackSizeXOuter=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackSizeXInner=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackSizeXF=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackSizeYOuter=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackSizeYInner=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackSizeYF=999;
    
  unsigned int SiPixelPhase1TrackClustersOnTrackSizeXYOuter=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackSizeXYInner=999;
  unsigned int SiPixelPhase1TrackClustersOnTrackSizeXYF=999;
  unsigned int SiPixelPhase1ClustersChargeVsSizeOnTrack=999;

  //this wrapper protects against filling histograms that the user does not care about
  void fill(unsigned int ind, double x, double y, DetId id, const edm::Event *event =nullptr, int col = 0, int row = 0) 
  { if ( ind < histo.size() ) histo[ind].fill(x,y,id,event,col,row); }
  void fill(unsigned int ind, double x, DetId id, const edm::Event *event =nullptr, int col = 0, int row = 0) 
  { if ( ind < histo.size() ) histo[ind].fill(x,id,event,col,row); }
  void fill(unsigned int ind, DetId id, const edm::Event *event = nullptr, int col = 0, int row = 0) 
  {  if ( ind < histo.size() ) histo[ind].fill(id,event,col,row); } 

};



SiPixelPhase1TrackClusters::SiPixelPhase1TrackClusters(const edm::ParameterSet& iConfig) :
  SiPixelPhase1Base(iConfig),
  applyVertexCut_(iConfig.getUntrackedParameter<bool>("VertexCut",true)),
  tracksToken_(consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("tracks"))),
  offlinePrimaryVerticesToken_(applyVertexCut_ ?
                              consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices")) :
                              edm::EDGetTokenT<reco::VertexCollection>()),
  pixelClusterShapeCacheToken_(consumes<SiPixelClusterShapeCache>(iConfig.getParameter<edm::InputTag>("clusterShapeCache")))
{


  //build index to histograms
  auto histograms = iConfig.getParameter<edm::VParameterSet>("histograms");
  for (unsigned int i=0; i<histograms.size(); i++) {
    std::string name = histograms[i].getParameter<std::string>("name");

    if ( name == "charge" ) SiPixelPhase1TrackClustersOnTrackCharge=i;
    else if ( name == "size" ) SiPixelPhase1TrackClustersOnTrackSize=i;
  }
}

void SiPixelPhase1TrackClusters::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {


  // get geometry
  edm::ESHandle<TrackerGeometry> tracker;
  iSetup.get<TrackerDigiGeometryRecord>().get(tracker);
  assert(tracker.isValid());

   edm::ESHandle<TrackerTopology> tTopoHandle;
   iSetup.get<TrackerTopologyRcd>().get(tTopoHandle);
   auto const & tkTpl = *tTopoHandle;

   edm::ESHandle<ClusterShapeHitFilter> shapeFilterH;
   iSetup.get<CkfComponentsRecord>().get("ClusterShapeHitFilter", shapeFilterH);
   auto const & shapeFilter = *shapeFilterH;


  edm::Handle<reco::VertexCollection> vertices;
  if(applyVertexCut_) {
    iEvent.getByToken(offlinePrimaryVerticesToken_, vertices);
    if (!vertices.isValid() || vertices->empty()) return;
  }


  //get the map
  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByToken( tracksToken_, tracks);

  if ( !tracks.isValid() ) {
    edm::LogWarning("SiPixelPhase1TrackClusters")  << "track collection is not valid";
    return;
  }

  edm::Handle<SiPixelClusterShapeCache> pixelClusterShapeCacheH;
  iEvent.getByToken(pixelClusterShapeCacheToken_, pixelClusterShapeCacheH);
  if ( !pixelClusterShapeCacheH.isValid() ) {
    edm::LogWarning("SiPixelPhase1TrackClusters")  << "PixelClusterShapeCache collection is not valid";
    return;
  }  
  auto const & pixelClusterShapeCache = *pixelClusterShapeCacheH;

  
  for (auto const & track : *tracks) {

    if (applyVertexCut_ && (track.pt() < 0.75 || std::abs( track.dxy((*vertices)[0].position()) ) > 5*track.dxyError())) continue;

    bool isBpixtrack = false, isFpixtrack = false, crossesPixVol=false;

    // find out whether track crosses pixel fiducial volume (for cosmic tracks)
    auto d0 = track.d0(), dz = track.dz(); 
    if(std::abs(d0)<15 && std::abs(dz)<50) crossesPixVol = true;

    auto etatk = track.eta();


    auto const & trajParams = track.extra()->trajParams();
    assert(trajParams.size()==track.recHitsSize());
    auto hb = track.recHitsBegin();
    for(unsigned int h=0;h<track.recHitsSize();h++){
      auto hit = *(hb+h);
      if (!hit->isValid()) continue;
      auto id = hit->geographicalId();

      // check that we are in the pixel
      auto subdetid = (id.subdetId());
      if (subdetid == PixelSubdetector::PixelBarrel) isBpixtrack = true;
      if (subdetid == PixelSubdetector::PixelEndcap) isFpixtrack = true;
      if (subdetid != PixelSubdetector::PixelBarrel && subdetid != PixelSubdetector::PixelEndcap) continue;
      bool iAmBarrel = subdetid ==PixelSubdetector::PixelBarrel;
      auto pixhit = dynamic_cast<const SiPixelRecHit*>(hit->hit());
      if (!pixhit) continue;

      // auto geomdetunit = dynamic_cast<const PixelGeomDetUnit*> (pixhit->detUnit());
      // auto const & topol = geomdetunit->specificTopology();
      
      // get the cluster
      auto clustp = pixhit->cluster();
      if (clustp.isNull()) continue; 
      auto const & cluster = *clustp;

      auto const & ltp = trajParams[h];
      
      auto localDir = ltp.momentum()/ltp.momentum().mag();

      // correct charge for track impact angle
      auto charge = cluster.charge()*ltp.absdz();

      auto clustgp =  pixhit->globalPosition();  // from rechit


      int part;
      ClusterData::ArrayType meas;
      std::pair<float,float> pred;
      if(shapeFilter.getSizes(*pixhit,localDir,pixelClusterShapeCache, part,meas, pred)) {
       auto shape = shapeFilter.isCompatible(*pixhit,localDir,pixelClusterShapeCache);
       if (iAmBarrel) {
         if(tkTpl.pxbLadder(id)%2==1) {
           fill(SiPixelPhase1TrackClustersOnTrackSizeXOuter,pred.first, cluster.sizeX(), id, &iEvent);
           fill(SiPixelPhase1TrackClustersOnTrackSizeYOuter,pred.second,cluster.sizeY(), id, &iEvent);
           fill(SiPixelPhase1TrackClustersOnTrackSizeXYOuter,cluster.sizeY(),cluster.sizeX(), id, &iEvent);

           fill(SiPixelPhase1TrackClustersOnTrackShapeOuter,shape?1:0,id, &iEvent);
         } else {
           fill(SiPixelPhase1TrackClustersOnTrackSizeXInner,pred.first, cluster.sizeX(), id, &iEvent);
           fill(SiPixelPhase1TrackClustersOnTrackSizeYInner,pred.second,cluster.sizeY(), id, &iEvent);
           fill(SiPixelPhase1TrackClustersOnTrackSizeXYInner,cluster.sizeY(),cluster.sizeX(), id, &iEvent);

           fill(SiPixelPhase1TrackClustersOnTrackShapeInner,shape?1:0,id, &iEvent);
         }
       } else {
           fill(SiPixelPhase1TrackClustersOnTrackSizeXF,pred.first, cluster.sizeX(), id, &iEvent);
           fill(SiPixelPhase1TrackClustersOnTrackSizeYF,pred.second,cluster.sizeY(), id, &iEvent);
           fill(SiPixelPhase1TrackClustersOnTrackSizeXYF,cluster.sizeY(),cluster.sizeX(), id, &iEvent);
       }
       fill(SiPixelPhase1TrackClustersOnTrackShape,shape?1:0,id, &iEvent);
      }

      for (int i =0; i<cluster.size(); i++){
          SiPixelCluster::Pixel const & vecipxl = cluster.pixel(i);
          fill(SiPixelPhase1DigisHitmapOnTrack,id, &iEvent, vecipxl.y, vecipxl.x);
      }

      fill(SiPixelPhase1TrackClustersOnTrackNClusters,id, &iEvent);
      fill(SiPixelPhase1TrackClustersOnTrackCharge,charge, id, &iEvent);
      fill(SiPixelPhase1TrackClustersOnTrackSize,cluster.size(), id, &iEvent);

      fill(SiPixelPhase1TrackClustersOnTrackPositionB,clustgp.z(),   clustgp.phi(),   id, &iEvent);
      fill(SiPixelPhase1TrackClustersOnTrackPositionF,clustgp.x(),   clustgp.y(),     id, &iEvent);

      fill(SiPixelPhase1ClustersChargeVsSizeOnTrack,cluster.size(), charge, id, &iEvent);

      if(tkTpl.pxbLadder(id)%2==1) {
        fill(SiPixelPhase1ClustersSizeVsEtaOnTrackOuter,etatk, cluster.sizeY(), id, &iEvent);
        fill(SiPixelPhase1TrackClustersOnTrackChargeOuter,charge, id, &iEvent);
      } else {
        fill(SiPixelPhase1ClustersSizeVsEtaOnTrackInner,etatk, cluster.sizeY(), id, &iEvent);
        fill(SiPixelPhase1TrackClustersOnTrackChargeInner,charge, id, &iEvent);
      }


    }

    // statistics on tracks
    fill(SiPixelPhase1TrackClustersNTracks,1, DetId(0), &iEvent);
    if (isBpixtrack || isFpixtrack) 
      fill(SiPixelPhase1TrackClustersNTracks,2, DetId(0), &iEvent);
    if (isBpixtrack) 
      fill(SiPixelPhase1TrackClustersNTracks,3, DetId(0), &iEvent);
    if (isFpixtrack) 
      fill(SiPixelPhase1TrackClustersNTracks,4, DetId(0), &iEvent);

    if (crossesPixVol) {
      if (isBpixtrack || isFpixtrack)
        fill(SiPixelPhase1TrackClustersNTracksInVolume,1, DetId(0), &iEvent);
      else 
        fill(SiPixelPhase1TrackClustersNTracksInVolume,0, DetId(0), &iEvent);
    }
  }

  if (SiPixelPhase1TrackClustersOnTrackNClusters < histo.size() ) 
    histo[SiPixelPhase1TrackClustersOnTrackNClusters].executePerEventHarvesting(&iEvent);
}

}// namespace

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SiPixelPhase1TrackClusters);

