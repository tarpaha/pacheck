#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "settings.h"

#include <QWidget>

namespace Ui {
class MainWidget;
}

class State;
class Package;

class MainWidget : public QWidget
{
    Q_OBJECT

    friend class Settings;
    friend class State_SvnCheck;
    friend class State_GetPackagesFolder;
    
public:
    explicit MainWidget(QWidget* parent = 0);
    ~MainWidget();

protected:
    virtual void closeEvent(QCloseEvent* event);

private:
    State*  _currentState;
private:
    void setState(State *state, void (MainWidget::* onSucceeded)(), void (MainWidget::* onFailed)());

private slots:
    void onSvnPresent();
    void onSvnAbsent();

private slots:
    void OnPackagesListReceived();
    void OnPackagesListFailed();

private slots:
    void onFolderSelected();
private:
    void fillPackagesList(const QStringList& packagesList);
    void applyPackagesToTable();

private slots:
    void onVersionsReceived();
    void onVersionChanged();

private:
    Ui::MainWidget* ui;
    Settings        _settings;
    QString         _packagesFolder;
    QList<Package*> _packages;

private:
    bool anyPackageVersionChanged();

private slots:
    void onApplyButtonPressed();
};

#endif // MAINWIDGET_H
