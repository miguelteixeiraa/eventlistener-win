#include <QList>
#include <QString>
#include <clocale>

#include <wineventlistener.h>


int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "portuguese-brazilian");
    
    QList<QString> event_list{
        "UIA_ToolTipOpenedEventId",
        "UIA_ToolTipClosedEventId"
    };

    WinEventListener w;
    w.addEventsToIdentify(event_list);
    w.listenerStart();

    return a.exec();
}
