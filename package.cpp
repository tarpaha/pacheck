#include "package.h"
#include "process.h"
#include "svnutils.h"

#include <QStringList>
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QDir>

Package::Package(QWidget* parent, const QString& url) :
    QObject(parent)
{
    QStringList parts = SvnUtils::divideSvnPath(url);

    _path    = parts[0];
    _name    = parts[1];
    _version = parts[2];

    _nameWidget = new QLabel(parent);
    _nameWidget->setText(_name);

    _versionsControlWidget = new QStackedWidget(parent);

    QLabel* progressLabel = new QLabel(parent);
    progressLabel->setText("  loading ...");

    _versionsWidget = new QComboBox(parent);

    _versionsControlWidget->addWidget(_versionsWidget);
    _versionsControlWidget->addWidget(progressLabel);

    _versionsControlWidget->setCurrentWidget(progressLabel);
}

Package::operator QString()
{
    return QString("name = %1, path = %2, version = %3").arg(_name, _path, _version);
}

void Package::getVersions()
{
    Process::run(this, "svn list " + _path, 0, SLOT(onGetBaseFoldersSucceeded(const QString&, const QString&)), 0);
}

void Package::onGetBaseFoldersSucceeded(const QString& data, const QString&)
{
    QStringList baseFolders = data.split(QRegExp("[\r\n/]"), QString::SkipEmptyParts);

    _folderContentCallsLeft = 0;

    foreach(QString baseFolder, baseFolders)
    {
        qDebug() << baseFolder;

        if(baseFolder == "trunk")
        {
            addVersion("trunk");
            continue;
        }

        _folderContentCallsLeft += 1;
    }

    foreach(QString baseFolder, baseFolders)
    {
        if(baseFolder == "trunk")
            continue;
        Process::run(this, "svn list " + _path + "/" + baseFolder, baseFolder, SLOT(onGetFolderContentSucceeded(const QString&, const QString&)), 0);
    }
}

void Package::onGetFolderContentSucceeded(const QString& str, const QString& data)
{
    QStringList versions = str.split(QRegExp("[\r\n/]"), QString::SkipEmptyParts);
    foreach(QString version, versions)
    {
        addVersion(data + "/" + version);
    }

    if(--_folderContentCallsLeft <= 0)
        _versionsControlWidget->setCurrentWidget(_versionsWidget);
}

void Package::addVersion(const QString &version)
{
    _versionsWidget->addItem(version);
    //qDebug() << version;
}
