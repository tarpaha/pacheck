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
    _settings.beginGroup("main_widget");
    {
        _mainWidget->resize(_settings.value("size", WINDOW_DEFAULT_SIZE).toSize());
        _mainWidget->move(_settings.value("pos", WINDOW_DEFAULT_POSITION).toPoint());
    }
    _settings.endGroup();

    _settings.beginGroup("table");
    {
        _mainWidget->ui->table->setColumnWidth(0, _settings.value("first_column_width", TABLE_DEFAULT_FIRST_COLUMN_WIDTH).toInt());
    }
    _settings.endGroup();
}

void Settings::saveUI()
{
    _settings.beginGroup("main_widget");
    {
        _settings.setValue("size", _mainWidget->size());
        _settings.setValue("pos", _mainWidget->pos());
    }
    _settings.endGroup();

    _settings.beginGroup("table");
    {
        _settings.setValue("first_column_width", _mainWidget->ui->table->columnWidth(0));
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
