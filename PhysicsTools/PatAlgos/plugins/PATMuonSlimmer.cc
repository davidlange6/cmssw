/**
  \class    pat::PATMuonSlimmer PATMuonSlimmer.h "PhysicsTools/PatAlgos/interface/PATMuonSlimmer.h"
  \brief    Slimmer of PAT Muons 
*/


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Common/interface/RefToPtr.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "PhysicsTools/PatAlgos/interface/ObjectModifier.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/PatCandidates/interface/libminifloat.h"

namespace pat {
  
  class PATMuonSlimmer : public edm::stream::EDProducer<> {
  public:
    explicit PATMuonSlimmer(const edm::ParameterSet & iConfig);
    virtual ~PATMuonSlimmer() { }
    
    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup) override;
    virtual void beginLuminosityBlock(const edm::LuminosityBlock&, const  edm::EventSetup&) override final;
    
  private:
    const edm::EDGetTokenT<pat::MuonCollection> src_;
    std::vector<edm::EDGetTokenT<reco::PFCandidateCollection>> pf_;
    std::vector<edm::EDGetTokenT<edm::Association<pat::PackedCandidateCollection>>> pf2pc_;
    const bool linkToPackedPF_;
    const StringCutObjectSelector<pat::Muon> saveTeVMuons_, dropDirectionalIso_, dropPfP4_, slimCaloVars_, slimKinkVars_, slimCaloMETCorr_, slimMatches_;
    const bool modifyMuon_;
    std::unique_ptr<pat::ObjectModifier<pat::Muon> > muonModifier_;
  };

} // namespace

pat::PATMuonSlimmer::PATMuonSlimmer(const edm::ParameterSet & iConfig) :
    src_(consumes<pat::MuonCollection>(iConfig.getParameter<edm::InputTag>("src"))),
    linkToPackedPF_(iConfig.getParameter<bool>("linkToPackedPFCandidates")),
    saveTeVMuons_(iConfig.getParameter<std::string>("saveTeVMuons")),
    dropDirectionalIso_(iConfig.getParameter<std::string>("dropDirectionalIso")),
    dropPfP4_(iConfig.getParameter<std::string>("dropPfP4")),
    slimCaloVars_(iConfig.getParameter<std::string>("slimCaloVars")),
    slimKinkVars_(iConfig.getParameter<std::string>("slimKinkVars")),
    slimCaloMETCorr_(iConfig.getParameter<std::string>("slimCaloMETCorr")),
    slimMatches_(iConfig.getParameter<std::string>("slimMatches")),
    modifyMuon_(iConfig.getParameter<bool>("modifyMuons"))
{
    if (linkToPackedPF_) {
      const std::vector<edm::InputTag> & pf = iConfig.getParameter<std::vector<edm::InputTag>>("pfCandidates");
      const std::vector<edm::InputTag> & pf2pc = iConfig.getParameter<std::vector<edm::InputTag>>("packedPFCandidates");
        if (pf.size() != pf2pc.size()) throw cms::Exception("Configuration") << "Mismatching pfCandidates and packedPFCandidates\n";
        for (const edm::InputTag &tag : pf) pf_.push_back(consumes<reco::PFCandidateCollection>(tag));
        for (const edm::InputTag &tag : pf2pc) pf2pc_.push_back(consumes<edm::Association<pat::PackedCandidateCollection>>(tag));
    }

    edm::ConsumesCollector sumes(consumesCollector());
    if( modifyMuon_ ) {
      const edm::ParameterSet& mod_config = iConfig.getParameter<edm::ParameterSet>("modifierConfig");
      muonModifier_.reset(new pat::ObjectModifier<pat::Muon>(mod_config) );
      muonModifier_->setConsumes(sumes);
    } else {
      muonModifier_.reset(nullptr);
    }
    produces<std::vector<pat::Muon> >();
}

void 
pat::PATMuonSlimmer::beginLuminosityBlock(const edm::LuminosityBlock&, const  edm::EventSetup& iSetup) {
  if( modifyMuon_ ) muonModifier_->setEventContent(iSetup);
}

void 
pat::PATMuonSlimmer::produce(edm::Event & iEvent, const edm::EventSetup & iSetup) {
    using namespace edm;
    using namespace std;

    Handle<pat::MuonCollection>      src;
    iEvent.getByToken(src_, src);

    auto out = std::make_unique<std::vector<pat::Muon>>();
    out->reserve(src->size());

    if( modifyMuon_ ) { muonModifier_->setEvent(iEvent); }

    std::map<reco::CandidatePtr,pat::PackedCandidateRef> mu2pc;
    if (linkToPackedPF_) {
        Handle<reco::PFCandidateCollection> pf;
        Handle<edm::Association<pat::PackedCandidateCollection>> pf2pc;
        for (unsigned int ipfh = 0, npfh = pf_.size(); ipfh < npfh; ++ipfh) {
            iEvent.getByToken(pf_[ipfh], pf);
            iEvent.getByToken(pf2pc_[ipfh], pf2pc);
            const auto & pfcoll = (*pf);
            const auto & pfmap  = (*pf2pc);
            for (unsigned int i = 0, n = pf->size(); i < n; ++i) {
                const reco::PFCandidate &p = pfcoll[i];
                if (p.muonRef().isNonnull()) mu2pc[refToPtr(p.muonRef())] = pfmap[reco::PFCandidateRef(pf, i)];
            }
        }
    }

    for (const auto & it : *src) {
        out->push_back(it);
        pat::Muon & mu = out->back();
        
        if( modifyMuon_ ) { muonModifier_->modify(mu); }

	if (saveTeVMuons_(mu)){mu.embedPickyMuon(); mu.embedTpfmsMuon(); mu.embedDytMuon();}
	if (linkToPackedPF_) {
            mu.refToOrig_ = refToPtr(mu2pc[mu.refToOrig_]);
        }
        if (dropDirectionalIso_(mu)) {
            reco::MuonPFIsolation zero;
            mu.setPFIsolation("pfIsoMeanDRProfileR03",zero);
            mu.setPFIsolation("pfIsoSumDRProfileR03",zero);
            mu.setPFIsolation("pfIsoMeanDRProfileR04",zero);
            mu.setPFIsolation("pfIsoSumDRProfileR04",zero);
        }
        if (mu.isPFMuon() && dropPfP4_(mu)) mu.setPFP4(reco::Particle::LorentzVector());
        if (slimCaloVars_(mu) && mu.isEnergyValid()) {
            reco::MuonEnergy ene = mu.calEnergy();
            if (ene.tower)   ene.tower = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.tower);
            if (ene.towerS9) ene.towerS9 = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.towerS9);
            if (ene.had)     ene.had = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.had);
            if (ene.hadS9)  ene.hadS9 = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.hadS9);
            if (ene.hadMax) ene.hadMax = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.hadMax);
            if (ene.em)    ene.em = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.em);
            if (ene.emS25) ene.emS25 = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.emS25);
            if (ene.emMax) ene.emMax = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.emMax);
            if (ene.hcal_time)   ene.hcal_time = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.hcal_time);
            if (ene.hcal_timeError)   ene.hcal_timeError = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.hcal_timeError);
            if (ene.ecal_time)   ene.ecal_time = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.ecal_time);
            if (ene.ecal_timeError)   ene.ecal_timeError = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.ecal_timeError);
            ene.ecal_position = math::XYZPointF(MiniFloatConverter::reduceMantissaToNbitsRounding<14>(ene.ecal_position.X()),
						MiniFloatConverter::reduceMantissaToNbitsRounding<14>(ene.ecal_position.Y()),
						MiniFloatConverter::reduceMantissaToNbitsRounding<14>(ene.ecal_position.Z()));
            ene.hcal_position = math::XYZPointF(MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.hcal_position.X()),
						MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.hcal_position.Y()), 
						MiniFloatConverter::reduceMantissaToNbitsRounding<12>(ene.hcal_position.Z()));
            mu.setCalEnergy(ene);
        }
        if (slimKinkVars_(mu) && mu.isQualityValid()) {
            reco::MuonQuality qual = mu.combinedQuality();
            qual.tkKink_position = math::XYZPointF(MiniFloatConverter::reduceMantissaToNbitsRounding<12>(qual.tkKink_position.X()),
						   MiniFloatConverter::reduceMantissaToNbitsRounding<12>(qual.tkKink_position.Y()),
						   MiniFloatConverter::reduceMantissaToNbitsRounding<12>(qual.tkKink_position.Z()));
            mu.setCombinedQuality(qual);
        }
        if (slimCaloMETCorr_(mu) && mu.caloMETMuonCorrs().type() != reco::MuonMETCorrectionData::NotUsed) {
            reco::MuonMETCorrectionData corrs = mu.caloMETMuonCorrs();
            corrs = reco::MuonMETCorrectionData(corrs.type(), MiniFloatConverter::reduceMantissaToNbitsRounding<10>(corrs.corrX()), MiniFloatConverter::reduceMantissaToNbitsRounding<10>(corrs.corrY()));
            mu.embedCaloMETMuonCorrs(corrs);
        }
        if (slimMatches_(mu) && mu.isMatchesValid()) {
            for (reco::MuonChamberMatch & cmatch : mu.matches()) {
                cmatch.edgeX = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(cmatch.edgeX);
                cmatch.edgeY = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(cmatch.edgeY);
                cmatch.xErr = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(cmatch.xErr);
                cmatch.yErr = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(cmatch.yErr);
                cmatch.dXdZErr = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(cmatch.dXdZErr);
                cmatch.dYdZErr = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(cmatch.dYdZErr);
                for (reco::MuonSegmentMatch & smatch : cmatch.segmentMatches) {
                    smatch.xErr = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(smatch.xErr);
                    smatch.yErr = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(smatch.yErr);
                    smatch.dXdZErr = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(smatch.dXdZErr);
                    smatch.dYdZErr = MiniFloatConverter::reduceMantissaToNbitsRounding<12>(smatch.dYdZErr);
                }
            }
        }
    }

    iEvent.put(std::move(out));
}

#include "FWCore/Framework/interface/MakerMacros.h"
using namespace pat;
DEFINE_FWK_MODULE(PATMuonSlimmer);
