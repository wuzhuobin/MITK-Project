#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "CasePlanningWidget.hpp"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
  mpUi(new Ui::MainWindow)
{
  this->mpUi->setupUi(this);
  this->mpUi->stackedWidget->addWidget(new CasePlanningWidget(this));
  this->mpUi->stackedWidget->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
  delete this->mpUi;
}