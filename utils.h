#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Utils
{
public:
    static QString bcStyleSheet(QObject* object, const char* color);
    static void msgBox(const QString &message);
};

#endif // UTILS_H
