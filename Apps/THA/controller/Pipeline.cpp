#include "Pipeline.h"

#include "MainWindow.h"
#include "PlanningSoftware/login_view.h"
#include "PlanningSoftware/CaseManagementWidget.h"
#include "IOController.h"

// qt
#include <QDir>

Pipeline::Pipeline(QObject *parent)
    : QObject(parent),
      mainWindow(new MainWindow),
      loginView(new LoginView),
      mCaseManagementWidget(new CaseManagementWidget)
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    connect(this->loginView, &LoginView::finished, mCaseManagementWidget, &CaseManagementWidget::show);
    // connect(this->CaseManagementWidget, &CaseManagementWidget::finished, this->mainWindow, &MainWindow::showFullScreen);
    connect(mCaseManagementWidget, &CaseManagementWidget::finished, this, [this]() {
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
    IOController::GetInstance()->LoadScene("THA.mitk");
    this->mainWindow->show();
    // this->mainWindow->Test();
}

Pipeline::~Pipeline()
{
    delete this->mainWindow;
    delete this->loginView;
    delete mCaseManagementWidget;
}
