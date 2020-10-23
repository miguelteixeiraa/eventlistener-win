#ifndef FOCUSCHANGEDEVENTHANDLER_H
#define FOCUSCHANGEDEVENTHANDLER_H

#include <UIAnimation.h>
#include <UIAutomationClient.h>

#include <QDebug>
#include <QString>
#include <QVariantMap>


class FocusChangedEventHandler : public IUIAutomationFocusChangedEventHandler
{
public:
    int _eventCount;
    QVariantMap *eventDetected_focusChange;
    FocusChangedEventHandler( QVariantMap &eventDetected_addr );
    void startHandler();

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
