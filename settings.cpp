#include "settings.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"

static const QSize  WINDOW_DEFAULT_SIZE     = QSize(400, 400);
static const QPoint WINDOW_DEFAULT_POSITION = QPoint(400, 200);

static const int TABLE_DEFAULT_FIRST_COLUMN_WIDTH = 200;

Settings::Settings(MainWidget *mainWidget) :
    _mainWidget(mainWidget),
    _settings("tarpaha", "pacheck")
{     
}

void Settings::loadUI()
{
    loadWidgetAppearance(_mainWidget, "main_widget", WINDOW_DEFAULT_POSITION, WINDOW_DEFAULT_SIZE);

    _settings.beginGroup("table");
    {
        _mainWidget->ui->table->setColumnWidth(0, _settings.value("first_column_width", TABLE_DEFAULT_FIRST_COLUMN_WIDTH).toInt());
    }
    _settings.endGroup();
}

void Settings::saveUI()
{
    saveWidgetAppearance(_mainWidget, "main_widget");

    _settings.beginGroup("table");
    {
        _settings.setValue("first_column_width", _mainWidget->ui->table->columnWidth(0));
    }
    _settings.endGroup();
}

void Settings::saveWidgetAppearance(const QWidget* widget, const QString& groupName)
{
    _settings.beginGroup(groupName);
    {
        _settings.setValue("size", widget->size());
        _settings.setValue("pos", widget->pos());
    }
    _settings.endGroup();
}

void Settings::loadWidgetAppearance(QWidget *widget, const QString &groupName, const QPoint& defaultPos, const QSize& defaultSize)
{
    _settings.beginGroup(groupName);
    {
        widget->resize(_settings.value("size", defaultSize).toSize());
        widget->move(_settings.value("pos", defaultPos).toPoint());
    }
    _settings.endGroup();
}

QString Settings::getPackagesFolder() const
{
    return _settings.value("packages_folder", "").toString();
}

void Settings::setPackagesFolder(const QString& folder)
{
    _settings.setValue("packages_folder", folder);
}
