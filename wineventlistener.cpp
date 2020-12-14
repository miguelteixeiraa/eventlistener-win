#include "wineventlistener.h"

// start FocusChangedEventWorker
FocusChangedEventWorker::FocusChangedEventWorker( QVariantMap &worker_eventDetected, const QString &eventsByAppName ){
    focusChangedEventHandler = new FocusChangedEventHandler(worker_eventDetected, eventsByAppName);
}

void FocusChangedEventWorker::run(){
    focusChangedEventHandler->startHandler();
}
// end FocusChangedEventWorker

// start GeneralEventsWorker
GeneralEventsWorker::GeneralEventsWorker( QVariantMap &worker_eventDetected, const QString &eventsByAppName ){
    generalEventsHandler = new GeneralEventsHandler(worker_eventDetected, eventsByAppName);
}

void GeneralEventsWorker::addEventsToIdentify( QList<QString> &w_eventsToIdentify ){
    generalEventsHandler->addEventsToIdentify(w_eventsToIdentify);
}

void GeneralEventsWorker::run(){
    generalEventsHandler->startHandler();
}
// end GeneralEventsWorker

// start WinEventListener
WinEventListener::WinEventListener( const QString &eventsByAppName ){
    this->WEL_eventsByAppName = eventsByAppName;
}

void WinEventListener::addEventsToIdentify( const QList<QString> &list ){
    QList<QString> *generalEvents = new QList<QString>;
    for( auto eventID : list ){
        if(!nonGeneralEventIDs.contains(eventID)){
            generalEvents->append(eventID);
        }
    }
    w_generalEvents = new GeneralEventsWorker(*eventDetected, this->WEL_eventsByAppName);
    w_generalEvents->addEventsToIdentify(*generalEvents);
    if( list.contains("UIA_AutomationFocusChangedEventId") ){
        w_focusChangedEvent = new FocusChangedEventWorker(*eventDetected, this->WEL_eventsByAppName);
    }
}

QVariantMap WinEventListener::getEventDetected(){
    return *eventDetected;
}

void WinEventListener::listenerStart(){
    if( w_focusChangedEvent != NULL ){
        QThreadPool::globalInstance()->start(w_focusChangedEvent);
        //qDebug() << "I'm here";
    }
    QThread::sleep(5);
    if( w_generalEvents != NULL ){
        QThreadPool::globalInstance()->start(w_generalEvents);
        //qDebug() << "I'm here";
    }
}
// end WinEventListener
