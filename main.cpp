#include <QCoreApplication>
#include <wineventlistener.h>
#include <QList>
#include <QString>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
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
