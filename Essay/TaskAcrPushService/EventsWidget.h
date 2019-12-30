#ifndef EVENTSWIDGET_H
#define EVENTSWIDGET_H

#include <QPushButton>
#include <QWidget>
#include <acr/module/ModuleAgent.h>
namespace Ui {
class EventsWidget;
}

namespace vsr {
namespace acr {

class EventsWidget
    : public QWidget
    , public ModuleAgent
{
    Q_OBJECT

public:
    explicit EventsWidget(const ModuleMeta &meta, uint16_t port, QWidget *parent = 0);
    ~EventsWidget();

    rsp_t handleModuleMessage(const Message &msg) override;
    void  onDelegateConnected() override;

    void sendEvent(const QString &event);

private slots:

    void on_mJoinMessagebus_clicked();

    void on_mLeaveMessagebus_clicked();

private:
    Ui::EventsWidget *ui;
    list<module_id_t> mEventReceivers;
    QStringList       mSupportAttributes;
    QStringList       mSupportEvents;
};

}   // namespace acr
}   // namespace vsr

#endif   // EVENTSWIDGET_H
