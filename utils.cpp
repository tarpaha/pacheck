#include "utils.h"

#include <QMessageBox>

QString Utils::bcStyleSheet(QObject* object, const char* color)
{
    return QString("%1 { background-color: %2; }").arg(
                object->metaObject()->className(),
                color);
}

void Utils::msgBox(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
