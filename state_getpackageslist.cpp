#include "state_getpackageslist.h"
#include "mainwidget.h"
#include "process.h"

#include <QMessageBox>

State_GetPackagesList::State_GetPackagesList(MainWidget *widget, const QString& packagesFolder) :
    State_Widget(widget),
    _packagesFolder(packagesFolder)
{
}

void State_GetPackagesList::start()
{
    Process::run(
                this,
                QString("svn propget svn:externals %1").arg(_packagesFolder),
                0,
                PROCESS_SLOT(onPropGetSucceeded),
                PROCESS_SLOT(onPropGetFailed));
}

void DisplayErrorMessage(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void State_GetPackagesList::onPropGetSucceeded(const QString& externalsString, const QVariant&)
{
    if(externalsString == 0)
    {
        DisplayErrorMessage(QString("Folder %1 do not contain external packages").arg(_packagesFolder));
        fail();
    }
    else
    {
        succeed();
    }
}

void State_GetPackagesList::onPropGetFailed(const QString&, const QVariant&)
{
    DisplayErrorMessage(QString("Folder %1 do not controlled by SVN").arg(_packagesFolder));
    fail();
}
