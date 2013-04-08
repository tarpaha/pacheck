#ifndef GETPACKAGESFOLDERSTATE_H
#define GETPACKAGESFOLDERSTATE_H

#include "state_widget.h"

class State_GetPackagesFolder : public State_Widget
{
    Q_OBJECT
public:
    explicit State_GetPackagesFolder(MainWidget* widget);

public:
    virtual void start();
};

#endif // GETPACKAGESFOLDERSTATE_H
