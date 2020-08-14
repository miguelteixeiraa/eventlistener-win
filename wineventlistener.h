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


class FocusChangedEventWorker : public QObject
{
    Q_OBJECT
public:
    explicit FocusChangedEventWorker(QMap<QString, QString> &worker_eventDetected);
    FocusChangedEventHandler *focusChangedEventHandler = NULL;
public slots:
    void doWork();

};

class GeneralEventsWorker : public QObject
{
    Q_OBJECT
public:
    explicit GeneralEventsWorker(QMap<QString, QString> &worker_eventDetected);
    void addEventsToIdentify(QList<QString> &w_eventsToIdentify);
    GeneralEventsHandler *generalEventsHandler = NULL;

public slots:
    void doWork();

};


class WinEventListener : public QObject
{
    Q_OBJECT

    FocusChangedEventWorker *w_focusChangedEvent = NULL;
    GeneralEventsWorker *w_generalEvents = NULL;

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
