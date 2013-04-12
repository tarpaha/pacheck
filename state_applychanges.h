#ifndef STATE_APPLYCHANGES_H
#define STATE_APPLYCHANGES_H

#include "state_widget.h"

#include <QTemporaryFile>

class Package;

class State_ApplyChanges : public State_Widget
{
    Q_OBJECT
public:
    explicit State_ApplyChanges(MainWidget* widget, const QList<Package*>& packages, const QString& packagesFolder);

public:
    virtual void start();

private:
    const QList<Package*>   _packages;
    const QString&          _packagesFolder;
    QTemporaryFile          _file;

private:
    QString prepareFile();

private slots:
    void onPropSetSucceeded(const QString&, const QVariant&);
    void onPropSetFailed(const QString& errorString, const QVariant&);
};

#endif // STATE_APPLYCHANGES_H
