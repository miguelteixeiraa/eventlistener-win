#ifndef MONITORABLEUIAUTOEVENTS_H
#define MONITORABLEUIAUTOEVENTS_H

#include <QMap>
#include <QString>

static const QMap<QString, long> uiAutoMonitorableEvents{
    {"UIA_ToolTipOpenedEventId", 20000},
    {"UIA_ToolTipClosedEventId", 20001},
    {"UIA_MenuOpenedEventId", 20003},
    {"UIA_AsyncContentLoadedEventId", 20006},
    {"UIA_MenuClosedEventId", 20007},
    {"UIA_LayoutInvalidatedEventId", 20008},
    {"UIA_Invoke_InvokedEventId", 20009},
    {"UIA_SelectionItem_ElementAddedToSelectionEventId", 20010},
    {"UIA_SelectionItem_ElementRemovedFromSelectionEventId", 20011},
    {"UIA_SelectionItem_ElementSelectedEventId", 20012},
    {"UIA_Selection_InvalidatedEventId", 20013},
    {"UIA_Text_TextSelectionChangedEventId", 20014},
    {"UIA_Text_TextChangedEventId", 20015},
    {"UIA_Window_WindowOpenedEventId", 20016},
    {"UIA_Window_WindowClosedEventId", 20017},
    {"UIA_MenuModeStartEventId", 20018},
    {"UIA_MenuModeEndEventId", 20019},
    {"UIA_InputReachedTargetEventId", 20020},
    {"UIA_InputReachedOtherElementEventId", 20021},
    {"UIA_InputDiscardedEventId", 20022},
    {"UIA_SystemAlertEventId", 20023},
    {"UIA_LiveRegionChangedEventId", 20024},
    {"UIA_HostedFragmentRootsInvalidatedEventId", 20025},
    {"UIA_Drag_DragStartEventId", 20026},
    {"UIA_Drag_DragCancelEventId", 20027},
    {"UIA_Drag_DragCompleteEventId", 20028},
    {"UIA_DropTarget_DragEnterEventId", 20029},
    {"UIA_DropTarget_DragLeaveEventId", 20030},
    {"UIA_DropTarget_DroppedEventId", 20031},
    {"UIA_TextEdit_TextChangedEventId", 20032},
    {"UIA_TextEdit_ConversionTargetChangedEventId", 20033}
};

#endif // MONITORABLEUIAUTOEVENTS_H
