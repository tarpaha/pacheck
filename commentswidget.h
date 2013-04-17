#ifndef COMMENTSWIDGET_H
#define COMMENTSWIDGET_H

#include <QDialog>

namespace Ui {
class CommentsWidget;
}

class CommentsWidget : public QDialog
{
    Q_OBJECT
    
public:
    explicit CommentsWidget(QWidget *parent, const QString& comments);
    ~CommentsWidget();
    
private:
    Ui::CommentsWidget *ui;

private slots:
    void copyAndClose();
};

#endif // COMMENTSWIDGET_H
