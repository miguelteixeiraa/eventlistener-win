#include "wineventlistener.h"



FocusChangedEventWorker::FocusChangedEventWorker(QMap<QString, QString> &worker_eventDetected){
    focusChangedEventHandler = new FocusChangedEventHandler(worker_eventDetected);
}

void FocusChangedEventWorker::doWork(){
    focusChangedEventHandler->startHandler();
}


GeneralEventsWorker::GeneralEventsWorker(QMap<QString, QString> &worker_eventDetected){
    generalEventsHandler = new GeneralEventsHandler(worker_eventDetected);
}

void GeneralEventsWorker::addEventsToIdentify(QList<QString> &w_eventsToIdentify){
    generalEventsHandler->addEventsToIdentify(w_eventsToIdentify);
}

void GeneralEventsWorker::doWork(){
    generalEventsHandler->startHandler();
}



WinEventListener::WinEventListener()
{
}

void WinEventListener::addEventsToIdentify(const QList<QString> &list){
    QList<QString> *generalEvents = new QList<QString>;
    if(list.contains("UIA_AutomationFocusChangedEventId")){
        w_focusChangedEvent = new FocusChangedEventWorker(*eventDetected);
    }
    for(auto eventID : list){
        if(!nonGeneralEventIDs.contains(eventID)){
            generalEvents->append(eventID);
        }
    }
    w_generalEvents = new GeneralEventsWorker(*eventDetected);
    w_generalEvents->addEventsToIdentify(*generalEvents);
}

void WinEventListener::listenerStart(){
    if(w_focusChangedEvent != NULL){
        QThread *t_focusChangedEventWorker = new QThread;
        w_focusChangedEvent->moveToThread(t_focusChangedEventWorker);
        connect(t_focusChangedEventWorker, SIGNAL(started()), w_focusChangedEvent, SLOT(doWork()));
        t_focusChangedEventWorker->start();
    }
    if(w_generalEvents != NULL){
        QThread *t_generalEventsWorker = new QThread;
        w_generalEvents->moveToThread(t_generalEventsWorker);
        connect(t_generalEventsWorker, SIGNAL(started()), w_generalEvents, SLOT(doWork()));
        t_generalEventsWorker->start();
    }
}
