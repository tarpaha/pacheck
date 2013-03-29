#include "process.h"

void Process::run(QObject *parent, const QString &command, const char* onDone, const char* onFail)
{
    new Process(parent, command, onDone, onFail);
}

Process::Process(QObject *parent, const QString &command, const char* onDone, const char* onFail) :
    QObject(parent),
    _command(command)
{
    _process = new QProcess(this);

    QObject::connect(_process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
    QObject::connect(_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

    if(onDone != 0)
        QObject::connect(this, SIGNAL(processSucceeded(const QString&)), parent, onDone);

    if(onFail != 0)
        QObject::connect(this, SIGNAL(processFailed(const QString&)), parent, onFail);

    _process->start(_command);
}

void Process::processFinished(int exitCode)
{
    if(exitCode == 0)
        emit processSucceeded(_process->readAllStandardOutput());
    else
        emit processFailed(_process->readAllStandardError());
}

void Process::processError(QProcess::ProcessError error)
{
    QString errorString = "ProcessError: ";
    switch(error)
    {
    case QProcess::FailedToStart:
        errorString += "Failed to start";
        break;
    case QProcess::Crashed:
        errorString += "Crashed";
        break;
    default:
        errorString += "unknown";
    }

    emit processFailed(errorString);
}
