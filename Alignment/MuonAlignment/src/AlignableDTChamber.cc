/** \file
 *
 *  $Date: 2008/03/26 21:59:25 $
 *  $Revision: 1.10 $
 *  \author Andre Sznajder - UERJ(Brazil)
 */
 
#include "Alignment/MuonAlignment/interface/AlignableDTChamber.h"
#include "Alignment/MuonAlignment/interface/AlignableDTSuperLayer.h"

AlignableDTChamber::AlignableDTChamber(const GeomDet *geomDet)
   : AlignableDet(geomDet, false)
{
   // even though we overload alignableObjectId(), it's dangerous to
   // have two different claims about the structure type
   theStructureType = align::AlignableDTChamber;

   // The unique thing about DT chambers is that they are Dets that contain Dets (superlayers)
   // The superlayer Dets contain DetUnits (layers), as usual
   const std::vector<const GeomDet*>& geomDets = geomDet->components();
   for (auto geomDet : geomDets) {
      addComponent(new AlignableDTSuperLayer(geomDet));
   }

   // DO NOT let the chamber position become an average of the layers
   this->theSurface = geomDet->surface();
}

/// Printout the DetUnits in the DT chamber
std::ostream& operator<< (std::ostream &os, const AlignableDTChamber & r) {
   std::vector<Alignable*> theDets = r.components();

   os << "    This DTChamber contains " << theDets.size() << " units" << std::endl ;
   os << "    position = " << r.globalPosition() << std::endl;
   os << "    (phi, r, z)= (" << r.globalPosition().phi() << "," << r.globalPosition().perp() << "," << r.globalPosition().z();
   os << "), orientation:" << std::endl<< r.globalRotation() << std::endl;
   
   os << "    total displacement and rotation: " << r.displacement() << std::endl;
   os << r.rotation() << std::endl;
 
   for (std::vector<Alignable*>::const_iterator idet = theDets.begin();  idet != theDets.end();  ++idet) {
      const align::Alignables& comp = (*idet)->components();

      for (auto i : comp) {
	 os << "     Det position, phi, r: " 
	    << i->globalPosition() << " , "
	    << i->globalPosition().phi() << " , "
	    << i->globalPosition().perp() << std::endl; 
	 os << "     local  position, phi, r: " 
	    << r.surface().toLocal(i->globalPosition())        << " , "
	    << r.surface().toLocal(i->globalPosition()).phi()  << " , "
	    << r.surface().toLocal(i->globalPosition()).perp() << std::endl; 
      }
   }

   return os;
}
