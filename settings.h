#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>

class MainWidget;
class QSettings;

class Settings
{
public:
    static void Init(const QString& organization, const QString& application);

public:
    static void loadUI(MainWidget* mainWidget);
    static void saveUI(MainWidget* mainWidget);

public:
    static void saveWidgetAppearance(const QWidget *widget, const QString& groupName);
    static void loadWidgetAppearance(QWidget *widget, const QString& groupName, const QPoint &defaultPos, const QSize &defaultSize);

public:
    static QString getPackagesFolder();
    static void setPackagesFolder(const QString& folder);

private:
    Settings(const QString& organization, const QString& application);

private:
    static Settings* _instance;
private:
    QSettings   _settings;

};

#endif // SETTINGS_H
