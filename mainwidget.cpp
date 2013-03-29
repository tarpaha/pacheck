#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "process.h"

#include <QDebug>
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    checkSvnVersion();
}

void MainWidget::checkSvnVersion()
{
    Process::run(this, "svna --version --quiet", SLOT(onSvnPresent(const QString&)), SLOT(onSvnAbsent(const QString&)));
}

void MainWidget::onSvnPresent(const QString& versionString)
{
    qDebug() << "SVN present: " << versionString;

    QStringList ver = versionString.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    ui->statusLabel->setText(QString("Using SVN ver. %1").arg(ver[0]));
}

void MainWidget::onSvnAbsent(const QString& errorString)
{
    qDebug() << "Error getting SVN version: " << errorString;

    QMessageBox msgBox;
    msgBox.setText(QString("Error running \"svn\" command.\nError string: %1").arg(errorString));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
}

MainWidget::~MainWidget()
{
    delete ui;
}
