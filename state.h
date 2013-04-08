#ifndef STATE_H
#define STATE_H

#include <QObject>

class State : public QObject
{
    Q_OBJECT
public:
    explicit State(QObject *parent = 0);

signals:
    void succeeded();
    void failed();

public:
    virtual void start() = 0;
};

#endif // STATE_H
