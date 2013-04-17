#include "commentswidget.h"
#include "ui_commentswidget.h"

#include <QClipboard>

CommentsWidget::CommentsWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommentsWidget)
{
    ui->setupUi(this);

    ui->commentsTextEdit->setText("Olala");

    QObject::connect(ui->copyAndCloseButton, SIGNAL(clicked()), this, SLOT(copyAndClose()));
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

CommentsWidget::~CommentsWidget()
{
    delete ui;
}

void CommentsWidget::copyAndClose()
{
    //QApplication::clipboard()->setText(ui->commentsTextEdit->pla
    close();
}
