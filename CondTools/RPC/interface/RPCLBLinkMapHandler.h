#ifndef CondTools_RPC_RPCLBLinkMapHandler_h
#define CondTools_RPC_RPCLBLinkMapHandler_h

#include <string>

#include "CondCore/CondDB/interface/ConnectionPool.h"
#include "CondCore/PopCon/interface/PopConSourceHandler.h"
#include "CondFormats/Common/interface/Time.h"

#include "CondFormats/RPCObjects/interface/RPCLBLinkMap.h"

namespace edm {
class ParameterSet;
class ConfigurationDescriptions;
} // namespace edm

class RPCLBLinkMapHandler
    : public popcon::PopConSourceHandler<RPCLBLinkMap>
{
public:
    static RPCDetId getRPCDetId(int region, int disk_or_wheel, int layer, int sector
                                , const std::string& subsector_string, const std::string& partition);

public:
    RPCLBLinkMapHandler(edm::ParameterSet const & config);
    ~RPCLBLinkMapHandler();

    void getNewObjects();
    std::string id() const;

protected:
    std::string id_;
    std::string data_tag_;
    cond::Time_t since_run_;

    std::string txt_file_;

    std::string connect_;
    cond::persistency::ConnectionPool connection_;
};

#endif // CondTools_RPC_RPCLBLinkMapHandler_h
