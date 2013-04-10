#ifndef SVNUTILS_H
#define SVNUTILS_H

#include <QString>
#include <QStringList>

class SvnUtils
{
public:
    static QStringList splitSvnPath(const QString& path);
    static QStringList splitPackagesList(const QString& packagesList);
    static QString getLatestBranch(const QStringList& versions);

private:
    static QStringList processSvnPath(const QStringList& parts, const int deep);
};

#endif // SVNUTILS_H
