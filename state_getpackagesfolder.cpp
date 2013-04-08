#include "state_getpackagesfolder.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"

State_GetPackagesFolder::State_GetPackagesFolder(MainWidget *widget) :
    State_Widget(widget)
{
}

void State_GetPackagesFolder::start()
{
    _packagesFolder = _widget->_settings.getPackagesFolder();
    if(_packagesFolder.length() > 0)
    {
        emit succeeded();
    }
    else
    {
        allowToChooseFolder();
    }
}

void State_GetPackagesFolder::allowToChooseFolder()
{
    _widget->ui->statusLabel->setText("");
    _widget->ui->selectFolderButton->setEnabled(true);
}
