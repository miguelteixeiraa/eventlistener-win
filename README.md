# Event listener for Windows


**Usage:**

```cpp
#include <QList>
#include <QMap>
#include <QString>
#include <QDebug>

#include <wineventlistener.h>


int main(int argc, char *argv[])
{
    // to receive the event identified by the listener
    QMap<QString, QString> eventDetected;

    /* QList with all events to be identified
     * To know all suported events, see:
     * https://docs.microsoft.com/en-us/windows/win32/winauto/uiauto-event-ids
     */
    QList<QString> eventsToIdenfify{
        "UIA_ToolTipOpenedEventId",
        "UIA_ToolTipClosedEventId",
        "UIA_AutomationFocusChangedEventId",
    };

    // Create an event listener
    WinEventListener *listener = new WinEventListener();

    // Add events you want identify
    listener->addEventsToIdentify(eventsToIdenfify);

    // And.. have fun!
    listener->listenerStart();

    while(true){
        if(eventDetected != *listener->eventDetected){
            eventDetected = *listener->eventDetected;
            qDebug() << eventDetected;
        }
    }
}
```

With this library you should be able to capture all Microsoft UIAutomation events present here:
```
https://docs.microsoft.com/en-us/windows/win32/winauto/uiauto-event-ids
```

This readme will be completely written one day.
