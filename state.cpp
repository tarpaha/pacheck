#include "state.h"

State::State(QObject *parent) :
    QObject(parent)
{
}

void State::succeed()
{
    emit succeeded();
    disconnect();
}

void State::fail()
{
    emit failed();
    disconnect();
}
