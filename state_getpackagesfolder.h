#ifndef GETPACKAGESFOLDERSTATE_H
#define GETPACKAGESFOLDERSTATE_H

#include "state_widget.h"

class State_GetPackagesFolder : public State_Widget
{
    Q_OBJECT
public:
    explicit State_GetPackagesFolder(MainWidget* widget, const QString &packagesFolder);

public:
    const QString& packagesFolder() const { return _packagesFolder; }

public:
    virtual void start();

private:
    QString _packagesFolder;

private:
    void enableFolderSelection(bool enabled);

private slots:
    void showSelectionDialog();
};

#endif // GETPACKAGESFOLDERSTATE_H
