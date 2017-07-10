#include "Calibration/EcalTBTools/interface/TB06Reco.h"

/* 
*/

//FIXME ClassImp (TB06Reco)

void TB06Reco::reset ()
{
  run = 0 ;
  event = 0 ;
  tableIsMoving = 0 ;
  S6ADC = 0 ;

  MEXTLindex = 0 ;
  MEXTLeta = 0 ;
  MEXTLphi = 0 ;
  MEXTLenergy = 0. ;
  beamEnergy = 0. ;

  for (auto & eta : localMap)
    for (int phi = 0 ; phi<7 ; ++phi)
      eta[phi] = 0. ;

  xECAL = 0. ;
  yECAL = 0. ;
  zECAL = 0. ;
  xHodo = 0. ;
  yHodo = 0. ;
  zHodo = 0. ;
  xSlopeHodo = 0. ;
  ySlopeHodo = 0. ;
  xQualityHodo = 0. ;
  yQualityHodo = 0. ;

  convFactor = 0. ;
}
                                                                                                                                                                    
