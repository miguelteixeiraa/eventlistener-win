#ifndef GENERALEVENTSHANDLER_H
#define GENERALEVENTSHANDLER_H

#include <UIAnimation.h>
#include <UIAutomationClient.h>
#include <tlhelp32.h>

#include <QList>
#include <QString>
#include <QVariantMap>
#include <QPair>

#include <QDebug>

#include <MonitorableUIAutoEvents.h>


class GeneralEventsHandler : public IUIAutomationEventHandler
{
public:
    GeneralEventsHandler( QVariantMap &eventDetected_addr, const QString &v_eventsByAppName );
    void addEventsToIdentify( const QList<QString> &q );
    DWORD findProcessIdByName( const QString &processName );
    void startHandler();

    const QMap<QString, long>* mUIAutoEvents = &uiAutoMonitorableEvents;

    QList<QString> eventsToIdentify;
    QVariantMap *eventDetected_general;
    QPair<bool, QString> eventsByAppName; // ex.: true -> process_name
    int _eventCount;

    // IUnknown methods.
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, void** ppInterface );

    // IUIAutomationEventHandler methods
    HRESULT STDMETHODCALLTYPE HandleAutomationEvent( IUIAutomationElement * pSender, EVENTID eventID );

private:
    LONG _refCount;
};

#endif // GENERALEVENTSHANDLER_H
