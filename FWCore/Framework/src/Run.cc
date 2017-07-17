#include "FWCore/Framework/interface/Run.h"

#include "FWCore/Framework/interface/RunPrincipal.h"
#include "FWCore/Utilities/interface/Algorithms.h"
#include "FWCore/Utilities/interface/get_underlying_safe.h"

namespace edm {

std::string const Run::emptyString_;

Run::Run(RunPrincipal const& rp, ModuleDescription const& md,
         ModuleCallingContext const* moduleCallingContext)
    : provRecorder_(rp, md),
      aux_(rp.aux()),
      moduleCallingContext_(moduleCallingContext) {}

Run::~Run() {}

Run::CacheIdentifier_t Run::cacheIdentifier() const {
  return runPrincipal().cacheIdentifier();
}

RunIndex Run::index() const { return runPrincipal().index(); }

RunPrincipal const& Run::runPrincipal() const {
  return dynamic_cast<RunPrincipal const&>(provRecorder_.principal());
}

Provenance Run::getProvenance(BranchID const& bid) const {
  return runPrincipal().getProvenance(bid, moduleCallingContext_);
}

void Run::getAllStableProvenance(
    std::vector<StableProvenance const*>& provenances) const {
  runPrincipal().getAllStableProvenance(provenances);
}

/* Not yet fully implemented
  bool
  Run::getProcessParameterSet(std::string const& processName,
  std::vector<ParameterSet>& psets) const {
    // Get the relevant ProcessHistoryIDs
    ProcessHistoryRegistry* phreg = ProcessHistoryRegistry::instance();
    // Need to fill these in.
    std::vector<ProcessHistoryID> historyIDs;


    // Get the relevant ParameterSetIDs.
    // Need to fill these in.
    std::vector<ParameterSetID> psetIdsUsed;
    for(std::vector<ProcessHistoryID>::const_iterator
           i = historyIDs.begin(),
           e = historyIDs.end();
         i != e;
         ++i) {
      ProcessHistory temp;
      phreg->getMapped(*i, temp);
    }

    // Look up the ParameterSets for these IDs.
    pset::Registry* psreg = pset::Registry::instance();
    for(std::vector<ParameterSetID>::const_iterator
           i = psetIdsUsed.begin(),
           e = psetIdsUsed.end();
         i != e;
         ++i) {
      ParameterSet temp;
      psreg->getMapped(*i, temp);
      psets.push_back(temp);
    }

    return false;
  }
*/

void Run::commit_(std::vector<edm::ProductResolverIndex> const& iShouldPut) {
  RunPrincipal const& rp = runPrincipal();
  ProductPtrVec::iterator pit(putProducts().begin());
  ProductPtrVec::iterator pie(putProducts().end());

  while (pit != pie) {
    rp.put(*pit->second, std::move(get_underlying_safe(pit->first)));
    ++pit;
  }

  auto sz = iShouldPut.size();
  if (sz != 0 and sz != putProducts().size()) {
    // some were missed
    auto& p = provRecorder_.principal();
    for (auto index : iShouldPut) {
      auto resolver = p.getProductResolverByIndex(index);
      if (not resolver->productResolved()) {
        resolver->putProduct(std::unique_ptr<WrapperBase>());
      }
    }
  }

  // the cleanup is all or none
  putProducts().clear();
}

ProcessHistoryID const& Run::processHistoryID() const {
  return runPrincipal().processHistoryID();
}

ProcessHistory const& Run::processHistory() const {
  return provRecorder_.processHistory();
}

BasicHandle Run::getByLabelImpl(std::type_info const&,
                                std::type_info const& iProductType,
                                const InputTag& iTag) const {
  BasicHandle h = provRecorder_.getByLabel_(TypeID(iProductType), iTag,
                                            moduleCallingContext_);
  return h;
}
}
