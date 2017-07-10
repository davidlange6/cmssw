#include "FWPFEcalRecHitLegoProxyBuilder.h"

//______________________________________________________________________________
void
FWPFEcalRecHitLegoProxyBuilder::scaleProduct( TEveElementList *parent, FWViewType::EType type, const FWViewContext *vc )
{
   FWViewEnergyScale *caloScale = vc->getEnergyScale();
   bool b = caloScale->getPlotEt();
   float maxVal = getMaxVal( b );
   typedef std::vector<FWPFLegoRecHit*> rh;

   // printf("FWPFEcalRecHitLegoProxyBuilder::scaleProduct >> scale %f \n", caloScale->getValToHeight());
   for(auto & m_recHit : m_recHits)
   {  // Tallest tower needs deciding still
      if( m_recHit->isTallest() == false && m_recHit->getEtEnergy( b ) == maxVal )
         m_recHit->setIsTallest( true );

      m_recHit->updateScale( vc, getMaxValLog(caloScale->getPlotEt()));
   }
}

//______________________________________________________________________________
void
FWPFEcalRecHitLegoProxyBuilder::localModelChanges( const FWModelId &iId, TEveElement *parent,
                                                   FWViewType::EType viewType, const FWViewContext *vc )
{
   for (TEveElement::List_i i = parent->BeginChildren(); i!= parent->EndChildren(); ++i)
   {
      {
         TEveStraightLineSet* line = dynamic_cast<TEveStraightLineSet*>(*i);
         if (line)
         {
            const FWDisplayProperties &p = item()->modelInfo( iId.index() ).displayProperties();
            line->SetMarkerColor( p.color() );
         }
      }
   }
}

//______________________________________________________________________________
TEveVector
FWPFEcalRecHitLegoProxyBuilder::calculateCentre( const std::vector<TEveVector> &corners ) const
{
   TEveVector centre;

   for(auto corner : corners)
   {
      centre.fX += corner.fX;
      centre.fY += corner.fY;            // Get total for x,y,z values
      centre.fZ += corner.fZ;
   }
   centre *= 1.f / 8.f;

   return centre;   
}

//______________________________________________________________________________
void
FWPFEcalRecHitLegoProxyBuilder::build( const FWEventItem *iItem, TEveElementList *product, const FWViewContext *vc )
{
   size_t itemSize = iItem->size(); //cache size

   for( size_t index = 0; index < itemSize; ++index )
   {
      TEveCompound *itemHolder = createCompound();
      product->AddElement( itemHolder );

      const EcalRecHit &iData = modelData( index );
      const float *corners = item()->getGeom()->getCorners( iData.detid() );
      float energy, et;
      std::vector<TEveVector> etaphiCorners(8);
      TEveVector centre;

      if( corners == 0 )
         continue;

      int k = 3;
      for( int i = 0; i < 4; ++i )
      {
         int j = k * 3;
         TEveVector cv = TEveVector( corners[j], corners[j+1], corners[j+2] );
         etaphiCorners[i].fX = cv.Eta();                                     // Conversion of rechit X/Y values for plotting in Eta/Phi
         etaphiCorners[i].fY = cv.Phi();
         etaphiCorners[i].fZ = 0.0;

         etaphiCorners[i+4].fX = etaphiCorners[i].fX;                        // Top can simply be plotted exactly over the top of the bottom face
         etaphiCorners[i+4].fY = etaphiCorners[i].fY;
         etaphiCorners[i+4].fZ = 0.001;
         // printf("%f %f %d \n",  etaphiCorners[i].fX, etaphiCorners[i].fY, i);
         --k;
      }

      centre = calculateCentre( etaphiCorners );
      energy = iData.energy();
      et = FWPFMaths::calculateEt( centre, energy );
      context().voteMaxEtAndEnergy( et, energy );

      if( energy > m_maxEnergy )
         m_maxEnergy = energy;
      if( energy > m_maxEt )
         m_maxEt = et;

      // Stop phi wrap
      float dPhi1 = etaphiCorners[2].fY - etaphiCorners[1].fY;
      float dPhi2 = etaphiCorners[3].fY - etaphiCorners[0].fY;
      float dPhi3 = etaphiCorners[1].fY - etaphiCorners[2].fY;
      float dPhi4 = etaphiCorners[0].fY - etaphiCorners[3].fY;

      if( dPhi1 > 1 )
         etaphiCorners[2].fY = etaphiCorners[2].fY - ( 2 * TMath::Pi() );
      if( dPhi2 > 1 )
         etaphiCorners[3].fY = etaphiCorners[3].fY - ( 2 * TMath::Pi() );
      if( dPhi3 > 1 )
         etaphiCorners[2].fY = etaphiCorners[2].fY + ( 2 * TMath::Pi() );
      if( dPhi4 > 1 )
         etaphiCorners[3].fY = etaphiCorners[3].fY + ( 2 * TMath::Pi() );

      FWPFLegoRecHit *recHit = new FWPFLegoRecHit( etaphiCorners, itemHolder, this, vc, energy, et );
      recHit->setSquareColor( item()->defaultDisplayProperties().color() );
      m_recHits.push_back( recHit );
   }

      m_maxEnergyLog = log( m_maxEnergy );
      m_maxEtLog = log( m_maxEt );

      scaleProduct( product, FWViewType::kLegoPFECAL, vc );
}

//______________________________________________________________________________
void
FWPFEcalRecHitLegoProxyBuilder::cleanLocal()
{
   for(auto & m_recHit : m_recHits)
      delete m_recHit;

   m_recHits.clear();
}

//______________________________________________________________________________
REGISTER_FWPROXYBUILDER( FWPFEcalRecHitLegoProxyBuilder, EcalRecHit, "PF Ecal RecHit", FWViewType::kLegoPFECALBit );
