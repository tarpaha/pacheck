#ifndef SVNCHECKSTATE_H
#define SVNCHECKSTATE_H

#include "state_widget.h"

class State_SvnCheck : public State_Widget
{
    Q_OBJECT

public:
    explicit State_SvnCheck(MainWidget* widget);

public:
    virtual void start();

private slots:
    void onSvnPresent(const QString&, const QVariant&);
    void onSvnAbsent(const QString&, const QVariant&);
};

#endif // SVNCHECKSTATE_H
