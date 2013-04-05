#include "mainwidget.h"
#include "ui_mainwidget.h"

static const QSize  WINDOW_DEFAULT_SIZE     = QSize(400, 400);
static const QPoint WINDOW_DEFAULT_POSITION = QPoint(400, 200);

static const int TABLE_DEFAULT_FIRST_COLUMN_WIDTH = 200;

void MainWidget::readSettings()
{
    _settings.beginGroup("main_widget");
    {
        resize(_settings.value("size", WINDOW_DEFAULT_SIZE).toSize());
        move(_settings.value("pos", WINDOW_DEFAULT_POSITION).toPoint());
    }
    _settings.endGroup();

    _settings.beginGroup("table");
    {
        ui->table->setColumnWidth(0, _settings.value("first_column_width", TABLE_DEFAULT_FIRST_COLUMN_WIDTH).toInt());
    }
    _settings.endGroup();
}

void MainWidget::writeSettings()
{
    _settings.beginGroup("main_widget");
    {
        _settings.setValue("size", size());
        _settings.setValue("pos", pos());
    }
    _settings.endGroup();

    _settings.beginGroup("table");
    {
        _settings.setValue("first_column_width", ui->table->columnWidth(0));
    }
    _settings.endGroup();
}
