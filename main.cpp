#include <QList>
#include <QMap>
#include <QString>
#include <QDebug>

#include <wineventlistener.h>


int main(int argc, char *argv[])
{
    WinEventListener *listener = new WinEventListener();
    QList<QString> events{
        "UIA_ToolTipOpenedEventId",
        "UIA_ToolTipClosedEventId",
        "UIA_AutomationFocusChangedEventId",
    };
    listener->addEventsToIdentify(events);
    listener->listenerStart();

    while(true){
        qDebug() << listener->eventDetected;
    }

    //general.addEventsToIdentify(events);
    //general.startHandler();
    //FocusChangedEventHandler *fceh = new FocusChangedEventHandler;
    //fceh->startHandler();
}
