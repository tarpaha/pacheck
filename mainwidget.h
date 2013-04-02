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
};

#endif // MAINWIDGET_H
