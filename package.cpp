#include "package.h"
#include "process.h"

#include <QStringList>
#include <QDebug>

Package::Package(QObject* parent, const QString& name, const QString& url) :
    QObject(parent),
    _name(name)
{
    // divide by base_path, package_name, package_path
    QStringList urlParts = url.split(_name, QString::SkipEmptyParts, Qt::CaseInsensitive); // TODO: assertion, length must == 2

    _path = urlParts[0] + _name;
    _version = urlParts[1];
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
    QStringList baseFolders = data.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    foreach(QString baseFolder, baseFolders)
    {
        if((baseFolder == "branches/") || (baseFolder == "tags/"))
            Process::run(this, "svn list " + _path + "/" + baseFolder, baseFolder, SLOT(onGetFolderContentSucceeded(const QString&, const QString&)), 0);
        else if(baseFolder == "trunk/")
            addVersion("trunk");
    }
}

void Package::onGetFolderContentSucceeded(const QString& str, const QString& data)
{
    QStringList versions = str.split(QRegExp("[\r\n/]"), QString::SkipEmptyParts);
    foreach(QString version, versions)
    {
        addVersion(data + version);
    }
}

void Package::addVersion(const QString &version)
{
    qDebug() << version;
}
