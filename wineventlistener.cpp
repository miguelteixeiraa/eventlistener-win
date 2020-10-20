#include "wineventlistener.h"

// start FocusChangedEventWorker
FocusChangedEventWorker::FocusChangedEventWorker(QVariantMap &worker_eventDetected){
    focusChangedEventHandler = new FocusChangedEventHandler(worker_eventDetected);
}

void FocusChangedEventWorker::run(){
    focusChangedEventHandler->startHandler();
}
// end FocusChangedEventWorker

// start GeneralEventsWorker
GeneralEventsWorker::GeneralEventsWorker(QVariantMap &worker_eventDetected){
    generalEventsHandler = new GeneralEventsHandler(worker_eventDetected);
}

void GeneralEventsWorker::addEventsToIdentify(QList<QString> &w_eventsToIdentify){
    generalEventsHandler->addEventsToIdentify(w_eventsToIdentify);
}

void GeneralEventsWorker::run(){
    generalEventsHandler->startHandler();
}
// end GeneralEventsWorker

// start WinEventListener
WinEventListener::WinEventListener(){
}

void WinEventListener::addEventsToIdentify(const QList<QString> &list){
    QList<QString> *generalEvents = new QList<QString>;
    for(auto eventID : list){
        if(!nonGeneralEventIDs.contains(eventID)){
            generalEvents->append(eventID);
        }
    }
    w_generalEvents = new GeneralEventsWorker(*eventDetected);
    w_generalEvents->addEventsToIdentify(*generalEvents);
    if(list.contains("UIA_AutomationFocusChangedEventId")){
        w_focusChangedEvent = new FocusChangedEventWorker(*eventDetected);
    }  
}

QVariantMap WinEventListener::getEventDetected(){
    return *eventDetected;
}

void WinEventListener::listenerStart(){
    if(w_generalEvents != NULL){
        //
        qDebug() << "I'm here";
    }
    if(w_focusChangedEvent != NULL){

        qDebug() << "I'm here";
    }
    QThreadPool::globalInstance()->start(w_focusChangedEvent);
    QThread::sleep(5);
    QThreadPool::globalInstance()->start(w_generalEvents);
}
// end WinEventListener
