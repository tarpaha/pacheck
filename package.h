#ifndef PACKAGE_H
#define PACKAGE_H

#include <QObject>

class Package : public QObject
{
    Q_OBJECT

public:
    Package(QObject* parent, const QString& name, const QString& url);

public:
    const QString& name() const { return _name; }
    const QString& version()  const { return _version; }

public:
    void getVersions();

public:
    operator QString();

private:
    void addVersion(const QString& version);

private:
    QString _name;
    QString _path;
    QString _version;

private slots:
    void onGetBaseFoldersSucceeded(const QString& data, const QString&);
    void onGetFolderContentSucceeded(const QString &data, const QString&);
};

#endif // PACKAGE_H
