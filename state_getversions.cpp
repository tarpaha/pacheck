#include "state_getversions.h"
#include "package.h"

State_GetVersions::State_GetVersions(MainWidget* widget, QList<Package*> packages) :
    State_Widget(widget),
    _packages(packages)
{
}

void State_GetVersions::start()
{
    _packagesLeft = _packages.length();
    foreach(Package* package, _packages)
        package->getVersions(this, SLOT(onVersionsReceived()));
}

void State_GetVersions::onVersionsReceived()
{
    if(--_packagesLeft <= 0)
        succeed();
}
