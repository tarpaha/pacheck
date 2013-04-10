#ifndef STATE_GETPACKAGESLIST_H
#define STATE_GETPACKAGESLIST_H

#include "state_widget.h"

class State_GetPackagesList : public State_Widget
{
    Q_OBJECT
public:
    explicit State_GetPackagesList(MainWidget *widget, const QString& packagesFolder);
    
public:
    virtual void start();

private:
    const QString _packagesFolder;

public:
    const QString& packagesList() const { return _packagesList; }
private:
    QString _packagesList;

private slots:
    void onPropGetSucceeded(const QString& packagesListString, const QVariant&);
    void onPropGetFailed(const QString& errorString, const QVariant&);
};

#endif // STATE_GETPACKAGESLIST_H
