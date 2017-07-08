#ifndef  QTestParameterNames_H
#define  QTestParameterNames_H

/** \class  QTestParameterNames
 * *
 *  Defines name and number of parameters that must be specified in the 
 *  xml configuration file for each quality test besides error and warning thresholds. 
 *  It's used by QTestConfigurationPerser
 *  to check that all necessary parameters are defined.
 * 
 *  \author Ilaria Segoni
  */

#include<map>
#include<string>
#include<vector>

struct QTestParameterNames{

  public:
  	///Constructor
	QTestParameterNames();
	///Destructor
	~QTestParameterNames(){}
	///returns the list of parameters used by the test of a given type (the string theTestType 
	///must be one of the names defined in DQMServices/ClientConfig/interface/DQMQualityTestsConfiguration.h
	std::vector<std::string> getTestParamNames(const std::string& theTestType);

  private:
	void constructMap(const std::string& testType,
		const std::string& param1="undefined",const std::string& param2="undefined",const std::string& param3="undefined",
		const std::string& param4="undefined",const std::string& param5="undefined",
		const std::string& param6="undefined",const std::string& param7="undefined"
		,const std::string& param8="undefined");

  private:
	std::map<std::string, std::vector<std::string> > configurationMap;



};


#endif
