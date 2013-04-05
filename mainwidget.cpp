#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "process.h"
#include "package.h"
#include "settings.h"

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

    checkSvnVersion();
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

void MainWidget::checkSvnVersion()
{
    ui->statusLabel->setText("checking SVN...");
    Process::run(this,
                 "svn --version --quiet",
                 0,
                 PROCESS_SLOT(onSvnPresent),
                 PROCESS_SLOT(onSvnAbsent));
}

void MainWidget::onSvnPresent(const QString& versionString, const QVariant&)
{
    QStringList ver = versionString.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    ui->svnVersionLabel->setText(QString("svn %1").arg(ver[0]));

    getPackagesFolder();
}

void MainWidget::onSvnAbsent(const QString& errorString, const QVariant &)
{
    QMessageBox msgBox;
    msgBox.setText(QString("Error running \"svn\" command.\nError string: %1").arg(errorString));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

void MainWidget::getPackagesFolder()
{
    _packagesFolder = _settings.getPackagesFolder();
    if(_packagesFolder.length() > 0)
    {
        getExternals();
    }
    else
    {
        allowToChooseFolder();
    }
}

void MainWidget::showFolderSelectionDialog()
{
    ui->selectFolderButton->setEnabled(false);
    ui->statusLabel->setText("selecting folder...");

    _packagesFolder = QFileDialog::getExistingDirectory(
                this,
                "Select packages folder",
                _packagesFolder,
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(_packagesFolder != 0)
        getExternals();
    else
        allowToChooseFolder();
}

void MainWidget::getExternals()
{
    ui->selectFolderButton->setEnabled(false);
    ui->statusLabel->setText("getting externals...");

    Process::run(
                this,
                QString("svn propget svn:externals %1").arg(_packagesFolder),
                0,
                PROCESS_SLOT(onGetExternalsSucceeded),
                PROCESS_SLOT(onGetExternalsFailed));
}

void DisplayErrorMessage(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWidget::onGetExternalsSucceeded(const QString& externalsString, const QVariant&)
{
    if(externalsString == 0)
    {
        DisplayErrorMessage(QString("Folder %1 do not contain external packages").arg(_packagesFolder));
        _settings.setPackagesFolder("");
    }
    else
    {
        _settings.setPackagesFolder(_packagesFolder);
        parsePackages(externalsString);
    }
    allowToChooseFolder();
}

void MainWidget::onGetExternalsFailed(const QString&, const QVariant&)
{
    DisplayErrorMessage(QString("Folder %1 do not controlled by SVN").arg(_packagesFolder));
    _settings.setPackagesFolder("");
    allowToChooseFolder();
}

void MainWidget::allowToChooseFolder()
{
    ui->statusLabel->setText("");
    ui->selectFolderButton->setEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

QList<Package*> packages;

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

    foreach(Package* package, packages)
        package->getVersions();
}
