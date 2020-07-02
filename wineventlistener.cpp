#include "wineventlistener.h"


WinEventListener::WinEventListener() : _refCount(1){}

void WinEventListener::addEventsToIdentify(const QList<QString> &list){
    for(auto event : list){
        if(mUIAutoEvents->contains(event)){
            eventsToIdentify.append(event);
        }
        else{
            std::cout << "The  event: " << event.toStdString() << " is not valid\n";
        }
    }
}

void WinEventListener::printEventCatched(){
    if(!eventCatched.empty()){
        for (auto key : eventCatched){
            std::cout << key.toStdString() << ": " << eventCatched[key].toStdString() << std::endl;
        }
    }
    else{
        qDebug() << "Nothing to print. EventCatched var is empty.";
    }
}

QMap<QString, QString>* WinEventListener::getEventCatched(){
    return &eventCatched;
}

const QMap<QString, long>* WinEventListener::getAllUIAutoMonitorableEvents(){
    return  mUIAutoEvents;
}

ULONG STDMETHODCALLTYPE WinEventListener::AddRef(){
    ULONG ret = InterlockedIncrement(&_refCount);
    return ret;
}

ULONG STDMETHODCALLTYPE WinEventListener::Release(){
    ULONG ret = InterlockedDecrement(&_refCount);
    if (ret == 0){
        delete this;
        return 0;
    }
    return ret;
}

HRESULT STDMETHODCALLTYPE WinEventListener::QueryInterface(REFIID riid, void** ppInterface)    {
    if (riid == __uuidof(IUnknown))
        *ppInterface = static_cast<IUIAutomationEventHandler*>(this);
    else if (riid == __uuidof(IUIAutomationEventHandler))
        *ppInterface = static_cast<IUIAutomationEventHandler*>(this);
    else
    {
        *ppInterface = NULL;
        return E_NOINTERFACE;
    }
    this->AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WinEventListener::HandleAutomationEvent(IUIAutomationElement* pSender, EVENTID eventID){
    static BSTR eventType, eventName, applicationName;

    auto bstrToQString = [](const BSTR &b){
        if(b == nullptr){
            return QString("");
        }
        long size = WideCharToMultiByte(CP_UTF8, 0, b, -1, nullptr, 0, nullptr, nullptr);
        if(size <= 0){
            return QString("");
        }
        char *tmp_buffer = new char[size];
        WideCharToMultiByte(CP_UTF8, 0, b, -1, tmp_buffer, size, nullptr, nullptr);
        QString  buff = tmp_buffer;
        buff = buff.toUtf8();
        return buff;
        //return QString::fromUtf16(reinterpret_cast<ushort*>(b)).toUtf8();
    };

    pSender->get_CurrentName(&eventName);
    eventCatched["Event name"] = bstrToQString(eventName);
    if(eventCatched["Event name"] == "1 nova notificação")
        qDebug() << QStringLiteral("1 nova notificação");
        //std::cout << QStringLiteral("1 nova notificação") << std::endl;

    return S_OK;
}

void WinEventListener::listenerStart(){
    auto cleanup = [](auto &pAutomation_var, auto &hr_var, auto &pTargetElement_var, auto &ret_var, auto &pEHTemp_var) {
        // Remove event handlers, release resources, and terminate
        if (pAutomation_var != NULL)
        {
            hr_var = pAutomation_var->RemoveAllEventHandlers();
            if (FAILED(hr_var))
                ret_var = 1;
            pAutomation_var->Release();
        }

        if (pEHTemp_var != NULL)
            pEHTemp_var->Release();

        if (pTargetElement_var != NULL)
            pTargetElement_var->Release();

        CoUninitialize();
        return ret_var;
    };

    HRESULT hr;
    int ret = 0;
    IUIAutomationElement* pTargetElement = NULL;
    WinEventListener* pEHTemp = NULL;

    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    IUIAutomation* pAutomation = NULL;
    hr = CoCreateInstance(__uuidof(CUIAutomation), NULL, CLSCTX_INPROC_SERVER, __uuidof(IUIAutomation), (void**)&pAutomation);
    if (FAILED(hr) || pAutomation == NULL){
        ret = 1;
        cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp);
    }

    // Use root element for listening to window and tooltip creation and destruction.
    hr = pAutomation->GetRootElement(&pTargetElement);
    if (FAILED(hr) || pTargetElement == NULL){
        ret = 1;
        cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp);
    }

    pEHTemp = new WinEventListener();
    if (pEHTemp == NULL){
        ret = 1;
        cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp);
    }

    qDebug() << "-Adding Event Handlers.\n";
    if(!eventsToIdentify.empty()){
        for(auto event : eventsToIdentify){
            hr = pAutomation->AddAutomationEventHandler(mUIAutoEvents->value(event), pTargetElement, TreeScope_Subtree, NULL, (IUIAutomationEventHandler*)pEHTemp);
            if(FAILED(hr)){
                ret = 1;
                cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp);
            }
        }
    }

    qDebug() << "-Press any key to remove event handlers and exit\n";
    getchar();

    qDebug() << "-Removing Event Handlers.\n";
}




