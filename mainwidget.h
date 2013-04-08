#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "settings.h"

#include <QWidget>

namespace Ui {
class MainWidget;
}

class State;

class MainWidget : public QWidget
{
    Q_OBJECT

    friend class Settings;
    friend class State_SvnCheck;
    
public:
    explicit MainWidget(QWidget* parent = 0);
    ~MainWidget();

protected:
    virtual void closeEvent(QCloseEvent* event);

private:
    State*  _currentState;
private:
    void setState(State *state);
private slots:
    void onStateSucceded();
    void onStateFailed();

private slots:
    void onGetExternalsSucceeded(const QString& externalsString, const QVariant&);
    void onGetExternalsFailed(const QString& errorString, const QVariant&);

public slots:
    void showFolderSelectionDialog();

private:
    Ui::MainWidget* ui;
    Settings        _settings;
    QString         _packagesFolder;


private:
    void getPackagesFolder();
    void getExternals();
    void allowToChooseFolder();
    void getVersions();

private slots:
    void onVersionsReceived();

private:
    void parsePackages(const QString& packagesString);
};

#endif // MAINWIDGET_H
