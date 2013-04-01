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

private slots:
    void onSvnPresent(const QString &versionString);
    void onSvnAbsent(const QString &errorString);

private slots:
    void onGetExternalsSucceeded(const QString &externalsString);
    void onGetExternalsFailed(const QString &errorString);

private:
    Ui::MainWidget *ui;
    QSettings      _settings;
    QString        _packagesFolder;


private:
    void checkSvnVersion();
    void getPackagesFolder();
    void getExternals();
};

#endif // MAINWIDGET_H
