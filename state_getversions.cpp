#include "state_getversions.h"
#include "package.h"
#include "utils.h"

State_GetVersions::State_GetVersions(MainWidget* widget, QList<Package*> packages) :
    State_Widget(widget),
    _packages(packages)
{
}

void State_GetVersions::start()
{
    _packagesLeft = _packages.length();
    foreach(Package* package, _packages)
        package->getVersions(this, SLOT(onVersionsReceived()), SLOT(onError(const QString&)));
}

void State_GetVersions::onVersionsReceived()
{
    if(--_packagesLeft <= 0)
        succeed();
}

void State_GetVersions::onError(const QString& errorString)
{
    Utils::msgBox(QString("Error getting versions.\n") + errorString);
    fail();
}
