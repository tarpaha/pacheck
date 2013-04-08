#ifndef WIDGETSTATE_H
#define WIDGETSTATE_H

#include "state.h"

class MainWidget;

class State_Widget : public State
{
    Q_OBJECT

public:
    explicit State_Widget(MainWidget* widget);

protected:
    MainWidget* _widget;
};

#endif // WIDGETSTATE_H
