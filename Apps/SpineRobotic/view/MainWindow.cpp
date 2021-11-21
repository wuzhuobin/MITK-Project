#include "MainWindow.h"
#include "IOController.h"
#include "ToolsWidget.h"
#include "ui_MainWindow.h"

// qt
#include <QActionGroup>
#include <QFileDialog>
#include <QMenu>

// mitk
#include <mitkDataNode.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mUi(new Ui::MainWindow),
      mActionGroup(new QActionGroup(this)) {

  this->mUi->setupUi(this);

  this->initializeMenu();
  this->initializeConnection();
  this->actionsTriggered(nullptr);

  QString fileName = QCoreApplication::arguments().size() > 1
                         ? QCoreApplication::arguments()[1]
                         : "";
  if (!fileName.isEmpty()) {
    IOController::getInstance()->loadScene(fileName);
    // this->setCurrentActionIndex(0);
  }
}

MainWindow::~MainWindow() { delete this->mUi; }

void MainWindow::test() {
  // this->actionsTriggered(this->ui->action_Segmentation);
}

void MainWindow::initializeMenu() {
  QMenu *menuShow = new QMenu(this);
  menuShow->addAction(this->mUi->action_Segmentation);
  menuShow->addAction(this->mUi->action_Planning);
  this->mUi->toolButtonShow->setMenu(menuShow);

  QMenu *menuView = new QMenu(this);
  menuView->addAction(this->mUi->action_Whole_Spine_CT);
  menuView->addAction(this->mUi->action_Single_Vertebra_CT);
  menuView->addAction(this->mUi->action_Whole_Spine_X_Ray);
  menuView->addAction(this->mUi->action_Whole_Spine_MRI);
  menuView->addAction(this->mUi->action_Single_Vertebra_MRI);
  this->mUi->toolButtonView->setMenu(menuView);

  QMenu *toolMenu = new QMenu(this);
  toolMenu->addAction(this->mUi->action_Length);
  toolMenu->addAction(this->mUi->action_Angle);
  this->mUi->toolButtonTools->setMenu(toolMenu);
}

void MainWindow::initializeConnection() {

  this->mActionGroup->setExclusive(true);
  this->mActionGroup->addAction(this->mUi->action_Segmentation);
  this->mActionGroup->addAction(this->mUi->action_Planning);
  this->mActionGroup->addAction(this->mUi->action_Whole_Spine_CT);
  this->mActionGroup->addAction(this->mUi->action_Single_Vertebra_CT);
  this->mActionGroup->addAction(this->mUi->action_Whole_Spine_X_Ray);
  this->mActionGroup->addAction(this->mUi->action_Whole_Spine_MRI);
  this->mActionGroup->addAction(this->mUi->action_Single_Vertebra_MRI);
  this->mActionGroup->addAction(this->mUi->action_Length);
  this->mActionGroup->addAction(this->mUi->action_Angle);

  connect(IOController::getInstance(), &IOController::sceneLoaded, this,
          &MainWindow::onsceneLoaded);
  connect(this->mActionGroup, &QActionGroup::triggered, this,
          &MainWindow::actionsTriggered);
}

void MainWindow::on_toolButtonNext_clicked(bool checked) {

  if (++currentAction < this->mActionGroup->actions().size()) {
    actionsTriggered(this->mActionGroup->actions()[currentAction]);
  }
}

void MainWindow::on_toolButtonPrevious_clicked(bool checked) {
  if (--currentAction < 0) {
    actionsTriggered(nullptr);
  } else {
    actionsTriggered(this->mActionGroup->actions()[currentAction]);
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
  this->mUi->stackedWidget->hide();
  this->mUi->segmentationWidget->setEnabled(false);
  this->mUi->planningWidget->setEnabled(false);
  this->mUi->toolsWidget->setEnabled(false);
  // this->ui->multiWidget->enablePlanarLine(false);
  // this->ui->multiWidget->enablePlanarAngle(false);
  if (action == this->mUi->action_Segmentation) {
    this->mUi->stackedWidget->setCurrentWidget(this->mUi->segmentationWidget);
    this->mUi->stackedWidget->show();
    this->mUi->segmentationWidget->setEnabled(true);
    this->mUi->toolButtonPrevious->setEnabled(false);
    this->mUi->toolButtonNext->setEnabled(true);
  } else if (action == this->mUi->action_Planning ||
             action == this->mUi->action_Single_Vertebra_CT) {
    this->mUi->stackedWidget->setCurrentWidget(this->mUi->planningWidget);
    this->mUi->stackedWidget->show();
    this->mUi->planningWidget->setEnabled(true);
    this->mUi->toolButtonPrevious->setEnabled(true);
    this->mUi->toolButtonNext->setEnabled(false);
  } else if (action == this->mUi->action_Whole_Spine_CT) {
    mitk::DataStorage *ds =
        mitk::RenderingManager::GetInstance()->GetDataStorage();
    mitk::DataNode *imageNode = ds->GetNamedNode("image");
    imageNode->SetVisibility(true);
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  } else if (action == this->mUi->action_Length) {
    this->mUi->stackedWidget->setCurrentWidget(this->mUi->toolsWidget);
    this->mUi->stackedWidget->show();
    this->mUi->toolsWidget->setEnabled(true);
    this->mUi->toolsWidget->setCurrentPage(ToolsWidget::Page::Line);
    // this->ui->multiWidget->enablePlanarLine(true);
  } else if (action == this->mUi->action_Angle) {
    this->mUi->stackedWidget->setCurrentWidget(this->mUi->toolsWidget);
    this->mUi->stackedWidget->show();
    this->mUi->toolsWidget->setEnabled(true);
    this->mUi->toolsWidget->setCurrentPage(ToolsWidget::Page::Angle);
    // this->ui->multiWidget->enablePlanarAngle(true);
  }
}

void MainWindow::onsceneLoaded() {
  this->mUi->multiWidget->InitializeMultiWidget();
}