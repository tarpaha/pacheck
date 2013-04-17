#include "settings.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"

static const QSize  WINDOW_DEFAULT_SIZE     = QSize(400, 400);
static const QPoint WINDOW_DEFAULT_POSITION = QPoint(400, 200);

static const int TABLE_DEFAULT_FIRST_COLUMN_WIDTH = 200;

Settings* Settings::_instance = 0;

void Settings::Init(const QString& organization, const QString& application)
{
    _instance = new Settings(organization, application);
}

Settings::Settings(const QString& organization, const QString& application) :
    _settings(organization, application)
{
}

void Settings::loadUI(MainWidget* mainWidget)
{
    loadWidgetAppearance(mainWidget, "main_widget", WINDOW_DEFAULT_POSITION, WINDOW_DEFAULT_SIZE);

    _instance->_settings.beginGroup("table");
    {
        mainWidget->ui->table->setColumnWidth(0, _instance->_settings.value("first_column_width", TABLE_DEFAULT_FIRST_COLUMN_WIDTH).toInt());
    }
    _instance->_settings.endGroup();
}

void Settings::saveUI(MainWidget* mainWidget)
{
    saveWidgetAppearance(mainWidget, "main_widget");

    _instance->_settings.beginGroup("table");
    {
        _instance->_settings.setValue("first_column_width", mainWidget->ui->table->columnWidth(0));
    }
    _instance->_settings.endGroup();
}

void Settings::saveWidgetAppearance(const QWidget* widget, const QString& groupName)
{
    _instance->_settings.beginGroup(groupName);
    {
        _instance->_settings.setValue("size", widget->size());
        _instance->_settings.setValue("pos", widget->pos());
    }
    _instance->_settings.endGroup();
}

void Settings::loadWidgetAppearance(QWidget *widget, const QString &groupName, const QPoint& defaultPos, const QSize& defaultSize)
{
    _instance->_settings.beginGroup(groupName);
    {
        widget->resize(_instance->_settings.value("size", defaultSize).toSize());
        widget->move(_instance->_settings.value("pos", defaultPos).toPoint());
    }
    _instance->_settings.endGroup();
}

QString Settings::getPackagesFolder()
{
    return _instance->_settings.value("packages_folder", "").toString();
}

void Settings::setPackagesFolder(const QString& folder)
{
    _instance->_settings.setValue("packages_folder", folder);
}
