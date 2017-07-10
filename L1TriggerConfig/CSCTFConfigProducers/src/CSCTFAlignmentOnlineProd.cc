#include "L1TriggerConfig/CSCTFConfigProducers/interface/CSCTFAlignmentOnlineProd.h"

std::shared_ptr< L1MuCSCTFAlignment >
CSCTFAlignmentOnlineProd::newObject( const std::string& objectKey )
{
   // Execute SQL queries to get data from OMDS (using key) and make C++ object
  
     //  SELECT Multiple columns  FROM TABLE with correct key: 
     std::vector< std::string > columns ;
     columns.emplace_back("ALIPAR1" ) ;
     columns.emplace_back("ALIPAR2" ) ;
     columns.emplace_back("ALIPAR3" ) ;
     columns.emplace_back("ALIPAR4" ) ;
     columns.emplace_back("ALIPAR5" ) ;
     columns.emplace_back("ALIPAR6" ) ;
     columns.emplace_back("ALIPAR7" ) ;
     columns.emplace_back("ALIPAR8" ) ;
     columns.emplace_back("ALIPAR9" ) ;
     columns.emplace_back("ALIPAR10" ) ;
     columns.emplace_back("ALIPAR11" ) ;
     columns.emplace_back("ALIPAR12" ) ;
     columns.emplace_back("ALIPAR13" ) ;
     columns.emplace_back("ALIPAR14" ) ;
     columns.emplace_back("ALIPAR15" ) ;
     columns.emplace_back("ALIPAR16" ) ;

     l1t::OMDSReader::QueryResults results = m_omdsReader.basicQuery(
	  columns,
         "CMS_CSC_TF",
	  "CSCTF_ALIGN_PARAM",
	  "CSCTF_ALIGN_PARAM.ALIGN_KEY",
	  m_omdsReader.singleAttribute( objectKey )
	  ) ;

   if( results.queryFailed() ) // check if query was successful
   {
      edm::LogError( "L1-O2O" ) << "Problem with CSCTF_ALIGN_PARAM query." ;
      return std::make_shared<L1MuCSCTFAlignment>() ;
   }

// oracle doesn't support double so some tweaks
//    float datum ;
   std::vector<float>  par_align;
   std::vector<double>  par_align_double;
   results.fillVariable( "ALIPAR1", par_align[0] ) ;
   results.fillVariable( "ALIPAR2", par_align[1] ) ;
   results.fillVariable( "ALIPAR3", par_align[2] ) ;
   results.fillVariable( "ALIPAR4", par_align[3] ) ;
   results.fillVariable( "ALIPAR5", par_align[4] ) ;
   results.fillVariable( "ALIPAR6", par_align[5] ) ;
   results.fillVariable( "ALIPAR7", par_align[6] ) ;
   results.fillVariable( "ALIPAR8", par_align[7] ) ;
   results.fillVariable( "ALIPAR9", par_align[8] ) ;
   results.fillVariable( "ALIPAR10", par_align[9] ) ;
   results.fillVariable( "ALIPAR11", par_align[10] ) ;
   results.fillVariable( "ALIPAR12", par_align[11] ) ;
   results.fillVariable( "ALIPAR13", par_align[12] ) ;
   results.fillVariable( "ALIPAR14", par_align[13] ) ;
   results.fillVariable( "ALIPAR15", par_align[14] ) ;
   results.fillVariable( "ALIPAR16", par_align[15] ) ;



//   std::cout<<conf_stat<<std::endl;
   for (int i=0;i<16;i++) {
	edm::LogInfo( "algn_par queried" ) << par_align[i] ;
	par_align_double[i]=par_align[i] ;
   }
   return std::make_shared<L1MuCSCTFAlignment>(par_align_double) ;
}


