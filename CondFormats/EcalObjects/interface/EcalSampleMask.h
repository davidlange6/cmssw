#ifndef CondFormats_EcalObjects_EcalSampleMask_H
#define CondFormats_EcalObjects_EcalSampleMask_H
/**
 * Author: Giovanni Franzoni, UMN
 * Created: 09 Apr 2012
 * $Id: EcalSampleMask.h,v 1.1 2012/05/10 08:22:10 argiro Exp $
 **/

#include "CondFormats/Serialization/interface/Serializable.h"

#include <iostream>
#include <vector>
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

class EcalSampleMask {
  public:
    EcalSampleMask();

    // construct from pre-organized binary words 
    EcalSampleMask(unsigned int ebmask, unsigned int eemask);
    // constructor from an ordered set of switches, one per sample 
    EcalSampleMask( const std::vector<unsigned int> &ebmask, const std::vector<unsigned int> &eemask);

    ~EcalSampleMask();

    void setEcalSampleMaskRecordEB( const unsigned int mask ) { sampleMaskEB_ = mask; }
    void setEcalSampleMaskRecordEE( const unsigned int mask ) { sampleMaskEB_ = mask; }
    void setEcalSampleMaskRecordEB( const std::vector<unsigned int> & ebmask );
    void setEcalSampleMaskRecordEE( const std::vector<unsigned int> & eemask );
    
    float getEcalSampleMaskRecordEB() const { return sampleMaskEB_; }
    float getEcalSampleMaskRecordEE() const { return sampleMaskEE_; }
    void print(std::ostream& s) const {
      s << "EcalSampleMask: EB " << sampleMaskEB_ << "; EE " << sampleMaskEE_ ;
    }

    bool useSampleEB  (int sampleId) const ;
    bool useSampleEE  (int sampleId) const ;
    bool useSample    (int sampleId, DetId &theCrystal) const;

  private:
    unsigned int sampleMaskEB_;
    unsigned int sampleMaskEE_;


  COND_SERIALIZABLE;
};


#endif
