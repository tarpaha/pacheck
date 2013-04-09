#include "state_getpackagesfolder.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QFileDialog>

State_GetPackagesFolder::State_GetPackagesFolder(MainWidget *widget, const QString& packagesFolder) :
    State_Widget(widget),
    _packagesFolder(packagesFolder)
{
}

void State_GetPackagesFolder::start()
{
    processPackagesFolder();
}

void State_GetPackagesFolder::processPackagesFolder()
{
    if(_packagesFolder.length() > 0)
    {
        enableFolderSelection(false);
        succeed();
    }
    else
    {
        enableFolderSelection(true);
    }
}

void State_GetPackagesFolder::enableFolderSelection(bool enabled)
{
    if(enabled)
    {
        _widget->ui->selectFolderButton->setEnabled(true);
        QObject::connect(_widget->ui->selectFolderButton, &QPushButton::clicked, this, &State_GetPackagesFolder::showSelectionDialog);
    }
    else
    {
        _widget->ui->selectFolderButton->setEnabled(false);
        QObject::disconnect(_widget->ui->selectFolderButton, 0, this, 0);
    }
}

void State_GetPackagesFolder::showSelectionDialog()
{
    _packagesFolder = QFileDialog::getExistingDirectory(
                _widget,
                "Select packages folder",
                _packagesFolder,
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    processPackagesFolder();
}
