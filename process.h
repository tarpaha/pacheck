#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QProcess>

class Process : public QObject
{
    Q_OBJECT

public:
    static void run(QObject* parent, const QString& command, const char *onDone, const char *onFail);

private:
    explicit Process(const QString& command, QObject* caller, const char *onDone, const char *onFail);

private:
    QProcess*   _process;
    QString     _command;

private slots:
    void start();

private slots:
    void processFinished(int exitCode);
    void processError(QProcess::ProcessError error);
signals:
    void processSucceeded(const QString&);
    void processFailed(const QString&);
};

#endif // PROCESS_H
