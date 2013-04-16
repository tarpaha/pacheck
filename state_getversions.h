#ifndef STATE_GETVERSIONS_H
#define STATE_GETVERSIONS_H

#include "state_widget.h"

class Package;

class State_GetVersions : public State_Widget
{
    Q_OBJECT
public:
    explicit State_GetVersions(MainWidget* widget, QList<Package*> packages);

public:
    virtual void start();

private:
    const QList<Package*> _packages;
    int _packagesLeft;

private slots:
    void onVersionsReceived();
    void onError(const QString& errorString);
    
};

#endif // STATE_GETVERSIONS_H
