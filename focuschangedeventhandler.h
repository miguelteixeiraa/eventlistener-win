#ifndef FOCUSCHANGEDEVENTHANDLER_H
#define FOCUSCHANGEDEVENTHANDLER_H

#include <UIAnimation.h>
#include <UIAutomationClient.h>

#include <QDebug>
#include <QString>


class FocusChangedEventHandler : public IUIAutomationFocusChangedEventHandler
{
public:
    int _eventCount;
    QMap<QString, QString> *eventDetected_focusChange;
    FocusChangedEventHandler(QMap<QString, QString> &eventDetected_addr);
    void startHandler();

    //IUnknown methods.
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppInterface);

    // IUIAutomationFocusChangedEventHandler methods.
    HRESULT STDMETHODCALLTYPE HandleFocusChangedEvent(IUIAutomationElement * pSender);

private:
    LONG _refCount;
};

#endif // FOCUSCHANGEDEVENTHANDLER_H
