#include "wineventlistener.h"


WinEventListener::WinEventListener()
{
}

void WinEventListener::addEventsToIdentify(const QList<QString> &list){
    QList<QString> *generalEvents = new QList<QString>;
    if(list.contains("UIA_AutomationFocusChangedEventId"){
        focusChangedEventHandler = new FocusChangedEventHandler(*eventDetected);
     }
    for(auto eventID : list){
        if(!nonGeneralEventIDs.contains(eventID){
            generalEvents->append(eventID);
        }
    }
    generalEventsHandler = new GeneralEventsHandler(*eventDetected);
    generalEventsHandler->addEventsToIdentify(*generalEvents);
}

void WinEventListener::listenerStart(){
    focusChangedEventHandler->startHandler();
    generalEventsHandler->startHandler();
}
