#include "svnutils.h"

static const char* TRUNK = "trunk";
static const char* SEPARATOR = "/";

QStringList SvnUtils::splitSvnPath(const QString &path)
{
    QStringList parts = path.split(SEPARATOR);

    if(parts.last() == TRUNK)
        return processSvnPath(parts, 1);

    return processSvnPath(parts, 2);
}

QStringList SvnUtils::splitPackagesList(const QString &packagesList)
{
    QStringList packageLines = packagesList.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

    QStringList result;
    foreach(QString packageLine, packageLines)
    {
        QStringList parts = packageLine.split(' ');

        QString* packageUrl = &parts[0];
        if(!packageUrl->startsWith("https"))
            packageUrl = &parts[1];

        result.append(*packageUrl);
    }

    return result;
}

QString SvnUtils::getLatestBranch(const QStringList& versions)
{
    QStringList digitalBranches = versions.filter(QRegExp("^branches/[0-9_]+$"));
    if(!digitalBranches.isEmpty())
        return digitalBranches.last();
    return 0;
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
