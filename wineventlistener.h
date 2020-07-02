#ifndef WINEVENTLISTENER_H
#define WINEVENTLISTENER_H

#include <QObject>
#include <QDebug>
#include <QMap>
#include <QList>
#include <iostream>
#include <windows.h>
#include <QString>

#include <UIAutomation.h>

#include "MonitorableUIAutoEvents.h"



class WinEventListener : public IUIAutomationEventHandler
{
    //Q_OBJECT
    const QMap<QString, long>* mUIAutoEvents = &uiAutoMonitorableEvents;
    QList<QString> eventsToIdentify;
    QMap<QString, QString> eventCatched{
        {"Event type", ""},
        {"Event name", ""},
        {"Application name", ""}
    };

public:

    //explicit WinEventListener(QObject *parent = nullptr);
    explicit WinEventListener();

    void addEventsToIdentify(const QList<QString> &list);

    const QMap<QString, long>* getAllUIAutoMonitorableEvents();

    QMap<QString, QString>* getEventCatched();

    void printEventCatched();

    // Start UIAutomation stuff to event listener to work
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppInterface);
    HRESULT STDMETHODCALLTYPE HandleAutomationEvent(IUIAutomationElement* pSender, EVENTID eventID);
    ULONG STDMETHODCALLTYPE AddRef();
    // End UIAutomation stuff to event listener to work

    void listenerStart();

private:
    LONG _refCount;


//signals:

};

#endif // WINEVENTLISTENER_H
