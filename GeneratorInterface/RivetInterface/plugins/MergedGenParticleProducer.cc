#include "MergedGenParticleProducer.hh"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"

#include "HepPDT/ParticleID.hh"

MergedGenParticleProducer::MergedGenParticleProducer(const edm::ParameterSet& config)
{
  input_pruned_ = consumes<edm::View<reco::GenParticle>>(config.getParameter<edm::InputTag>("inputPruned"));
  input_packed_ = consumes<edm::View<pat::PackedGenParticle>>(config.getParameter<edm::InputTag>("inputPacked"));

  produces<reco::GenParticleCollection>();
}

void MergedGenParticleProducer::produce(edm::Event& event, const edm::EventSetup& setup)
{
  // Need a ref to the product now for creating the mother/daughter refs
  auto ref = event.getRefBeforePut<reco::GenParticleCollection>();

  // Get the input collections
  edm::Handle<edm::View<reco::GenParticle> > pruned_handle;
  event.getByToken(input_pruned_, pruned_handle);

  edm::Handle<edm::View<pat::PackedGenParticle> > packed_handle;
  event.getByToken(input_packed_, packed_handle);

  // First determine which packed particles are also still in the pruned collection
  // so that we can skip them later
  std::map<pat::PackedGenParticle const*, reco::GenParticle const*> st1_dup_map;

  // Also map pointers in the original pruned collection to their index in the vector.
  // This index will be the same in the merged collection.
  std::map<reco::Candidate const*, std::size_t> pruned_idx_map;

  for (unsigned int i = 0; i < pruned_handle->size(); ++i) {
    reco::GenParticle const& src = pruned_handle->at(i);
    pruned_idx_map[&src] = i;
    if (src.status() != 1) continue;

    // Convert the pruned GenParticle into a PackedGenParticle then do an exact
    // floating point comparison of the pt, eta and phi
    // This of course relies on the PackedGenParticle constructor being identical
    // between the CMSSW version this sample was produced with and the one we're
    // analysing with
    pat::PackedGenParticle pks(src, reco::GenParticleRef());
    unsigned found_matches = 0;
    for (const auto & pk : *packed_handle) {
      if ( pks.pdgId() != pk.pdgId() or pks.p4() != pk.p4() ) continue;
      ++found_matches;
      st1_dup_map[&pk] = &src;
    }
    if (found_matches > 1) {
      edm::LogWarning("MergedGenParticleProducer") << "Found multiple packed matches for: " << i << "\t" << src.pdgId() << "\t" << src.pt() << "\t" << src.y() << "\n";
    }
    else if (found_matches == 0 && std::abs(src.y()) < 6.0) {
      edm::LogWarning("MergedGenParticleProducer") << "unmatched status 1: " << i << "\t" << src.pdgId() << "\t" << src.pt() << "\t" << src.y() << "\n";
    }
  }

  // Fix by Markus
  // check for photons from pruned (light) hadrons
  unsigned int nPhotonsFromPrunedHadron = 0;
  for (const auto & pk : *packed_handle) {
    if (isPhotonFromPrunedHadron(pk)) ++nPhotonsFromPrunedHadron;
  }

  // At this point we know what the size of the merged GenParticle will be so we can create it
  const unsigned int n = pruned_handle->size() + (packed_handle->size() - st1_dup_map.size()) + nPhotonsFromPrunedHadron;
  auto cands = std::unique_ptr<reco::GenParticleCollection>(new reco::GenParticleCollection(n));

  // First copy in all the pruned candidates
  for (unsigned i = 0; i < pruned_handle->size(); ++i) {
    reco::GenParticle const& old_cand = pruned_handle->at(i);
    reco::GenParticle & new_cand = cands->at(i);
    new_cand = reco::GenParticle(pruned_handle->at(i));
    // Update the mother and daughter refs to this new merged collection
    new_cand.resetMothers(ref.id());
    new_cand.resetDaughters(ref.id());
    for (unsigned m = 0; m < old_cand.numberOfMothers(); ++m) {
      new_cand.addMother(reco::GenParticleRef(ref, pruned_idx_map.at(old_cand.mother(m))));
    }
    for (unsigned d = 0; d < old_cand.numberOfDaughters(); ++d) {
      new_cand.addDaughter(reco::GenParticleRef(ref, pruned_idx_map.at(old_cand.daughter(d))));
    }
  }

  // Now copy in the packed candidates that are not already in the pruned
  for (unsigned i = 0, idx = pruned_handle->size(); i < packed_handle->size(); ++i) {
    pat::PackedGenParticle const& pk = packed_handle->at(i);
    if (st1_dup_map.count(&pk)) continue;
    reco::GenParticle & new_cand = cands->at(idx);
    new_cand = reco::GenParticle(pk.charge(), pk.p4(), pk.vertex(), pk.pdgId(), 1, true);

    // Insert dummy pi0 mothers for orphaned photons
    if (isPhotonFromPrunedHadron(pk)) {
      ++idx;
      reco::GenParticle & dummy_mother = cands->at(idx);
      dummy_mother = reco::GenParticle(0, pk.p4(), pk.vertex(), 111, 2, true);
      for (unsigned m = 0; m < pk.numberOfMothers(); ++m) {
        new_cand.addMother(reco::GenParticleRef(ref, idx));
        // Since the packed candidates drop the vertex position we'll take this from the mother
        if (m == 0) {
          dummy_mother.setP4(pk.mother(m)->p4());
          dummy_mother.setVertex(pk.mother(m)->vertex());
          new_cand.setVertex(pk.mother(m)->vertex());
        }
        // Should then add the GenParticle as a daughter of its dummy mother
        dummy_mother.addDaughter(reco::GenParticleRef(ref, idx-1));
      }
    }
    // Connect to mother from pruned particles
    reco::GenParticle & daughter = cands->at(idx);
    for (unsigned m = 0; m < pk.numberOfMothers(); ++m) {
      daughter.addMother(reco::GenParticleRef(ref, pruned_idx_map.at(pk.mother(m))));
      // Since the packed candidates drop the vertex position we'll take this from the mother
      if (m == 0) {
        daughter.setVertex(pk.mother(m)->vertex());
      }
      // Should then add this GenParticle as a daughter of its mother
      cands->at(pruned_idx_map.at(pk.mother(m))).addDaughter(reco::GenParticleRef(ref, idx));
    }
    ++idx;
  }

  event.put(std::move(cands));
}

bool MergedGenParticleProducer::isPhotonFromPrunedHadron(const pat::PackedGenParticle& pk) const
{
  HepPDT::ParticleID motherid(pk.mother(0)->pdgId());
  return
    ( pk.pdgId() == 22 // We care about photons for lepton dressing here
      and pk.statusFlags().isDirectHadronDecayProduct() // Gen status flag seems correct
      // Catch cases where miniaod mother is not compatible with the status flag
      and not (motherid.isHadron() and pk.mother(0)->status() == 2)
    );
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MergedGenParticleProducer);
