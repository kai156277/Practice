#include "EventsWidget.h"

#include "stdafx.h"
#include "ui_EventsWidget.h"

#include <acr/common/Types.h>
#include <acr/utils/VDebug.h>
#include <algorithm>
#pragma execution_character_set("utf-8")

#define Module_Support_Attribute 0xE0001001
#define Module_Value 0xE0001002
#define Module_Support_Event 0xE0001003
#define Module_Event 0xE0001004
using namespace vsr;
using namespace vsr::acr;

EventsWidget::EventsWidget(const ModuleMeta &meta, uint16_t port, QWidget *parent)
    : QWidget(parent)
    , ModuleAgent(meta, "192.168.1.3", port, 1)
    , ui(new Ui::EventsWidget)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromStdString(meta.name));
    if (!layout())
        setLayout(new QHBoxLayout);

    mSupportAttributes = QStringList({"日期", "时间", "毫秒", "星期", "月份", "日期时间"});
    mSupportEvents     = QStringList({"闹钟", "计时器"});
    for (const auto &event : mSupportEvents)
    {
        QPushButton *button = new QPushButton(event, this);
        QObject::connect(button, &QPushButton::clicked, [this, event]() {
            qDebug() << "event:" << event;
            this->sendEvent(event);
        });
        layout()->addWidget(button);
    }
}

EventsWidget::~EventsWidget()
{
    on_mLeaveMessagebus_clicked();
    delete ui;
}

rsp_t EventsWidget::handleModuleMessage(const Message &msg)
{
    rsp_t   rsp;
    QString metaName = QString::fromStdString(name());

    switch (msg.type)
    {
    case Module_Support_Attribute:   // 获取支持的类型
    {
        QJsonArray attributes;
        for (const QString &attribute : mSupportAttributes)
        {
            attributes.push_back(metaName + "." + attribute);
        }
        QJsonDocument doc(attributes);
        rsp.code   = Ok;
        rsp.module = id();
        rsp.msg    = doc.toJson().toStdString();
        qDebug() << metaName + " - Request Attributes";
        qDebug() << "Json:" << QString::fromUtf8(doc.toJson());
        break;
    }
    case Module_Value:   // 获取支持的信息
    {
        QDateTime   current = QDateTime::currentDateTime();
        QJsonObject values {
            {mSupportAttributes[0], current.toString("yyyy/MM/dd")},
            {mSupportAttributes[1], current.toString("HH:mm:ss")},
            {mSupportAttributes[2], current.toString("zzz")},
            {mSupportAttributes[3], current.toString("dddd")},
            {mSupportAttributes[4], current.toString("MMMM")},
            {mSupportAttributes[5], current.toString("yyyy/MM/dd HH:mm:ss")},
        };

        QJsonDocument doc(values);
        rsp.code   = Ok;
        rsp.module = id();
        rsp.msg    = doc.toJson().toStdString();
        qDebug() << metaName + " - Request Values";
        qDebug() << "Json:" << QString::fromUtf8(doc.toJson());
        break;
    }
    case Module_Support_Event:   // 获取支持的事件
    {
        QJsonArray events;
        for (const QString &event : mSupportEvents)
        {
            events.push_back(metaName + "." + event);
        }
        QJsonDocument doc(events);
        rsp.code   = Ok;
        rsp.module = id();
        rsp.msg    = doc.toJson().toStdString();
        qDebug() << metaName + " - Request Events";
        qDebug() << "Json:" << QString(doc.toJson());

        module_id_t senderId = msg.sender;

        auto fIter = std::find_if(
            mEventReceivers.begin(),
            mEventReceivers.end(),
            [senderId](module_id_t id) {
                return senderId == id;
            });
        if (fIter == mEventReceivers.end())
        {
            mEventReceivers.push_back(msg.sender);
        }

        return rsp;
        break;
    }
    default:
        rsp.code   = Ok;
        rsp.module = id();
        rsp.msg    = "Ok";
        break;
    }
    return rsp;
}

void EventsWidget::onDelegateConnected()
{
    on_mJoinMessagebus_clicked();
}

void EventsWidget::sendEvent(const QString &event)
{
    Message msg;
    msg.type           = Module_Event;
    msg.sender         = id();
    QJsonArray    json = {QString::fromStdString(name()) + "." + event};
    QJsonDocument doc(json);
    msg.message = doc.toJson().toStdString();

    MessageBus        bus(*this);
    a_message_token_t token_t = bus.sendMessage(msg, mEventReceivers, false);

    err_t ret = token_t.wait_for(2000);
    if (ret == Ok)
    {
        message_token_t token = token_t.get();
        if (token == invalid_mq_token)
        {
            vdebug("send Event Failed");
            return;
        }
        else
        {
            vdebug("send Event Succeed!", msg.message);
        }
    }
}

void EventsWidget::on_mJoinMessagebus_clicked()
{
    MessageBus bus(*this);
    auto       token_t = bus.joinMessageBus();
    err_t      ret     = token_t.wait_for(2000);
    if (ret == Ok)
    {
        msgbus_member_token_t token = token_t.get();
        if (token == invalid_msgbus_member)
        {
            qDebug() << QString::fromStdString(name()) << "无效的消息总线成员" << token;
        }
    }
    else
    {
        qDebug() << "未收到 joinMessageBus() 消息的回执";
    }
    qDebug() << "Module id:" << id();
}

void EventsWidget::on_mLeaveMessagebus_clicked()
{
    MessageBus bus(*this);
    a_err_t    errt = bus.leaveMessageBus();
    err_t      ret  = errt.wait_for(2000);
    if (ret == Ok)
    {
        err_t leaveRet = errt.get();
        if (leaveRet != Ok)
            qDebug() << "leaveMessagebus 错误";
    }
    else
    {
        qDebug() << "未收到 leaveMessagebus() 消息的回执";
    }
}
