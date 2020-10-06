#ifndef GENERALEVENTSHANDLER_H
#define GENERALEVENTSHANDLER_H

#include <UIAnimation.h>
#include <UIAutomationClient.h>

#include <QList>
#include <QString>
#include <QDebug>
#include <QVariantMap>

#include <eventlistener-win/MonitorableUIAutoEvents.h>


class GeneralEventsHandler : public IUIAutomationEventHandler
{
public:
    GeneralEventsHandler(QVariantMap &eventDetected_addr);
    void addEventsToIdentify(const QList<QString> &q);
    void startHandler();

    const QMap<QString, long>* mUIAutoEvents = &uiAutoMonitorableEvents;

    QList<QString> eventsToIdentify;
    QVariantMap *eventDetected_general;
    int _eventCount;

    // IUnknown methods.
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppInterface);

    // IUIAutomationEventHandler methods
    HRESULT STDMETHODCALLTYPE HandleAutomationEvent(IUIAutomationElement * pSender, EVENTID eventID);

private:
    LONG _refCount;
};

#endif // GENERALEVENTSHANDLER_H
