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

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mActionGroup(new QActionGroup(this))
{
  mUi->setupUi(this);

  initializeMenu();
  initializeConnection();
  actionsTriggered(nullptr);

  QString fileName = QCoreApplication::arguments().size() > 1
                         ? QCoreApplication::arguments()[1]
                         : "";
  if (!fileName.isEmpty())
  {
    IOController::getInstance()->loadScene(fileName);
    // setCurrentActionIndex(0);
  }
}

MainWindow::~MainWindow()
{
  delete mUi;
}

void MainWindow::test()
{
  auto fileName = QApplication::applicationDirPath() + "/SpineRobotic.mitk";
  IOController::getInstance()->loadScene(fileName);
  // actionsTriggered(ui->action_Segmentation);
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
  mActionGroup->setExclusive(true);
  mActionGroup->addAction(mUi->action_Segmentation);
  mActionGroup->addAction(mUi->action_Screw_Planning);
  mActionGroup->addAction(mUi->action_Path_Planning);
  mActionGroup->addAction(mUi->action_Plate_Planning);
  mActionGroup->addAction(mUi->action_Interval_Planning);
  mActionGroup->addAction(mUi->action_Lateral_Planning);
  mActionGroup->addAction(mUi->action_Posterior_Planning);
  mActionGroup->addAction(mUi->action_Planning);
  mActionGroup->addAction(mUi->action_Whole_Spine_CT);
  mActionGroup->addAction(mUi->action_Single_Vertebra_CT);
  mActionGroup->addAction(mUi->action_Whole_Spine_X_Ray);
  mActionGroup->addAction(mUi->action_Whole_Spine_MRI);
  mActionGroup->addAction(mUi->action_Single_Vertebra_MRI);
  mActionGroup->addAction(mUi->action_Length);
  mActionGroup->addAction(mUi->action_Angle);

  connect(IOController::getInstance(),
          &IOController::sceneLoaded,
          this,
          &MainWindow::onsceneLoaded);
  connect(mActionGroup,
          &QActionGroup::triggered,
          this,
          &MainWindow::actionsTriggered);
}

void MainWindow::on_toolButtonNext_clicked(bool checked)
{
  if (++currentActionIndex < mActionGroup->actions().size())
  {
    actionsTriggered(mActionGroup->actions()[currentActionIndex]);
  }
}

void MainWindow::on_toolButtonPrevious_clicked(bool checked)
{
  if (--currentActionIndex < 0)
  {
    actionsTriggered(nullptr);
  }
  else
  {
    actionsTriggered(mActionGroup->actions()[currentActionIndex]);
  }
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

void MainWindow::actionsTriggered(QAction* action)
{
  mUi->stackedWidget->hide();
  mUi->segmentationWidget->setEnabled(false);
  mUi->planningWidget->setEnabled(false);
  mUi->toolsWidget->setEnabled(false);
  // ui->multiWidget->enablePlanarLine(false);
  // ui->multiWidget->enablePlanarAngle(false);
  if (action == mUi->action_Segmentation)
  {
    mUi->stackedWidget->setCurrentWidget(mUi->segmentationWidget);
    mUi->stackedWidget->show();
    mUi->segmentationWidget->setEnabled(true);
    mUi->toolButtonPrevious->setEnabled(false);
    mUi->toolButtonNext->setEnabled(true);
  }
  else if (action == mUi->action_Planning ||
           action == mUi->action_Single_Vertebra_CT)
  {
    mUi->stackedWidget->setCurrentWidget(mUi->planningWidget);
    mUi->stackedWidget->show();
    mUi->planningWidget->setEnabled(true);
    mUi->toolButtonPrevious->setEnabled(true);
    mUi->toolButtonNext->setEnabled(false);
  }
  else if (action == mUi->action_Whole_Spine_CT)
  {
    mitk::DataStorage* ds =
        mitk::RenderingManager::GetInstance()->GetDataStorage();
    mitk::DataNode* imageNode = ds->GetNamedNode("image");
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

void MainWindow::onsceneLoaded()
{
  mUi->multiWidget->InitializeMultiWidget();
}