#include "utils.h"

QString Utils::bcStyleSheet(QObject* object, const char* color)
{
    return QString("%1 { background-color: %2; }").arg(
                object->metaObject()->className(),
                color);
}
