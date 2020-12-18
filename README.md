# Event listener for Windows


**Usage:**

```cpp
#include <QList>
#include <QVariantMap>
#include <QString>
#include <QDebug>

#include <wineventlistener.h>


int main(int argc, char *argv[])
{
    // to receive the event identified by the listener
    QVariantMap eventDetected;

    /* QList with all events to be identified
     * To know all suported events, see:
     * https://docs.microsoft.com/en-us/windows/win32/winauto/uiauto-event-ids
     */
    QList<QString> eventsToIdentify{
        "UIA_ToolTipOpenedEventId",
        "UIA_ToolTipClosedEventId",
        "UIA_AutomationFocusChangedEventId",
        "UIA_Window_WindowClosedEventId",
    };

    /* Create an event listener 
       you can also limit the listener to specific programs,
       like: WinEventListener *listener = new WinEventListener("soffice.bin);
       to watch LibreOffice events
    */
    WinEventListener *listener = new WinEventListener();

    // Add events you want identify
    listener->addEventsToIdentify(eventsToIdentify);

    // And.. have fun!
    listener->listenerStart();

    while(true){
        if( eventDetected != *listener->eventDetected ){
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
