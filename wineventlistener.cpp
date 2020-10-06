#include "wineventlistener.h"

// start FocusChangedEventWorker
FocusChangedEventWorker::FocusChangedEventWorker(QVariantMap &worker_eventDetected){
    focusChangedEventHandler = new FocusChangedEventHandler(worker_eventDetected);
}

void FocusChangedEventWorker::doWork(){
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

void GeneralEventsWorker::doWork(){
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
        QThread *t_generalEventsWorker = new QThread;
        w_generalEvents->moveToThread(t_generalEventsWorker);
        connect(t_generalEventsWorker, SIGNAL(started()), w_generalEvents, SLOT(doWork()));
        t_generalEventsWorker->start();
    }
    if(w_focusChangedEvent != NULL){
        QThread *t_focusChangedEventWorker = new QThread;
        w_focusChangedEvent->moveToThread(t_focusChangedEventWorker);
        connect(t_focusChangedEventWorker, SIGNAL(started()), w_focusChangedEvent, SLOT(doWork()));
        t_focusChangedEventWorker->start();
    }
}
// end WinEventListener
