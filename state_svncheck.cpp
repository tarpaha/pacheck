#include "state_svncheck.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "process.h"

#include <QMessageBox>

State_SvnCheck::State_SvnCheck(MainWidget* widget) :
    State_Widget(widget)
{
}

void State_SvnCheck::start()
{
    Process::run(this,
                 "svn --version --quiet",
                 0,
                 PROCESS_SLOT(onSvnPresent),
                 PROCESS_SLOT(onSvnAbsent));
}

void State_SvnCheck::onSvnPresent(const QString& versionString, const QVariant&)
{
    QStringList ver = versionString.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    _widget->ui->svnVersionLabel->setText(QString("svn %1").arg(ver[0]));

    succeed();
}

void State_SvnCheck::onSvnAbsent(const QString& errorString, const QVariant &)
{
    QMessageBox msgBox;
    msgBox.setText(QString("Error running \"svn\" command.\nError string: %1").arg(errorString));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    fail();
}
