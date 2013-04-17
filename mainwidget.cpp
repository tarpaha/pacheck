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
#include "state_applychanges.h"
#include "commentswidget.h"

#include <QDebug>
#include <QFileDialog>

MainWidget::MainWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    Settings::Init("tarpaha", "pacheck");
    Settings::loadUI(this);

    ui->selectFolderButton->setEnabled(false);

    ui->applyChangesButton->setEnabled(false);
    QObject::connect(ui->applyChangesButton, SIGNAL(clicked()), this, SLOT(onApplyButtonPressed()));

    setState(new State_SvnCheck(this), &MainWidget::onSvnPresent, &MainWidget::onSvnAbsent);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::closeEvent(QCloseEvent* event)
{
    Settings::saveUI(this);;
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
    _packagesFolder = Settings::getPackagesFolder();
    if(_packagesFolder != 0)
    {
        getPackagesList();
    }
    else
    {
        getPackagesFolder();
    }
}

void MainWidget::onSvnAbsent()
{
    QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void MainWidget::getPackagesFolder()
{
    setState(new State_GetPackagesFolder(this, _packagesFolder), &MainWidget::onFolderSelected, 0);
}

void MainWidget::onFolderSelected()
{
    _packagesFolder = qobject_cast<State_GetPackagesFolder*>(_currentState)->packagesFolder();
    getPackagesList();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void MainWidget::getPackagesList()
{
    setState(new State_GetPackagesList(this, _packagesFolder),
             &MainWidget::OnPackagesListReceived,
             &MainWidget::OnPackagesListFailed);
}

void MainWidget::OnPackagesListFailed()
{
    getPackagesFolder();
}

void MainWidget::OnPackagesListReceived()
{
    Settings::setPackagesFolder(_packagesFolder);

    const QString& packagesListString = qobject_cast<State_GetPackagesList*>(_currentState)->packagesList();

    fillPackagesList(SvnUtils::splitPackagesList(packagesListString));
    applyPackagesToTable();

    setWindowTitle(_packagesFolder);

    setState(new State_GetVersions(this, _packages), &MainWidget::onVersionsReceived, &MainWidget::onVersionsReceiveError);
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
    getPackagesFolder();
    onVersionChanged();
}

void MainWidget::onVersionsReceiveError()
{
    getPackagesFolder();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void MainWidget::onVersionChanged()
{
    bool gettingVesions = (qobject_cast<State_GetVersions*>(_currentState) != 0);
    ui->applyChangesButton->setEnabled(!gettingVesions && anyPackageVersionChanged());
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void MainWidget::onApplyButtonPressed()
{
    ui->applyChangesButton->setEnabled(false);
    setState(new State_ApplyChanges(this, _packages, _packagesFolder), &MainWidget::onChangesApplied, &MainWidget::onChangesFailed);
}

void MainWidget::onChangesApplied()
{
    QString comments = qobject_cast<State_ApplyChanges*>(_currentState)->getComments();

    CommentsWidget* commentsWidget = new CommentsWidget(this, comments);
    commentsWidget->show();

    getPackagesList();
}

void MainWidget::onChangesFailed()
{
    getPackagesList();
}
