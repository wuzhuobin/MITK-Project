#include "PlanningSoftware/case_manage_view.h"

// qt
#include <QApplication>

int main (int argc, char * argv[])
{
    QApplication app(argc, argv);

    CaseManageView w;
    w.show();

    return app.exec();
}