#ifndef SVNUTILS_H
#define SVNUTILS_H

#include <QString>
#include <QStringList>

class SvnUtils
{
public:
    static QStringList divideSvnPath(const QString& path);

private:
    static QStringList processSvnPath(const QStringList& parts, const int deep);
};

#endif // SVNUTILS_H
