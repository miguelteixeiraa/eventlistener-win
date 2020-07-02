#include <QCoreApplication>
#include <wineventlistener.h>
#include <QList>
#include <QString>

int main(int argc, char *argv[]){
    
    WinEventListener w;
    QList<QString> q{
        "UIA_ToolTipOpenedEventId",
        "UIA_ToolTipClosedEventId"
    };
    
    w.addEventsToIdentify(q);
    w.listenerStart();
    
    QCoreApplication a(argc, argv);
    return a.exec();
}
