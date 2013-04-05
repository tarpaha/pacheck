#include "process.h"

#include <QThread>
#include <QDebug>

void Process::run(QObject *parent, const QString &command, const QVariant& data, const char* onDone, const char* onFail)
{
    Process* process = new Process(command, parent, data, onDone, onFail);
    QThread* thread = new QThread(parent);

    QObject::connect(thread, &QThread::started, process, &Process::start);
    QObject::connect(thread, &QThread::finished, process, &Process::deleteLater);

    process->moveToThread(thread);
    thread->start();
}

Process::Process(const QString &command, QObject* caller, const QVariant &data, const char* onDone, const char* onFail) :
    _command(command),
    _data(data)
{
    _process = new QProcess(this);
    _process->setProcessChannelMode(QProcess::MergedChannels);

    QObject::connect(_process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
    QObject::connect(_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

    QObject::connect(this, PROCESS_SIGNAL(processSucceeded), caller, onDone);
    QObject::connect(this, PROCESS_SIGNAL(processFailed), caller, onFail);
}

void Process::start()
{
    _process->start(_command);
    _process->waitForFinished();

    QThread::currentThread()->quit();
}

void Process::processFinished(int exitCode)
{
    if(exitCode == 0)
        emit processSucceeded(_process->readAllStandardOutput(), _data);
    else
        emit processFailed(_process->readAllStandardOutput(), _data);
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

    emit processFailed(errorString, _data);
}
