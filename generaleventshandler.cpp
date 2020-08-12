#include "generaleventshandler.h"

GeneralEventsHandler::GeneralEventsHandler(QMap<QString, QString> &eventDetected_addr) : _refCount(1), _eventCount(0)
{
    eventDetected_general = &eventDetected_addr;
}

void GeneralEventsHandler::addEventsToIdentify(const QList<QString> &q){
    for(auto event : q){
        if(mUIAutoEvents->contains(event)){
            eventsToIdentify.append(event);
        }
    }
}

// IUnknown methods.
ULONG STDMETHODCALLTYPE GeneralEventsHandler::AddRef(){
    ULONG ret = InterlockedIncrement(&_refCount);
    return ret;
}

ULONG STDMETHODCALLTYPE GeneralEventsHandler::Release(){
    ULONG ret = InterlockedDecrement(&_refCount);
    if (ret == 0) {
        delete this;
        return 0;
    }
    return ret;
}

HRESULT STDMETHODCALLTYPE GeneralEventsHandler::QueryInterface(REFIID riid, void** ppInterface){
    if (riid == __uuidof(IUnknown))
        *ppInterface=static_cast<IUIAutomationEventHandler*>(this);
    else if (riid == __uuidof(IUIAutomationEventHandler))
        *ppInterface=static_cast<IUIAutomationEventHandler*>(this);
    else
    {
        *ppInterface = NULL;
        return E_NOINTERFACE;
    }
    this->AddRef();
    return S_OK;
}

// IUIAutomationEventHandler methods
HRESULT STDMETHODCALLTYPE GeneralEventsHandler::HandleAutomationEvent(IUIAutomationElement * pSender, EVENTID eventID){
    BSTR eventName;
    QString eventID_qstr;
    auto bstrToQString = [](const BSTR &b){
        QString tmp = QString::fromUtf16(reinterpret_cast<ushort*>(b)).toUtf8();
        return tmp.simplified();
    };

    pSender->get_CurrentName(&eventName);
    for(auto event : mUIAutoEvents->keys()){
        if(mUIAutoEvents->value(event) == eventID){
            eventDetected_general->insert("EventID", event);
            eventDetected_general->insert("EventName", bstrToQString(eventName));
            //qDebug() << event + " Received! " + bstrToQString(eventName);
        }
    }

    return S_OK;
}

void GeneralEventsHandler::startHandler(){
    auto cleanup = [](auto &pAutomation_var, auto &hr_var, auto &pTargetElement_var, auto &ret_var, auto &pEHTemp_var) {
        // Remove event handlers, release resources, and terminate
        if (pAutomation_var != NULL){
            hr_var = pAutomation_var->RemoveAllEventHandlers();
            if (FAILED(hr_var))
                ret_var = 1;
            pAutomation_var->Release();
        }

        if (pEHTemp_var != NULL){
            pEHTemp_var->Release();
        }

        if (pTargetElement_var != NULL){
            pTargetElement_var->Release();
        }

        CoUninitialize();
        return ret_var;
    };

    HRESULT hr;
    int ret = 0;
    IUIAutomationElement* pTargetElement = NULL;
    GeneralEventsHandler* pEHTemp_general = NULL;

    CoInitializeEx(NULL,COINIT_MULTITHREADED);
    IUIAutomation* pAutomation = NULL;
    hr = CoCreateInstance(__uuidof(CUIAutomation), NULL,CLSCTX_INPROC_SERVER, __uuidof(IUIAutomation), (void**)&pAutomation);
    if(FAILED(hr) || pAutomation==NULL){
        ret = 1;
        cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp_general);
    }

    // Use root element for listening to window and tooltip creation and destruction.
    hr = pAutomation->GetRootElement(&pTargetElement);
    if (FAILED(hr) || pTargetElement==NULL){
        ret = 1;
        cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp_general);
    }

    pEHTemp_general = new GeneralEventsHandler(*eventDetected_general);
    if (pEHTemp_general == NULL){
        ret = 1;
        cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp_general);
    }

    qDebug() << "Adding Event Handlers.";
    for(auto event : eventsToIdentify){
        hr = pAutomation->AddAutomationEventHandler(mUIAutoEvents->value(event), pTargetElement, TreeScope_Subtree, NULL, (IUIAutomationEventHandler*) pEHTemp_general);
        if (FAILED(hr)){
            ret = 1;
            cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp_general);
        }
    }

    qDebug() << "Press any key to remove event handlers and exit\n";
    getchar();

    qDebug() << "Removing Event Handlers.\n";
}
