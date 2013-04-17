#include "commentswidget.h"
#include "ui_commentswidget.h"

#include <QClipboard>

CommentsWidget::CommentsWidget(QWidget *parent, const QString& comments) :
    QDialog(parent),
    ui(new Ui::CommentsWidget)
{
    ui->setupUi(this);

    ui->commentsTextEdit->setPlainText(comments);

    QObject::connect(ui->copyAndCloseButton, SIGNAL(clicked()), this, SLOT(copyAndClose()));
    QObject::connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

CommentsWidget::~CommentsWidget()
{
    delete ui;
}

void CommentsWidget::copyAndClose()
{
    QApplication::clipboard()->setText(ui->commentsTextEdit->toPlainText());
    close();
}
