#include "MainWindow.h"
#include "IOController.h"
#include "ui_MainWindow.h"

// qt
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  this->ui->setupUi(this);
  this->initializeMenu();

  QString fileName = QCoreApplication::arguments().size() > 1
                         ? QCoreApplication::arguments()[1]
                         : "";
  if (!fileName.isEmpty()) {
    IOController::getInstance()->loadScene(fileName);
    this->ui->multiWidget->InitializeMultiWidget();
    // this->SetCurrentActionIndex(0);
  }
}

MainWindow::~MainWindow() { delete this->ui; }

void MainWindow::test() {}

void MainWindow::initializeMenu()
{
    QMenu *menuShow = new QMenu(this);
    this->ui->toolButtonShow->setMenu(menuShow);

    QMenu *menuView = new QMenu(this);
    this->ui->toolButtonView->setMenu(menuView);

    QMenu *toolMenu = new QMenu(this);
    this->ui->toolButtonTools->setMenu(toolMenu);
}