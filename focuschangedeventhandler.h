#ifndef FOCUSCHANGEDEVENTHANDLER_H
#define FOCUSCHANGEDEVENTHANDLER_H

#include <UIAnimation.h>
#include <UIAutomationClient.h>
#include <tlhelp32.h>
#include <string>

#include <QDebug>
#include <QString>
#include <QVariantMap>
#include <QPair>



class FocusChangedEventHandler : public IUIAutomationFocusChangedEventHandler
{
public:
    FocusChangedEventHandler( QVariantMap &eventDetected_addr, const QString &v_eventsByAppName );
    DWORD findProcessIdByName( const QString &processName );
    void startHandler();

    int _eventCount;    
    QVariantMap *eventDetected_focusChange;
    QPair<bool, QString> eventsByAppName; // ex.: true -> process_name

    //IUnknown methods.
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, void** ppInterface );

    // IUIAutomationFocusChangedEventHandler methods.
    HRESULT STDMETHODCALLTYPE HandleFocusChangedEvent( IUIAutomationElement * pSender );

private:
    LONG _refCount;
};

#endif // FOCUSCHANGEDEVENTHANDLER_H
