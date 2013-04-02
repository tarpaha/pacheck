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
                 STRING_SLOT(onSvnPresent), STRING_SLOT(onSvnAbsent));
}

void MainWidget::onSvnPresent(const QString& versionString)
{
    QStringList ver = versionString.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    ui->svnVersionLabel->setText(QString("svn %1").arg(ver[0]));

    getPackagesFolder();
}

void MainWidget::onSvnAbsent(const QString& errorString)
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
                STRING_SLOT(onGetExternalsSucceeded),
                STRING_SLOT(onGetExternalsFailed));
}

void DisplayErrorMessage(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWidget::onGetExternalsSucceeded(const QString& externalsString)
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

void MainWidget::onGetExternalsFailed(const QString &errorString)
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

class Package
{
private:
    QString _name;
    QString _url;
    QString _version;

public:
    const QString& name() const { return _name; }
    const QString& url()  const { return _url; }

    const QString& version()  const { return _version; }

public:
    Package(const QString& name, const QString& url) : _name(name), _url(url)
    {
        parseUrl();
    }
    operator QString() { return QString("name = %1, url = %2").arg(_name, _url); }

    void parseUrl()
    {
        QStringList urlParts = _url.split(_name, QString::SkipEmptyParts, Qt::CaseInsensitive); // TODO: assertion, length must == 2
        qDebug() << urlParts;
        _version = urlParts[1];
    }
};

QList<Package> packages;

void MainWidget::parsePackages(const QString& packagesString)
{
    QStringList packagesStrings = packagesString.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

    foreach(QString packageString, packagesStrings)
    {
        QStringList parts = packageString.split(' ');
        packages.append(Package(parts[1], parts[0]));
    }

    ui->table->clearContents();
    ui->table->setRowCount(packages.length());
    int row = 0;
    foreach(Package package, packages)
    {
        ui->table->setItem(row, 0, new QTableWidgetItem(package.name()));
        ui->table->setItem(row, 1, new QTableWidgetItem(package.version()));
        row++;
    }
}
