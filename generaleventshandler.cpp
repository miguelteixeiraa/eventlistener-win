#include "generaleventshandler.h"

GeneralEventsHandler::GeneralEventsHandler( QVariantMap &eventDetected_addr, const QString &v_eventsByAppName ): _refCount(1), _eventCount(0){
    eventDetected_general = &eventDetected_addr;
    if(v_eventsByAppName != "default"){
        this->eventsByAppName.first = true;
        this->eventsByAppName.second = v_eventsByAppName;
    }
    else{
        this->eventsByAppName.first = false;
        this->eventsByAppName.second = v_eventsByAppName;
    }
}

DWORD GeneralEventsHandler::findProcessIdByName( const QString &processName ){
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile)){
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo)){
        if (!processName.compare(processInfo.szExeFile)){
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return FALSE;
}

void GeneralEventsHandler::addEventsToIdentify( const QList<QString> &q ){
    for( auto event : q ){
        if( mUIAutoEvents->contains(event) ){
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
    if ( ret == 0 ){
        delete this;
        return 0;
    }
    return ret;
}

HRESULT STDMETHODCALLTYPE GeneralEventsHandler::QueryInterface( REFIID riid, void** ppInterface ){
    if (riid == __uuidof( IUnknown )){
        *ppInterface=static_cast<IUIAutomationEventHandler*>(this);
    }
    else if ( riid == __uuidof(IUIAutomationEventHandler) ){
        *ppInterface=static_cast<IUIAutomationEventHandler*>(this);
    }
    else{
        *ppInterface = NULL;
        return E_NOINTERFACE;
    }
    this->AddRef();
    return S_OK;
}

// IUIAutomationEventHandler methods
HRESULT STDMETHODCALLTYPE GeneralEventsHandler::HandleAutomationEvent( IUIAutomationElement * pSender, EVENTID eventID ){
    BSTR eventName;

    auto bstrToQString = []( const BSTR &b ){
        std::wstring step1(b);
        return QString::fromWCharArray(step1.c_str());
    };

    auto normalizeString = []( const QString &s ){
        auto result = s.normalized(QString::NormalizationForm_KD);
        for ( auto chr : result ){
            if( chr.unicode() > 255 ){
                result.remove(chr);
            }
        }
        return result;
    };

    if(this->eventsByAppName.first){
        long target_pid = findProcessIdByName(this->eventsByAppName.second);
        int pSender_catchedPid;

        pSender->get_CurrentProcessId(&pSender_catchedPid);

        if(pSender_catchedPid == (int)target_pid){
            qDebug() << "by event PID: " + QString::number(target_pid);
            pSender->get_CurrentName(&eventName);
            eventDetected_general->insert("EventID", mUIAutoEvents->key(eventID));
            eventDetected_general->insert("EventName", normalizeString(bstrToQString(eventName)));
            qDebug() <<  mUIAutoEvents->key(eventID) + " Received! " + normalizeString(bstrToQString(eventName));

            qDebug() << "Works!";
        }
    }
    else{
        pSender->get_CurrentName(&eventName);
        eventDetected_general->insert("EventID", mUIAutoEvents->key(eventID));
        eventDetected_general->insert("EventName", normalizeString(bstrToQString(eventName)));
        qDebug() <<  mUIAutoEvents->key(eventID) + " Received! " + normalizeString(bstrToQString(eventName));
    }

    return S_OK;
}

void GeneralEventsHandler::startHandler(){
    auto cleanup = []( auto &pAutomation_var, auto &hr_var, auto &pTargetElement_var, auto &ret_var, auto &pEHTemp_var ) {
        // Remove event handlers, release resources, and terminate
        if ( pAutomation_var != NULL ){
            hr_var = pAutomation_var->RemoveAllEventHandlers();
            if ( FAILED(hr_var) )
                ret_var = 1;
            pAutomation_var->Release();
        }

        if ( pEHTemp_var != NULL ){
            pEHTemp_var->Release();
        }

        if ( pTargetElement_var != NULL ){
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
    if( FAILED(hr) || pAutomation==NULL ){
        ret = 1;
        cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp_general);
    }

    // Use root element for listening to window and tooltip creation and destruction.
    hr = pAutomation->GetRootElement(&pTargetElement);
    if ( FAILED(hr) || pTargetElement==NULL ){
        ret = 1;
        cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp_general);
    }

    pEHTemp_general = new GeneralEventsHandler(*eventDetected_general, this->eventsByAppName.second);
    if ( pEHTemp_general == NULL ){
        ret = 1;
        cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp_general);
    }

    //qDebug() << "Adding Event Handlers.";
    for( auto event : eventsToIdentify ){
        hr = pAutomation->AddAutomationEventHandler(mUIAutoEvents->value(event), pTargetElement, TreeScope_Subtree, NULL, (IUIAutomationEventHandler*) pEHTemp_general);
        if ( FAILED(hr) ){
            ret = 1;
            cleanup(pAutomation, hr, pTargetElement, ret, pEHTemp_general);
        }
    }
}
