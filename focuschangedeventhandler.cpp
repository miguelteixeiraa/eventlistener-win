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
        QString res = QString::fromUtf16(reinterpret_cast<ushort*>(b)).toLocal8Bit().data();
        return res;
    };

    auto normalizeString = [](const QString &s){
        QString res = s;
        QString removeF = "F";
        for (auto n=1; n<13; n++){
            QString tmp_removeF = removeF + QString::number(n);
            if(res.contains(tmp_removeF)){
                res = res.replace(res.indexOf(tmp_removeF), res.length()-1, "");
            }
        }
        res = res.toLower();
        res = res.replace(res.indexOf("("), res.length()-1, "");
        res = res.replace(res.indexOf("..."), res.length()-1, "");
        res = res.replace(res.indexOf("ctrl"), res.length()-1, "");
        QString res_normalized = res.normalized (QString::NormalizationForm_KD);
        res_normalized.remove(QRegExp("[^a-zA-Z\\s]"));
        return res_normalized.simplified();
    };

    pSender->get_CurrentName(&eventName);

    eventDetected_focusChange->insert("EventID", "FocusChangedEvent");
    eventDetected_focusChange->insert("EventName", normalizeString(bstrToQString(eventName)));
    //qDebug() << " Received! " + normalizeString(bstrToQString(eventName));
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
