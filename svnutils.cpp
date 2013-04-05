#include "svnutils.h"

static const char* TRUNK = "trunk";
static const char* SEPARATOR = "/";

QStringList SvnUtils::divideSvnPath(const QString &path)
{
    QStringList parts = path.split(SEPARATOR);

    if(parts.last() == TRUNK)
        return processSvnPath(parts, 1);

    return processSvnPath(parts, 2);
}

QStringList SvnUtils::processSvnPath(const QStringList& parts, const int deep)
{
    QStringList basePath;
    basePath << parts.mid(0, parts.length() - deep);

    QStringList currentVersion;
    currentVersion << parts.mid(parts.length() - deep, deep);

    QStringList out;
    out << basePath.join(SEPARATOR);
    out << parts[parts.length() - deep - 1];
    out << currentVersion.join(SEPARATOR);
    return out;
}
