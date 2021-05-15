#include "MainWindow.h"
#include "IOController.h"
#include "ui_MainWindow.h"

// qt
#include <QActionGroup>
#include <QFileDialog>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      actionGroup(new QActionGroup(this)) {

  this->ui->setupUi(this);

  this->initializeMenu();
  this->initializeConnection();
  this->actionsTriggered(nullptr);

  QString fileName = QCoreApplication::arguments().size() > 1
                         ? QCoreApplication::arguments()[1]
                         : "";
  if (!fileName.isEmpty()) {
    IOController::getInstance()->loadScene(fileName);
    // this->SetCurrentActionIndex(0);
  }
}

MainWindow::~MainWindow() { delete this->ui; }

void MainWindow::test() {
  this->actionsTriggered(this->ui->action_Segmentation);
}

void MainWindow::initializeMenu() {
  QMenu *menuShow = new QMenu(this);
  menuShow->addAction(this->ui->action_Segmentation);
  this->ui->toolButtonShow->setMenu(menuShow);

  QMenu *menuView = new QMenu(this);
  menuView->addAction(this->ui->action_Whole_Spine_CT);
  menuView->addAction(this->ui->action_Single_Vertebra_CT);
  menuView->addAction(this->ui->action_Whole_Spine_X_Ray);
  menuView->addAction(this->ui->action_Whole_Spine_MRI);
  menuView->addAction(this->ui->action_Single_Vertebra_MRI);
  this->ui->toolButtonView->setMenu(menuView);

  QMenu *toolMenu = new QMenu(this);
  this->ui->toolButtonTools->setMenu(toolMenu);
}

void MainWindow::initializeConnection() {

  this->actionGroup->setExclusive(true);
  this->actionGroup->addAction(this->ui->action_Segmentation);
  this->actionGroup->addAction(this->ui->action_Planning);

  connect(IOController::getInstance(), &IOController::sceneLoaded, this,
          &MainWindow::onSceneLoaded);
  connect(this->actionGroup, &QActionGroup::triggered, this,
          &MainWindow::actionsTriggered);
}

void MainWindow::on_toolButtonNext_clicked(bool checked) {

  if (++currentAction < this->actionGroup->actions().size()) {
    actionsTriggered(this->actionGroup->actions()[currentAction]);
  }
}

void MainWindow::on_toolButtonPrevious_clicked(bool checked) {
  if (--currentAction < 0) {
    actionsTriggered(nullptr);
  } else {
    actionsTriggered(this->actionGroup->actions()[currentAction]);
  }
}

void MainWindow::on_toolButtonLoad_clicked(bool checked) {
  QString fileName = QFileDialog::getOpenFileName(this, "Scene", QString(),
                                                  tr("MITK (*.mitk)"));
  if (fileName.isEmpty()) {
    return;
  }
  IOController::getInstance()->loadScene(fileName);
}

void MainWindow::on_toolButtonExport_clicked(bool checked) {
  QString fileName =
      QFileDialog::getSaveFileName(this, "Scene", ".mitk", tr("MITK (*.mitk)"));
  if (fileName.isEmpty()) {
    return;
  }
  IOController::getInstance()->saveScene(fileName);
}

void MainWindow::actionsTriggered(QAction *action) {
  this->ui->stackedWidget->hide();
  this->ui->segmentationWidget->setEnabled(false);
  this->ui->planningWidget->setEnabled(false);
  if (action == this->ui->action_Segmentation) {
    this->ui->stackedWidget->setCurrentWidget(this->ui->segmentationWidget);
    this->ui->stackedWidget->show();
    this->ui->segmentationWidget->setEnabled(true);
    this->ui->toolButtonPrevious->setEnabled(false);
    this->ui->toolButtonNext->setEnabled(true);
  } else if (action == this->ui->action_Planning) {
    this->ui->stackedWidget->setCurrentWidget(this->ui->planningWidget);
    this->ui->stackedWidget->show();
    this->ui->planningWidget->setEnabled(true);
    this->ui->toolButtonPrevious->setEnabled(true);
    this->ui->toolButtonNext->setEnabled(false);
  }
}

void MainWindow::onSceneLoaded() {
  this->ui->multiWidget->InitializeMultiWidget();
}