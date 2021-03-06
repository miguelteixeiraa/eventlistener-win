#include "focuschangedeventhandler.h"

FocusChangedEventHandler::FocusChangedEventHandler(  QVariantMap &eventDetected_addr, const QString &v_eventsByAppName ): _refCount(1), _eventCount(0){
    eventDetected_focusChange = &eventDetected_addr;
    if(v_eventsByAppName != "default"){
        this->eventsByAppName.first = true;
        this->eventsByAppName.second = v_eventsByAppName;
    }
    else{
        this->eventsByAppName.first = false;
        this->eventsByAppName.second = v_eventsByAppName;
    }
}

DWORD FocusChangedEventHandler::findProcessIdByName( const QString &processName ){
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

//IUnknown methods.
ULONG STDMETHODCALLTYPE FocusChangedEventHandler::AddRef(){
    ULONG ret = InterlockedIncrement(&_refCount);
    return ret;
}

ULONG STDMETHODCALLTYPE FocusChangedEventHandler::Release(){
    ULONG ret = InterlockedDecrement(&_refCount);
    if ( ret == 0 ){
        delete this;
        return 0;
    }
    return ret;
}

HRESULT STDMETHODCALLTYPE FocusChangedEventHandler::QueryInterface( REFIID riid, void** ppInterface ){
    if ( riid == __uuidof(IUnknown) ){
        *ppInterface = static_cast<IUIAutomationFocusChangedEventHandler*>(this);
    }
    else if ( riid == __uuidof(IUIAutomationFocusChangedEventHandler) ){
        *ppInterface = static_cast<IUIAutomationFocusChangedEventHandler*>(this);
    }
    else{
        *ppInterface = NULL;
        return E_NOINTERFACE;
    }
    this->AddRef();
    return S_OK;
}

// IUIAutomationFocusChangedEventHandler methods.
HRESULT STDMETHODCALLTYPE FocusChangedEventHandler::HandleFocusChangedEvent( IUIAutomationElement * pSender ){
    BSTR eventName;

    auto bstrToQString = []( const BSTR &b ){
        std::wstring step1(b);
        return QString::fromWCharArray(step1.c_str());
    };

    auto normalizeString = []( const QString &s ){
        auto result = s.normalized(QString::NormalizationForm_KD);
        for (auto chr : result){
            if(chr.unicode() > 255){
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
            eventDetected_focusChange->insert("EventID", "FocusChangedEvent");
            eventDetected_focusChange->insert("EventName", normalizeString(bstrToQString(eventName)));
            qDebug() << "FocusChangedEvent Received! " + normalizeString(bstrToQString(eventName));
        }
    }

    else{
        pSender->get_CurrentName(&eventName);

        eventDetected_focusChange->insert("EventID", "FocusChangedEvent");
        eventDetected_focusChange->insert("EventName", normalizeString(bstrToQString(eventName)));
        qDebug() << "FocusChangedEvent Received! " + normalizeString(bstrToQString(eventName));
    }

    return S_OK;
}

void FocusChangedEventHandler::startHandler(){
    auto cleanup = []( auto &pAutomation_var, auto &hr_var, auto &pEHTemp_var, auto &ret_var ){
        // Release resources and terminate.
        if ( pEHTemp_var != NULL ){
            pEHTemp_var->Release();
        }
        if ( pAutomation_var != NULL ){
            pAutomation_var->Release();
        }
        CoUninitialize();
        return ret_var;
    };

    HRESULT hr;
    auto ret = 0;
    FocusChangedEventHandler *pEHTemp = NULL;

    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    IUIAutomation* pAutomation = NULL;
    hr = CoCreateInstance( __uuidof(CUIAutomation), NULL,CLSCTX_INPROC_SERVER, __uuidof(IUIAutomation), (void**)&pAutomation );

    if ( FAILED(hr) || pAutomation == NULL ){
        ret = 1;
        cleanup(pAutomation, hr, pEHTemp, ret);
    }

    pEHTemp = new FocusChangedEventHandler( *eventDetected_focusChange, this->eventsByAppName.second );
    if ( pEHTemp == NULL ){
        ret = 1;
    }

    //qDebug() << "Adding Event Handlers.\n";
    hr = pAutomation->AddFocusChangedEventHandler( NULL, (IUIAutomationFocusChangedEventHandler*) pEHTemp );
    if ( FAILED(hr) ){
        ret = 1;
        cleanup(pAutomation, hr, pEHTemp, ret);
    }
}
