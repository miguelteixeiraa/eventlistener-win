#include "wineventlistener.h"



WinEventListener::WinEventListener()
{
}

void WinEventListener::addEventsToIdentify(const QList<QString> &list){
    QList<QString> *generalEvents = new QList<QString>;
    for(auto eventID : list){
        if(nonGeneralEventIDs.contains(eventID) && eventID == "UIA_StructureChangedEventId"){
            qDebug() << "Could not add event " + eventID;
        }
        else if(nonGeneralEventIDs.contains(eventID) && eventID == "UIA_AutomationPropertyChangedEventId"){
            qDebug() << "Could not add event " + eventID;
        }
        else if(nonGeneralEventIDs.contains(eventID) && eventID == "UIA_AutomationFocusChangedEventId"){
            focusChangedEventHandler = new FocusChangedEventHandler(*eventDetected);
        }
        else{
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
