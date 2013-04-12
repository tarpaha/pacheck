#include "package.h"
#include "process.h"
#include "svnutils.h"
#include "utils.h"

#include <QStringList>
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QDir>

// colors from
// http://www.w3schools.com/cssref/css_colornames.asp
static const char* COLOR_PROGRESS = "Lavender";
static const char* COLOR_GOOD     = "PaleGreen";
static const char* COLOR_BAD      = "LightSalmon";

Package::Package(QWidget* parent, const QString& url, const char *onVersionChanged) :
    QObject(parent),
    _onVersionChanged(onVersionChanged)
{
    QStringList parts = SvnUtils::splitSvnPath(url);

    _basePath       = parts[0];
    _name           = parts[1];
    _currentVersion = parts[2];

    _selectedVersion = _bestVersion = _currentVersion;

    _nameWidget = new QLabel(parent);
    updateNameWidget();

    _nameWidget->setStyleSheet(Utils::bcStyleSheet(_nameWidget, COLOR_PROGRESS));

    _versionsControlWidget = new QStackedWidget(parent);

    QLabel* progressLabel = new QLabel(parent);
    progressLabel->setText("  loading ...");
    progressLabel->setStyleSheet(Utils::bcStyleSheet(progressLabel, COLOR_PROGRESS));

    _versionsWidget = new QComboBox(parent);
    _versionsWidget->setStyleSheet(Utils::bcStyleSheet(_versionsWidget, COLOR_PROGRESS));
    _versionsWidget->setFrame(false);    

    _versionsControlWidget->addWidget(_versionsWidget);
    _versionsControlWidget->addWidget(progressLabel);

    _versionsControlWidget->setCurrentWidget(progressLabel);
}

QString Package::selectedPath() const
{
    return _basePath + "/" + _selectedVersion;
}

Package::operator QString()
{
    return QString("name = %1, path = %2, version = %3").arg(_name, _basePath, _currentVersion);
}

void Package::getVersions(QObject* sender, const char* onVersionsReceived)
{
    QObject::connect(this, SIGNAL(versionsReceived()), sender, onVersionsReceived);
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

    _selectedVersion = _currentVersion;
    _bestVersion = SvnUtils::getLatestBranch(_versions);

    _versionsWidget->setItemData(_versions.indexOf(_currentVersion), QColor(COLOR_BAD), Qt::BackgroundColorRole);
    _versionsWidget->setItemData(_versions.indexOf(_bestVersion), QColor(COLOR_GOOD), Qt::BackgroundColorRole);

    if(_currentVersion == _bestVersion)
    {
        _nameWidget->setStyleSheet(Utils::bcStyleSheet(_nameWidget, COLOR_GOOD));
        _versionsWidget->setStyleSheet(Utils::bcStyleSheet(_versionsWidget, COLOR_GOOD));
    }
    else
    {
        _nameWidget->setStyleSheet(Utils::bcStyleSheet(_nameWidget, COLOR_BAD));
        _versionsWidget->setStyleSheet(Utils::bcStyleSheet(_versionsWidget, COLOR_BAD));
    }

    _versionsControlWidget->setCurrentWidget(_versionsWidget);

    QObject::connect(_versionsWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(currentVersionIndexChanged(int)));
    QObject::connect(_versionsWidget, SIGNAL(currentIndexChanged(int)), parent(), _onVersionChanged);

    emit versionsReceived();
}

void Package::updateNameWidget()
{
    if(_selectedVersion != _currentVersion)
    {
        _nameWidget->setText(" <b><i> " + _name + "</i></b>");
    }
    else
    {
        _nameWidget->setText(" " + _name);
    }
}

void Package::currentVersionIndexChanged(int index)
{
    _selectedVersion = _versions.at(index);
    updateNameWidget();

    if(_selectedVersion == _bestVersion)
    {
        _nameWidget->setStyleSheet(Utils::bcStyleSheet(_nameWidget, COLOR_GOOD));
        _versionsWidget->setStyleSheet(Utils::bcStyleSheet(_versionsWidget, COLOR_GOOD));
    }
    else
    {
        _nameWidget->setStyleSheet(Utils::bcStyleSheet(_nameWidget, COLOR_BAD));
        _versionsWidget->setStyleSheet(Utils::bcStyleSheet(_versionsWidget, COLOR_BAD));
    }
}

void Package::addVersion(const QString &version)
{
    _versions.append(version);
}

bool Package::lessThan(Package* p1, Package* p2)
{
    return p1->_name.compare(p2->_name) < 0;
}
