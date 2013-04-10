#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "process.h"
#include "package.h"
#include "settings.h"

#include "state.h"
#include "state_svncheck.h"
#include "state_getpackagesfolder.h"
#include "state_getpackageslist.h"

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

void MainWidget::OnPackagesListReceived()
{
    _settings.setPackagesFolder(_packagesFolder);

    const QString& packagesList = static_cast<State_GetPackagesList*>(_currentState)->packagesList();
    qDebug() << packagesList;
}

void MainWidget::OnPackagesListFailed()
{
    setState(new State_GetPackagesFolder(this, _packagesFolder), &MainWidget::onFolderSelected, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
QList<Package*> packages;
int packagesCounter;

void MainWidget::parsePackages(const QString& packagesString)
{
    QStringList packagesStrings = packagesString.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

    packages.clear();
    foreach(QString packageString, packagesStrings)
    {
        QStringList parts = packageString.split(' ');
        packages.append(new Package(this, parts[0]));
    }

    qSort(packages.begin(), packages.end(), Package::lessThan);

    ui->table->clearContents();
    ui->table->setRowCount(packages.length());

    int row = 0;
    foreach(Package* package, packages)
    {
        ui->table->setCellWidget(row, 0, package->getNameWidget());
        ui->table->setCellWidget(row, 1, package->getVersionsWidget());
        row++;
    }

    getVersions();
}

void MainWidget::getVersions()
{
    packagesCounter = packages.length();

    ui->statusLabel->setText("getting versions...");
    foreach(Package* package, packages)
        package->getVersions(this, SLOT(onVersionsReceived()));
}


void MainWidget::onVersionsReceived()
{
    if(--packagesCounter <= 0)
        allowToChooseFolder();
}
*/





