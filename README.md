# Event listener for Windows

**Usage:**

```cpp
#include <QList>
#include <QString>

#include <wineventlistener.h>

int main(int argc, char *argv[])
{
    //Create a WinEventListener
    WinEventListener w;

    //Events to watch
    QList<QString> q{
        "UIA_ToolTipOpenedEventId",
        "UIA_ToolTipClosedEventId"
    };

    //Add events that you wanna watch to your listener
    w.addEventsToIdentify(q);

    //And.. have fun!
    w.listenerStart();
}
```

With this library you should be able to capture all Microsoft UIAutomation events present here:
```
MontorableUIAutoEvents.h
```

This readme will be completely written one day.
