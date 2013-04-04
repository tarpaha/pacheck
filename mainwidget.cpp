#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "process.h"
#include "package.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#define PROCESS_SLOT(slotName) SLOT(slotName(const QString&, const QString&))

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    _settings("tarpaha", "pacheck")
{
    ui->setupUi(this);

    ui->selectFolderButton->setEnabled(false);

    checkSvnVersion();
}

MainWidget::~MainWidget()
{
    delete ui;
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

void MainWidget::onSvnPresent(const QString& versionString, const QString&)
{
    QStringList ver = versionString.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    ui->svnVersionLabel->setText(QString("svn %1").arg(ver[0]));

    getPackagesFolder();
}

void MainWidget::onSvnAbsent(const QString& errorString, const QString&)
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
    _packagesFolder = _settings.value("packages_folder", "").toString();
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

void MainWidget::onGetExternalsSucceeded(const QString& externalsString, const QString&)
{
    if(externalsString == 0)
    {
        DisplayErrorMessage(QString("Folder %1 do not contain external packages").arg(_packagesFolder));
        _settings.setValue("packages_folder", "");
    }
    else
    {
        _settings.setValue("packages_folder", _packagesFolder);
        parsePackages(externalsString);
    }
    allowToChooseFolder();
}

void MainWidget::onGetExternalsFailed(const QString &errorString, const QString&)
{
    DisplayErrorMessage(QString("Folder %1 do not controlled by SVN").arg(_packagesFolder));
    _settings.setValue("packages_folder", "");
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

    foreach(QString packageString, packagesStrings)
    {
        QStringList parts = packageString.split(' ');
        packages.append(new Package(this, parts[1], parts[0]));
        break;
    }

    ui->table->clearContents();
    ui->table->setRowCount(packages.length());
    int row = 0;
    foreach(Package* package, packages)
    {
        ui->table->setItem(row, 0, new QTableWidgetItem(package->name()));
        ui->table->setItem(row, 1, new QTableWidgetItem(package->version()));
        row++;
    }

    foreach(Package* package, packages)
        package->getVersions();
}
