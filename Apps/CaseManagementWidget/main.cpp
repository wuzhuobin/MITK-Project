#include "PlanningSoftware/CaseManagementWidget.h"

// qt
#include <QApplication>

int main (int argc, char * argv[])
{
    QApplication app(argc, argv);

    CaseManagementWidget w;
    w.show();

    return app.exec();
}