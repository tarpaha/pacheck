#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QProcess>
#include <QVariant>
#include <QSemaphore>

#define PROCESS_SIGNAL(slotName) SIGNAL(slotName(const QString&, const QVariant&))
#define PROCESS_SLOT(slotName) SLOT(slotName(const QString&, const QVariant&))

class Process : public QObject
{
    Q_OBJECT

public:
    static void run(QObject* parent, const QString& command, const QVariant& data, const char *onDone, const char *onFail);

private:
    explicit Process(const QString& command, QObject* caller, const QVariant& data, const char *onDone, const char *onFail);

private:
    static QSemaphore _semaphore;

private:
    QProcess*   _process;
    QString     _command;
    QVariant    _data;

private slots:
    void start();

private slots:
    void processFinished(int exitCode);
    void processError(QProcess::ProcessError error);
signals:
    void processSucceeded(const QString&, const QVariant&);
    void processFailed(const QString&, const QVariant&);
};

#endif // PROCESS_H
