#ifndef PACKAGE_H
#define PACKAGE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QComboBox>
#include <QStringList>

class Package : public QObject
{
    Q_OBJECT

public:
    Package(QWidget* parent, const QString& url);

public:
    const QString& name() const { return _name; }
    const QString& version()  const { return _currentVersion; }

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
    QString _basePath;
    QString _currentVersion;

private:
    int _folderContentCallsLeft;

private:
    QLabel*         _nameWidget;
    QStackedWidget* _versionsControlWidget;
    QComboBox*      _versionsWidget;

private:
    QStringList     _versions;

private:
    void showVersions();

private slots:
    void onGetBaseFoldersSucceeded(const QString& data, const QVariant&);
    void onGetFolderContentSucceeded(const QString &data, const QVariant&);
};

#endif // PACKAGE_H
