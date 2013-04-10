#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class Utils
{
public:
    static QString bcStyleSheet(QObject* object, const char* color);
};

#endif // UTILS_H
