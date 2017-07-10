#ifndef CaloOnlineTools_HcalOnlineDb_HcalChannelIterator_h
#define CaloOnlineTools_HcalOnlineDb_HcalChannelIterator_h
// -*- C++ -*-
//
// Package:     HcalOnlineDb
// Class  :     HcalChannelIterator
// 
/**\class HcalChannelIterator HcalChannelIterator.h CaloOnlineTools/HcalOnlineDb/interface/HcalChannelIterator.h

 Description: Iterators over HCAL channels using various sources

 Usage:
    <usage>

*/
//
// Original Author:  Gena Kukartsev
//         Created:  Fri Jul 10 16:59:15 CEST 2009
//

#include <iostream>
#include <string>
#include <vector>
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"


class HcalChannelIterator
{

   public:
      HcalChannelIterator();
      virtual ~HcalChannelIterator();

      int clearChannelList(); 
      int size(); 
      int addListFromLmapAscii(std::string filename); 
      int initHBEFListFromLmapAscii();
      int init(const std::vector<HcalGenericDetId>& map);

      //
      //_____iterator methods __________________________
      //
      int begin();
      int next();
      bool end();
      HcalGenericDetId getHcalGenericDetId();
      HcalSubdetector getHcalSubdetector();
      int getIeta();
      int getIphi();
      int getDepth();

   private:

      std::vector<HcalGenericDetId> channel_list;
      std::vector<HcalGenericDetId>::const_iterator const_iterator;

};


#endif
