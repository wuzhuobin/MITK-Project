#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "IOController.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);

    QString fileName = QCoreApplication::arguments().size() > 1 ? QCoreApplication::arguments()[1] : "";
    if (!fileName.isEmpty())
    {
        IOController::GetInstance()->LoadScene(fileName);
        this->ui->multiWidget->InitializeMultiWidget();
        // this->SetCurrentActionIndex(0);
    }
}

void MainWindow::Test()
{
    delete this->ui;
}