#ifndef STATE_H
#define STATE_H

#include <QObject>

class State : public QObject
{
    Q_OBJECT
public:
    explicit State(QObject *parent = 0);

public:
    virtual void start() = 0;

protected:
    void succeed();
    void fail();

signals:
    void succeeded();
    void failed();
};

#endif // STATE_H
