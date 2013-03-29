#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

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

private:
    Ui::MainWidget *ui;

private:
    void checkSvnVersion();
};

#endif // MAINWIDGET_H
