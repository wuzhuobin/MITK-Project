#include "Pipeline.h"

#include "MainWindow.h"
#include "PlanningSoftware/login_view.h"
#include "PlanningSoftware/case_manage_view.h"
#include "IOController.h"

// qt
#include <QDir>

Pipeline::Pipeline(QObject *parent)
    : QObject(parent),
      mainWindow(new MainWindow),
      loginView(new LoginView),
      caseManageView(new CaseManageView)
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    connect(this->loginView, &LoginView::finished, this->caseManageView, &CaseManageView::show);
    // connect(this->caseManageView, &CaseManageView::finished, this->mainWindow, &MainWindow::showFullScreen);
    connect(this->caseManageView, &CaseManageView::finished, this, [this]() {
        this->mainWindow->showFullScreen();
        QCoreApplication::processEvents();
        IOController::GetInstance()->LoadScene("THA.mitk");
    });
}

void Pipeline::start()
{
    this->loginView->show();
}

void Pipeline::test()
{
    this->mainWindow->show();
    // IOController::GetInstance()->LoadScene("THA.mitk");
    // this->mainWindow->Test();
}

Pipeline::~Pipeline()
{
    delete this->mainWindow;
    delete this->loginView;
    delete this->caseManageView;
}
