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

    _basePath       = parts[0];
    _name           = parts[1];
    _currentVersion = parts[2];

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
    return QString("name = %1, path = %2, version = %3").arg(_name, _basePath, _currentVersion);
}

void Package::getVersions()
{
    Process::run(this, "svn list " + _basePath, 0, PROCESS_SLOT(onGetBaseFoldersSucceeded), 0);
}

void Package::onGetBaseFoldersSucceeded(const QString& data, const QVariant&)
{
    QStringList baseFolders = data.split(QRegExp("[\r\n/]"), QString::SkipEmptyParts);

    _versions.clear();
    _folderContentCallsLeft = 0;

    foreach(QString baseFolder, baseFolders)
    {
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
        Process::run(this, "svn list " + _basePath + "/" + baseFolder, baseFolder, PROCESS_SLOT(onGetFolderContentSucceeded), 0);
    }
}

void Package::onGetFolderContentSucceeded(const QString& str, const QVariant &data)
{
    QStringList versions = str.split(QRegExp("[\r\n/]"), QString::SkipEmptyParts);
    foreach(QString version, versions)
    {
        addVersion(data.toString() + "/" + version);
    }

    if(--_folderContentCallsLeft <= 0)
    {
        showVersions();
    }
}

void Package::showVersions()
{
    _versions.sort();

    _versionsWidget->clear();
    _versionsWidget->addItems(_versions);

    _versionsWidget->setCurrentText(_currentVersion);

    _versionsControlWidget->setCurrentWidget(_versionsWidget);
}

void Package::addVersion(const QString &version)
{
    _versions.append(version);
}
