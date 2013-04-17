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
    explicit CommentsWidget(QWidget *parent = 0);
    ~CommentsWidget();
    
private:
    Ui::CommentsWidget *ui;
};

#endif // COMMENTSWIDGET_H
