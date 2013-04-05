#ifndef PACKAGE_H
#define PACKAGE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QComboBox>

class Package : public QObject
{
    Q_OBJECT

public:
    Package(QWidget* parent, const QString& url);

public:
    const QString& name() const { return _name; }
    const QString& version()  const { return _version; }

public:
    QWidget* getNameWidget()       { return _nameWidget; }
    QWidget* getVersionsWidget()   { return _versionsControlWidget; }

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

private:
    int _folderContentCallsLeft;

private:
    QLabel*         _nameWidget;
    QStackedWidget* _versionsControlWidget;
    QComboBox*      _versionsWidget;

private slots:
    void onGetBaseFoldersSucceeded(const QString& data, const QString&);
    void onGetFolderContentSucceeded(const QString &data, const QString&);
};

#endif // PACKAGE_H
