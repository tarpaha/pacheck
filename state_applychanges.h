#ifndef STATE_APPLYCHANGES_H
#define STATE_APPLYCHANGES_H

#include "state_widget.h"

#include <QTemporaryFile>

class Package;

class State_ApplyChanges : public State_Widget
{
    Q_OBJECT
public:
    explicit State_ApplyChanges(MainWidget* widget, QList<Package*> packages);

public:
    virtual void start();

private:
    const QList<Package*>   _packages;
    QTemporaryFile          _file;

private:
    void prepareFile();
};

#endif // STATE_APPLYCHANGES_H
