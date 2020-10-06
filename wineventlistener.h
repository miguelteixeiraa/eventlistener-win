#ifndef WINEVENTLISTENER_H
#define WINEVENTLISTENER_H

#include <QDebug>
#include <QObject>
#include <QVariantMap>
#include <QList>
#include <QString>
#include <QThread>
#include <QMap>

#include <eventlistener-win/focuschangedeventhandler.h>
#include <eventlistener-win/generaleventshandler.h>


class FocusChangedEventWorker : public QObject
{
    Q_OBJECT
public:
    explicit FocusChangedEventWorker(QVariantMap &worker_eventDetected);
    FocusChangedEventHandler *focusChangedEventHandler = NULL;
public slots:
    void doWork();

};

class GeneralEventsWorker : public QObject
{
    Q_OBJECT
public:
    explicit GeneralEventsWorker(QVariantMap &worker_eventDetected);
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
    QThread *t_WinEventListener = new QThread;
    QVariantMap *eventDetected = new QVariantMap;
    Q_PROPERTY(QVariantMap eventDetected READ getEventDetected);
    explicit WinEventListener();
    Q_INVOKABLE void addEventsToIdentify(const QList<QString> &list);
    Q_INVOKABLE QVariantMap getEventDetected();
public slots:
    Q_INVOKABLE void listenerStart();

};

#endif // WINEVENTLISTENER_H
