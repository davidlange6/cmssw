//-------------------------------------------------
//
/** \class L1MuGMTLFEtaConvLUT
 *
 *   LFEtaConv look-up table
 *          
 *   this class was automatically generated by 
 *     L1MuGMTLUT::MakeSubClass()  
*/ 
//
//   Author :
//   H. Sakulin            HEPHY Vienna
//
//   Migrated to CMSSW:
//   I. Mikulec
//
//--------------------------------------------------
#ifndef L1TriggerGlobalMuonTrigger_L1MuGMTLFEtaConvLUT_h
#define L1TriggerGlobalMuonTrigger_L1MuGMTLFEtaConvLUT_h

//---------------
// C++ Headers --
//---------------


//----------------------
// Base Class Headers --
//----------------------
#include "L1Trigger/GlobalMuonTrigger/src/L1MuGMTLUT.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------


//              ---------------------
//              -- Class Interface --
//              ---------------------


class L1MuGMTLFEtaConvLUT : public L1MuGMTLUT {
  
 public:
  enum {DT, bRPC, CSC, fRPC};

  /// constuctor using function-lookup
  L1MuGMTLFEtaConvLUT() : L1MuGMTLUT("LFEtaConv", 
				       "DT bRPC CSC fRPC",
				       "eta_regional(6)",
				       "eta_gmt(6)", 6, true) {
    InitParameters();
  } ;

  /// destructor
  ~L1MuGMTLFEtaConvLUT() override {};

  /// specific lookup function for eta_gmt
  unsigned SpecificLookup_eta_gmt (int idx, unsigned eta_regional) const {
    std::vector<unsigned> addr(1);
    addr[0] = eta_regional;
    return Lookup(idx, addr) [0];
  };

  /// specific lookup function for entire output field
  unsigned SpecificLookup (int idx, unsigned eta_regional) const {
    std::vector<unsigned> addr(1);
    addr[0] = eta_regional;
    return LookupPacked(idx, addr);
  };



  /// access to lookup function with packed input and output

  unsigned LookupFunctionPacked (int idx, unsigned address) const override {
    std::vector<unsigned> addr = u2vec(address, m_Inputs);
    return TheLookupFunction(idx ,addr[0]);

  };

 private:
  /// Initialize scales, configuration parameters, alignment constants, ...
  void InitParameters();

  /// The lookup function - here the functionality of the LUT is implemented
  unsigned TheLookupFunction (int idx, unsigned eta_regional) const;

};
#endif



















