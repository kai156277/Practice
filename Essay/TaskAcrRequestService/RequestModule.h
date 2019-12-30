#pragma once   // REQUESTMODULE_H

#include <acr/module/ModuleAgent.h>

namespace vsr {
namespace acr {

class RequestModule : public ModuleAgent
{
public:
    RequestModule(const ModuleMeta &meta, const string &ip, uint16_t port, domain_t domain);
    ~RequestModule();
    void handleMessageQueueResponse(message_token_t token, const MessageResponseTable &allRsp) override;

    void onDelegateConnected() override;

    void sendRequestAttributes();
    void sendRequestValues();

private:
    ModuleMetaTable mMetaTable;
    MessageBus      mBus;
    message_token_t mRequestValuesToken;
    message_token_t mRequestAttributesToken;
};

}   // namespace acr
}   // namespace vsr
