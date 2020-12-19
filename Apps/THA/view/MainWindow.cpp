#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CasePlanningWidget.h"
#include "IOController.h"

// qt
#include <QFileDialog>

// mitk

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  this->ui->setupUi(this);
  this->ui->stackedWidget->addWidget(new CasePlanningWidget(this));
  this->ui->stackedWidget->setCurrentIndex(1);

  QString fileName = QCoreApplication::arguments().size() > 1 ? QCoreApplication::arguments()[1] : "";
  if (!fileName.isEmpty())
  {
    IOController::LoadScene(fileName);
  }
}

MainWindow::~MainWindow()
{
  delete this->ui;
}

void MainWindow::on_radioButtonOptions_toggled(bool checked)
{
  if (checked)
  {
    QString fileName =
      QFileDialog::getOpenFileName(this, "Scene", QString(), tr("MITK (*.mitk)"));
    IOController::LoadScene(fileName);
    this->ui->multiWidget->InitializeMultiWidget();
  }
}