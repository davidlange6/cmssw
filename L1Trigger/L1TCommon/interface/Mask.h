#ifndef L1Trigger_L1TCommon_Mask_h
#define L1Trigger_L1TCommon_Mask_h
#include <string>

//boost libraries
#include <boost/lexical_cast.hpp>
#include <utility>

namespace l1t{
	
class Mask
{
	public:
		Mask() {};
		Mask(const std::string& id, std::string procRole);
		void setProcRole(std::string procRole) { procRole_ = std::move(procRole); };
		void setPort(const std::string& id);
		std::string getProcRole() { return procRole_; };
		unsigned getPort() { return port_; };
		std::string getId() {return id_;};

	private:
		unsigned port_;
		std::string procRole_, id_;
};

}
#endif

