#include "state_applychanges.h"
#include "package.h"

#include <QDebug>
#include <QTextStream>

State_ApplyChanges::State_ApplyChanges(MainWidget* widget, QList<Package*> packages) :
    State_Widget(widget),
    _packages(packages)
{
}

void State_ApplyChanges::start()
{
    prepareFile();

    /*
    */
}

void State_ApplyChanges::prepareFile()
{
    // todo: error reaction here
    if(_file.open())
    {
        qDebug() << _file.fileName();

        QTextStream s_out(&_file);
        foreach(Package* package, _packages)
        {
            s_out << package->selectedPath() << " " << package->name() << "\n";
        }

        _file.close();
    }
}
