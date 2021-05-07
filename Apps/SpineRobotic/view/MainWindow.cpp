#include "MainWindow.h"
#include "IOController.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  this->ui->setupUi(this);



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
}