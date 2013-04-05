#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>

class MainWidget;
class QSettings;

class Settings
{
public:
    Settings(MainWidget* mainWidget);

public:
    void loadUI();
    void saveUI();

public:
    QString getPackagesFolder() const;
    void setPackagesFolder(const QString& folder);

private:
    MainWidget* _mainWidget;
    QSettings   _settings;
};

#endif // SETTINGS_H
