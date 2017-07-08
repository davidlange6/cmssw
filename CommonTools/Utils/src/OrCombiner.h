#ifndef CommonTools_Utils_OrCombiner_h
#define CommonTools_Utils_OrCombiner_h
/* \class reco::parser::OrCombiner
 *
 * logical OR combiner
 *
 * \author original version: Chris Jones, Cornell, 
 *         adapted by Luca Lista, INFN
 *
 * \version $Revision: 1.2 $
 *
 */
#include <utility>

#include "CommonTools/Utils/src/SelectorBase.h"
#include "CommonTools/Utils/src/SelectorPtr.h"

namespace reco {
  namespace parser {    
    struct OrCombiner : public SelectorBase {
      OrCombiner( SelectorPtr lhs, SelectorPtr rhs ) :
	lhs_( std::move(lhs) ), rhs_( std::move(rhs) ) {}
      virtual bool operator()( const edm::ObjectWithDict& o ) const {
	return (*lhs_)( o ) || (*rhs_)( o );
      }
    private:
      SelectorPtr lhs_, rhs_;
    };
  }
}

#endif
