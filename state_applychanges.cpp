#include "state_applychanges.h"
#include "package.h"
#include "process.h"
#include "utils.h"

#include <QDebug>
#include <QTextStream>

State_ApplyChanges::State_ApplyChanges(MainWidget* widget, const QList<Package *>& packages, const QString& packagesFolder) :
    State_Widget(widget),
    _packages(packages),
    _packagesFolder(packagesFolder)
{
}

void State_ApplyChanges::start()
{
    QString fileName = prepareFile();

    Process::run(
                this,
                QString("svn propset svn:externals %1 -F %2").arg(_packagesFolder, fileName),
                0,
                PROCESS_SLOT(onPropSetSucceeded),
                PROCESS_SLOT(onPropSetFailed));
}

QString State_ApplyChanges::getComments() const
{
    return _comments.join("\n");
}

QString State_ApplyChanges::prepareFile()
{
    // todo: error reaction here
    if(_file.open())
    {
        _comments.clear();

        QTextStream s_out(&_file);
        foreach(Package* package, _packages)
        {
            s_out << package->selectedPath() << " " << package->name() << "\n";

            if(package->version() != package->selectedVersion())
            {
                QString commentString = QString("%1 version was changed from %2 to %3").arg(
                            package->name(),
                            package->version(),
                            package->selectedVersion());

                _comments.append(commentString);
            }
        }

        _file.close();
    }
    return _file.fileName();
}

void State_ApplyChanges::onPropSetSucceeded(const QString&, const QVariant&)
{
    succeed();
}

void State_ApplyChanges::onPropSetFailed(const QString& errorString, const QVariant&)
{
    Utils::msgBox(QString("Error running \"svn propset svn:externals\" command.\nError string: %1").arg(errorString));
    fail();
}
