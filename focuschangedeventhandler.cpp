#include "focuschangedeventhandler.h"

FocusChangedEventHandler::FocusChangedEventHandler(QVariantMap &eventDetected_addr): _refCount(1), _eventCount(0){
    eventDetected_focusChange = &eventDetected_addr;
}

//IUnknown methods.
ULONG STDMETHODCALLTYPE FocusChangedEventHandler::AddRef(){
    ULONG ret = InterlockedIncrement(&_refCount);
    return ret;
}

ULONG STDMETHODCALLTYPE FocusChangedEventHandler::Release(){
    ULONG ret = InterlockedDecrement(&_refCount);
    if (ret == 0){
        delete this;
        return 0;
    }
    return ret;
}

HRESULT STDMETHODCALLTYPE FocusChangedEventHandler::QueryInterface(REFIID riid, void** ppInterface){
    if (riid == __uuidof(IUnknown)){
        *ppInterface = static_cast<IUIAutomationFocusChangedEventHandler*>(this);
    }
    else if (riid == __uuidof(IUIAutomationFocusChangedEventHandler)){
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
HRESULT STDMETHODCALLTYPE FocusChangedEventHandler::HandleFocusChangedEvent(IUIAutomationElement * pSender){
    BSTR eventName;
    auto bstrToQString = [](const BSTR &b){
        QString tmp_result = QString::fromUtf16(reinterpret_cast<ushort*>(b)).toLocal8Bit().data();
        tmp_result.replace(tmp_result.indexOf("("), tmp_result.length()-1);
        QString removeF = "F";
        for (auto n=1; n<13; n++){
            QString tmp_removeF = removeF + QString::number(n);
            if(tmp_result.contains(tmp_removeF)){
                tmp_result.replace(tmp_result.indexOf(tmp_removeF), tmp_result.length()-1);
            }
        }
        if(tmp_result.contains("(")){
            tmp_result.replace(tmp_result.indexOf("("), tmp_result.length()-1);
        }
        tmp_result = tmp_result.toLower();
        qDebug() << tmp_result.simplified();
        return tmp_result.simplified();
    };

    pSender->get_CurrentName(&eventName);

    eventDetected_focusChange->insert("EventID", "FocusChangedEvent");
    eventDetected_focusChange->insert("EventName", bstrToQString(eventName));

    return S_OK;
}

void FocusChangedEventHandler::startHandler(){
    auto cleanup = [](auto &pAutomation_var, auto &hr_var, auto &pEHTemp_var, auto &ret_var){
        // Release resources and terminate.
        if (pEHTemp_var != NULL){
            pEHTemp_var->Release();
        }
        if (pAutomation_var != NULL){
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
    hr = CoCreateInstance(__uuidof(CUIAutomation), NULL,CLSCTX_INPROC_SERVER, __uuidof(IUIAutomation), (void**)&pAutomation);

    if (FAILED(hr) || pAutomation == NULL){
        ret = 1;
        cleanup(pAutomation, hr, pEHTemp, ret);
    }

    pEHTemp = new FocusChangedEventHandler(*eventDetected_focusChange);
    if (pEHTemp == NULL){
        ret = 1;
    }

    qDebug() << "Adding Event Handlers.\n";
    hr = pAutomation->AddFocusChangedEventHandler(NULL, (IUIAutomationFocusChangedEventHandler*) pEHTemp);
    if (FAILED(hr)){
        ret = 1;
        cleanup(pAutomation, hr, pEHTemp, ret);
    }
    qDebug() << "Press any key to remove event handler and exit";
    getchar();

    qDebug() << "Removing Event Handlers.\n";
    hr = pAutomation->RemoveFocusChangedEventHandler((IUIAutomationFocusChangedEventHandler*) pEHTemp);
    if (FAILED(hr)){
        ret = 1;
        cleanup(pAutomation, hr, pEHTemp, ret);
    }
}
