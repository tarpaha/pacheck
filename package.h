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
    Package(QWidget* parent, const QString& url, const char* onVersionChanged);

public:
    const QString& name() const { return _name; }
    const QString& version()  const { return _currentVersion; }

public:
    QWidget* getNameWidget()       { return _nameWidget; }
    QWidget* getVersionsWidget()   { return _versionsControlWidget; }

public:
    void getVersions(QObject *sender, const char* onVesionsReceived);
    bool versionChanged() const { return _currentVersion != _selectedVersion; }

public:
    operator QString();

private:
    void addVersion(const QString& version);

private:
    QString _name;
    QString _basePath;
    const char* _onVersionChanged;
private:
    QString _currentVersion;
    QString _selectedVersion;
    QString _bestVersion;

private:
    int _folderContentCallsLeft;

private:
    QLabel*         _nameWidget;
    QStackedWidget* _versionsControlWidget;
    QComboBox*      _versionsWidget;

signals:
    void versionsReceived();

private:
    QStringList     _versions;

private:
    void updateNameWidget();
    void showVersions();

private slots:
    void onGetBaseFoldersSucceeded(const QString& data, const QVariant&);
    void onGetFolderContentSucceeded(const QString &data, const QVariant&);

private slots:
    void currentVersionIndexChanged(int index);

public:
    static bool lessThan(Package *p1, Package *p2);
};

#endif // PACKAGE_H
