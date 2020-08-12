#ifndef WINEVENTLISTENER_H
#define WINEVENTLISTENER_H

#include <QDebug>
#include <QObject>
#include <QMap>
#include <QList>
#include <QString>
#include <QThread>

#include <focuschangedeventhandler.h>
#include <generaleventshandler.h>


class WinEventListener : public QObject
{
    Q_OBJECT

    FocusChangedEventHandler *focusChangedEventHandler = NULL;
    GeneralEventsHandler *generalEventsHandler = NULL;

    QList<QString> nonGeneralEventIDs = {
        "UIA_AutomationFocusChangedEventId",
        "UIA_AutomationPropertyChangedEventId",
        "UIA_StructureChangedEventId"
    };

public:
    QMap<QString, QString> *eventDetected = new QMap<QString, QString>;
    explicit WinEventListener();
    void addEventsToIdentify(const QList<QString> &list);
    void listenerStart();
    
};

#endif // WINEVENTLISTENER_H
