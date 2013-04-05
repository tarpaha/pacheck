#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:
    virtual void closeEvent(QCloseEvent* event);

private slots:
    void onSvnPresent(const QString &versionString, const QVariant &);
    void onSvnAbsent(const QString &errorString, const QVariant&);

private slots:
    void onGetExternalsSucceeded(const QString &externalsString, const QVariant&);
    void onGetExternalsFailed(const QString &errorString, const QVariant &);

public slots:
    void showFolderSelectionDialog();

private:
    Ui::MainWidget *ui;
    QSettings      _settings;
    QString        _packagesFolder;


private:
    void checkSvnVersion();
    void getPackagesFolder();
    void getExternals();
    void allowToChooseFolder();

private:
    void parsePackages(const QString &packagesString);

private:
    // implemented in mainwidget_settings.cpp
    void readSettings();
    void writeSettings();
};

#endif // MAINWIDGET_H
