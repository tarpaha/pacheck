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

private:
    QString _packagesFolder;

private:
    void allowToChooseFolder();
};

#endif // GETPACKAGESFOLDERSTATE_H
