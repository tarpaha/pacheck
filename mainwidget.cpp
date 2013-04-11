#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "process.h"
#include "package.h"
#include "settings.h"

#include "svnutils.h"
#include "state.h"
#include "state_svncheck.h"
#include "state_getpackagesfolder.h"
#include "state_getpackageslist.h"
#include "state_getversions.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

MainWidget::MainWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    _settings(this)
{
    ui->setupUi(this);

    _settings.loadUI();    

    ui->selectFolderButton->setEnabled(false);

    setState(new State_SvnCheck(this), &MainWidget::onSvnPresent, &MainWidget::onSvnAbsent);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::closeEvent(QCloseEvent* event)
{
    _settings.saveUI();
    QWidget::closeEvent(event);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void MainWidget::setState(State* state, void (MainWidget::* onSucceeded)(), void (MainWidget::* onFailed)())
{
    _currentState = state;

    QObject::connect(_currentState, &State::succeeded, this, onSucceeded);
    QObject::connect(_currentState, &State::failed, this, onFailed);

    _currentState->start();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void MainWidget::onSvnPresent()
{
    _packagesFolder = _settings.getPackagesFolder();
    if(_packagesFolder != 0)
    {
        setState(new State_GetPackagesList(this, _packagesFolder), &MainWidget::OnPackagesListReceived, &MainWidget::OnPackagesListFailed);
    }
    else
    {
        setState(new State_GetPackagesFolder(this, _packagesFolder), &MainWidget::onFolderSelected, 0);
    }
}

void MainWidget::onSvnAbsent()
{
    QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void MainWidget::onFolderSelected()
{
    _packagesFolder = static_cast<State_GetPackagesFolder*>(_currentState)->packagesFolder();
    setState(new State_GetPackagesList(this, _packagesFolder), &MainWidget::OnPackagesListReceived, &MainWidget::OnPackagesListFailed);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void MainWidget::OnPackagesListFailed()
{
    setState(new State_GetPackagesFolder(this, _packagesFolder), &MainWidget::onFolderSelected, 0);
}

void MainWidget::OnPackagesListReceived()
{
    _settings.setPackagesFolder(_packagesFolder);

    const QString& packagesListString = static_cast<State_GetPackagesList*>(_currentState)->packagesList();

    fillPackagesList(SvnUtils::splitPackagesList(packagesListString));
    applyPackagesToTable();

    setWindowTitle(_packagesFolder);

    setState(new State_GetVersions(this, _packages), &MainWidget::onVersionsReceived, 0);
}

void MainWidget::fillPackagesList(const QStringList& packagesList)
{
    _packages.clear();
    foreach(QString packageUrl, packagesList)
    {
        _packages.append(new Package(this, packageUrl, SLOT(onVersionChanged())));
    }
    qSort(_packages.begin(), _packages.end(), Package::lessThan);
}

void MainWidget::applyPackagesToTable()
{
    ui->table->clearContents();
    ui->table->setRowCount(_packages.length());

    int row = 0;
    foreach(Package* package, _packages)
    {
        ui->table->setCellWidget(row, 0, package->getNameWidget());
        ui->table->setCellWidget(row, 1, package->getVersionsWidget());
        row++;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void MainWidget::onVersionsReceived()
{
    setState(new State_GetPackagesFolder(this, _packagesFolder), &MainWidget::onFolderSelected, 0);
}

void MainWidget::onVersionChanged()
{
    ui->applyChangesButton->setEnabled(anyPackageVersionChanged());
}

bool MainWidget::anyPackageVersionChanged()
{
    foreach(Package* package, _packages)
    {
        if(package->versionChanged())
            return true;
    }
    return false;
}
