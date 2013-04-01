#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "process.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#define STRING_SLOT(slotName) SLOT(slotName(const QString&))

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    _settings("tarpaha", "pacheck")
{
    ui->setupUi(this);

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
    Process::run(this, "svn --version --quiet", STRING_SLOT(onSvnPresent), STRING_SLOT(onSvnAbsent));
}

void MainWidget::onSvnPresent(const QString& versionString)
{
    qDebug() << "SVN present: " << versionString;

    QStringList ver = versionString.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    ui->statusLabel->setText(QString("Using SVN ver. %1").arg(ver[0]));

    getPackagesFolder();
}

void MainWidget::onSvnAbsent(const QString& errorString)
{
    qDebug() << "Error getting SVN version: " << errorString;

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
    _packagesFolder = _settings.value("packages_folder").toString();
    if(_packagesFolder == 0)
    {
        _packagesFolder = QFileDialog::getExistingDirectory(
                    this,
                    "Select packages folder",
                    _packagesFolder,
                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        _settings.setValue("packages_folder", _packagesFolder);
    }
    getExternals();
}

void MainWidget::getExternals()
{
    Process::run(
                this,
                QString("svn propget svn:externals %1").arg(_packagesFolder),
                STRING_SLOT(onGetExternalsSucceeded),
                STRING_SLOT(onGetExternalsFailed));
}


void MainWidget::onGetExternalsSucceeded(const QString &externalsString)
{
    if(externalsString == 0)
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Folder %1 do not contain external packages").arg(_packagesFolder));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}

void MainWidget::onGetExternalsFailed(const QString &errorString)
{
    QMessageBox msgBox;
    msgBox.setText(QString("Folder %1 do not controlled by SVN").arg(_packagesFolder));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
