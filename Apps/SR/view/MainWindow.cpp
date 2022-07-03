/**
 * @file MainWindow.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-14
 *
 * @copyright Copyright (c) 2022
 *
 */
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
#include <mitkLogMacros.h>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(std::make_unique<Ui::MainWindow>()),
    mActionGroup(new QActionGroup(this))
{
  mUi->setupUi(this);
  mUi->frameLegacy->setVisible(false);

  initializeMenu();

  mActionGroup->setExclusive(true);

  // SegmentationWidget
  mActionGroup->addAction(mUi->action_Segmentation);

  // CasePlanningWidget
  mActionGroup->addAction(mUi->action_Screw_Planning);
  mActionGroup->addAction(mUi->action_Path_Planning);
  mActionGroup->addAction(mUi->action_Plate_Planning);
  mActionGroup->addAction(mUi->action_Interval_Planning);
  mActionGroup->addAction(mUi->action_Lateral_Planning);
  mActionGroup->addAction(mUi->action_Posterior_Planning);

  // PlanningWidget
  mActionGroup->addAction(mUi->action_Planning);

  // ToolsWidget
  mActionGroup->addAction(mUi->action_Length);
  mActionGroup->addAction(mUi->action_Angle);

  // Others
  mActionGroup->addAction(mUi->action_Whole_Spine_CT);
  mActionGroup->addAction(mUi->action_Single_Vertebra_CT);
  mActionGroup->addAction(mUi->action_Whole_Spine_X_Ray);
  mActionGroup->addAction(mUi->action_Whole_Spine_MRI);
  mActionGroup->addAction(mUi->action_Single_Vertebra_MRI);

  initializeConnection();

  QString fileName = QCoreApplication::arguments().size() > 1
                         ? QCoreApplication::arguments()[1]
                         : "";
  if (!fileName.isEmpty())
  {
    IOController::getInstance()->loadScene(fileName);
  }
  setCurrentActionIndex(0);
}

MainWindow::~MainWindow() = default;

void MainWindow::test()
{
  auto fileName = QApplication::applicationDirPath() + "/SpineRobotic.mitk";
  setCurrentActionIndex(
      mActionGroup->actions().indexOf(mUi->action_Screw_Planning));
  IOController::getInstance()->loadScene(fileName);
}

void MainWindow::initializeMenu()
{
  QMenu* menuShow = new QMenu(this);
  menuShow->addAction(mUi->action_Segmentation);
  menuShow->addAction(mUi->action_Planning);
  mUi->toolButtonShow->setMenu(menuShow);

  QMenu* menuView = new QMenu(this);
  menuView->addAction(mUi->action_Whole_Spine_CT);
  menuView->addAction(mUi->action_Single_Vertebra_CT);
  menuView->addAction(mUi->action_Whole_Spine_X_Ray);
  menuView->addAction(mUi->action_Whole_Spine_MRI);
  menuView->addAction(mUi->action_Single_Vertebra_MRI);
  mUi->toolButtonView->setMenu(menuView);

  QMenu* toolMenu = new QMenu(this);
  toolMenu->addAction(mUi->action_Length);
  toolMenu->addAction(mUi->action_Angle);
  mUi->toolButtonTools->setMenu(toolMenu);
}

void MainWindow::initializeConnection()
{
  connect(mUi->action_Screw_Planning,
          &QAction::triggered,
          mUi->casePlanningWidget,
          &CasePlanningWidget::onActionScrewPlanningTriggered);
  connect(mUi->action_Path_Planning,
          &QAction::triggered,
          mUi->casePlanningWidget,
          &CasePlanningWidget::onActionPathPlanningTriggered);
  connect(mUi->action_Plate_Planning,
          &QAction::triggered,
          mUi->casePlanningWidget,
          &CasePlanningWidget::onActionPlatePlanningTriggered);
  connect(mUi->action_Interval_Planning,
          &QAction::triggered,
          mUi->casePlanningWidget,
          &CasePlanningWidget::onActionIntervalPlanningTriggered);
  connect(mUi->action_Lateral_Planning,
          &QAction::triggered,
          mUi->casePlanningWidget,
          &CasePlanningWidget::onActionLateralPlanningTriggered);
  connect(mUi->action_Posterior_Planning,
          &QAction::triggered,
          mUi->casePlanningWidget,
          &CasePlanningWidget::onActionPosteriorPlanningTriggered);
  connect(IOController::getInstance(),
          &IOController::sceneLoaded,
          this,
          &MainWindow::onSceneLoaded);
  connect(mActionGroup,
          &QActionGroup::triggered,
          this,
          &MainWindow::onActionsTriggered);
}

void MainWindow::setCurrentActionIndex(int index)
{
  // bounding
  MITK_INFO << index;
  if (index >= mActionGroup->actions().size())
  {
    mCurrentActionIndex = mActionGroup->actions().size() - 1;
  }
  else if (index < 0)
  {
    mCurrentActionIndex = 0;
  }
  else
  {
    mCurrentActionIndex = index;
  }
  mActionGroup->actions()[mCurrentActionIndex]->trigger();
}

void MainWindow::on_toolButtonNext_clicked(bool checked)
{
  setCurrentActionIndex(++mCurrentActionIndex);
}

void MainWindow::on_toolButtonPrevious_clicked(bool checked)
{
  setCurrentActionIndex(--mCurrentActionIndex);
}

void MainWindow::on_toolButtonLoad_clicked(bool checked)
{
  QString fileName = QFileDialog::getOpenFileName(
      this, "Scene", QString(), tr("MITK (*.mitk)"));
  if (fileName.isEmpty())
  {
    return;
  }
  IOController::getInstance()->loadScene(fileName);
}

void MainWindow::on_toolButtonExport_clicked(bool checked)
{
  QString fileName =
      QFileDialog::getSaveFileName(this, "Scene", ".mitk", tr("MITK (*.mitk)"));
  if (fileName.isEmpty())
  {
    return;
  }
  IOController::getInstance()->saveScene(fileName);
}

void MainWindow::onActionsTriggered(QAction* action)
{
  if (action == mActionGroup->actions().last())
  {
    mUi->toolButtonNext->setEnabled(false);
    mUi->toolButtonPrevious->setEnabled(true);
  }
  else if (action == mActionGroup->actions().first())
  {
    mUi->toolButtonNext->setEnabled(true);
    mUi->toolButtonPrevious->setEnabled(false);
  }
  else
  {
    mUi->toolButtonNext->setEnabled(true);
    mUi->toolButtonPrevious->setEnabled(true);
  }

  if (mActionGroup->actions().indexOf(action) <=
      mActionGroup->actions().indexOf(mUi->action_Segmentation))
  {
    mUi->stackedWidget->setCurrentWidget(mUi->segmentationWidget);
  }
  else if (mActionGroup->actions().indexOf(action) <=
           mActionGroup->actions().indexOf(mUi->action_Posterior_Planning))
  {
    mUi->stackedWidget->setCurrentWidget(mUi->casePlanningWidget);
  }

  mUi->segmentationWidget->setEnabled(false);
  mUi->planningWidget->setEnabled(false);
  mUi->toolsWidget->setEnabled(false);

  if (action == mUi->action_Segmentation)
  {
    mUi->stackedWidget->setCurrentWidget(mUi->segmentationWidget);
    mUi->stackedWidget->show();
    mUi->segmentationWidget->setEnabled(true);
  }
  else if (action == mUi->action_Planning ||
           action == mUi->action_Single_Vertebra_CT)
  {
    mUi->stackedWidget->setCurrentWidget(mUi->casePlanningWidget);
    mUi->stackedWidget->show();
    mUi->planningWidget->setEnabled(true);
  }
  else if (action == mUi->action_Whole_Spine_CT)
  {
    auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
    auto* imageNode = ds->GetNamedNode("image");
    imageNode->SetVisibility(true);
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  }
  else if (action == mUi->action_Length)
  {
    mUi->stackedWidget->setCurrentWidget(mUi->toolsWidget);
    mUi->stackedWidget->show();
    mUi->toolsWidget->setEnabled(true);
    mUi->toolsWidget->setCurrentPage(ToolsWidget::Page::Line);
    // ui->multiWidget->enablePlanarLine(true);
  }
  else if (action == mUi->action_Angle)
  {
    mUi->stackedWidget->setCurrentWidget(mUi->toolsWidget);
    mUi->stackedWidget->show();
    mUi->toolsWidget->setEnabled(true);
    mUi->toolsWidget->setCurrentPage(ToolsWidget::Page::Angle);
    // ui->multiWidget->enablePlanarAngle(true);
  }
}

void MainWindow::onSceneLoaded()
{
  mUi->multiWidget->InitializeMultiWidget();
}