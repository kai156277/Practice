#include "RequestModule.h"

#include <acr/module/MessageBus.h>
#include <acr/utils/VDebug.h>

#ifdef USER_STDAFX
#    include "stdafx.h"
#endif

namespace vsr {
namespace acr {

RequestModule::RequestModule(const ModuleMeta &meta, const string &ip, uint16_t port, domain_t domain)
    : ModuleAgent(meta, ip, port, domain)
    , mBus((*this))
{
}

RequestModule::~RequestModule()
{
    mBus.leaveMessageBus();
}

void RequestModule::handleMessageQueueResponse(message_token_t token, const MessageResponseTable &allRsp)
{
    vdebug("message token:", token);
    for (auto iter = allRsp.begin(); iter != allRsp.end(); ++iter)
    {
        vdebug("---------------------------");
        vdebug("module id:", iter->first);
        vdebug("module msg:", (iter->second.msg));
    }
}

void RequestModule::onDelegateConnected()
{
    //    MessageBus bus(*this);
    auto token_t = mBus.joinMessageBus();

    err_t ret = token_t.wait_for(2000);
    if (ret == Ok)
    {
        msgbus_member_token_t token = token_t.get();
        if (token == invalid_msgbus_member)
        {
            //qDebug() << "Invalid msgbus member token" << token;
            vdebug("Invalid msgbus member token", token);
            return;
        }
    }
    else
    {
        //qDebug() << "Join Message bus error";
        vdebug("Join Message bus error");
        return;
    }

    auto metaTable_t = mBus.getAllMemberInfo();
    ret              = metaTable_t.wait_for(2000);
    if (ret == Ok)
    {
        mMetaTable = metaTable_t.get();
    }
    vdebug("connected");
    sendRequestAttributes();
}

void RequestModule::sendRequestAttributes()
{
    Message msg;
    msg.type    = MessageType::UserDefined + 1;
    msg.sender  = id();
    msg.message = "";

    list<module_id_t> receivers;
    auto              iter = mMetaTable.begin();
    for (; iter != mMetaTable.end(); ++iter)
    {
        if (iter->first == id())
        {
            vdebug("Myself ID:", id());
            continue;
        }
        receivers.push_back(iter->first);
        vdebug("Receiver ID:", iter->first);
    }

    //    MessageBus        bus(*this);
    a_message_token_t token_t = mBus.sendMessage(msg, receivers, true);

    err_t ret = token_t.wait_for(2000);
    if (ret == Ok)
    {
        mRequestAttributesToken = token_t.get();
        if (mRequestAttributesToken == invalid_msg_token)
        {
            vdebug("request Module Attributes Failed");
            return;
        }
    }
}

void RequestModule::sendRequestValues()
{
    Message msg;
    msg.type    = MessageType::UserDefined + 2;
    msg.sender  = id();
    msg.message = "";

    list<module_id_t> receivers;
    auto              iter = mMetaTable.begin();
    for (; iter != mMetaTable.end(); ++iter)
    {
        if (iter->first == id())
        {
            continue;
        }
        receivers.push_back(iter->first);
    }

    //    MessageBus        bus(*this);
    a_message_token_t token_t = mBus.sendMessage(msg, receivers, true);

    err_t ret = token_t.wait_for(2000);
    if (ret == Ok)
    {
        mRequestValuesToken = token_t.get();
        if (mRequestValuesToken == invalid_msg_token)
        {
            vdebug("request Module Values Failed");
            return;
        }
        vdebug("request Module Values Token:", mRequestValuesToken);
    }
}

}   // namespace acr
}   // namespace vsr
