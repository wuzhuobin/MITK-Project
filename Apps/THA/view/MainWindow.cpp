#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CasePlanningWidget.h"
#include "IOController.h"

// qt
#include <QFileDialog>
#include <QActionGroup>

// mitk
#include <mitkRenderingManager.h>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
  ui(new Ui::MainWindow),
  casePlanning(new CasePlanningWidget(this))
{
  this->ui->setupUi(this);

  connect(this->casePlanning->GetActions(), &QActionGroup::triggered, 
    this, &MainWindow::OnCasePlanningActionsTriggered);
  QList<QAction*>  casePlanningActions = this->casePlanning->GetActions()->actions();
  
  this->ui->stackedWidget->addWidget(this->casePlanning);
  casePlanningActions[0]->trigger();

	mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  this->ui->levelWindow->SetDataStorage(ds);

  // hide contrast, luminance, zoom buttons
  this->ui->toolButtonContrast->setVisible(false);
  this->ui->toolButtonLuminance->setVisible(false);
  this->ui->toolButtonZoom->setVisible(false);

  QString fileName = QCoreApplication::arguments().size() > 1 ? QCoreApplication::arguments()[1] : "";
  if (!fileName.isEmpty())
  {
    IOController::LoadScene(fileName);
    this->ui->multiWidget->InitializeMultiWidget();
  }
}

MainWindow::~MainWindow()
{
  delete this->ui;
}

void MainWindow::Test()
{
  this->ui->multiWidget->SetMode(THAStdMultiWidget::MODES::MODE_CUP_PLAN);
  this->ui->multiWidget->SetView(THAStdMultiWidget::VIEWS::VIEW_3D_SLICER);
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

void MainWindow::on_buttonGroupMode_buttonClicked(QAbstractButton *button) const
{
  MITK_INFO << __func__ << ' ' << button->objectName().toStdString();
  for(QAbstractButton *button_: this->ui->buttonGroupMode->buttons())
  {
    if(button_ != button)
    {
      button_->setChecked(false);
    }
  }

  if(this->ui->toolButtonPrepOpMode->isChecked())
  {
    this->ui->multiWidget->SetMode(THAStdMultiWidget::MODES::MODE_PRE_OP);
  }
  else if (this->ui->toolButtonCupPlanMode->isChecked())
  {
    this->ui->multiWidget->SetMode(THAStdMultiWidget::MODES::MODE_CUP_PLAN);
  }
  else if (this->ui->toolButtonStemPlanMode->isChecked())
  {
    this->ui->multiWidget->SetMode(THAStdMultiWidget::MODES::MODE_STEM_PLAN);
  }
  else if (this->ui->toolButtonReducedMode->isChecked())
  {
    this->ui->multiWidget->SetMode(THAStdMultiWidget::MODES::MODE_REDUCED);
  }
  else
  {
    this->ui->multiWidget->SetMode(THAStdMultiWidget::MODES::MODE_DEFAULT);
  }
}

void MainWindow::on_buttonGroupView_buttonClicked(QAbstractButton *button) const
{
  MITK_INFO << __func__ << ' ' << button->objectName().toStdString();
  for(QAbstractButton *button_: this->ui->buttonGroupView->buttons())
  {
    if(button_ != button)
    {
      button_->setChecked(false);
    }
  }

  if(this->ui->toolButton3DSlicerView->isChecked())
  {
    this->ui->multiWidget->SetView(THAStdMultiWidget::VIEWS::VIEW_3D_SLICER);
  }
  else if (this->ui->toolButtonCTView->isChecked())
  {
    this->ui->multiWidget->SetView(THAStdMultiWidget::VIEWS::VIEW_CT);
  }
  else if (this->ui->toolButtonReamingView->isChecked())
  {
    this->ui->multiWidget->SetView(THAStdMultiWidget::VIEWS::VIEW_REAMING);
  }
  else if (this->ui->toolButtonXRayView->isChecked())
  {
    this->ui->multiWidget->SetView(THAStdMultiWidget::VIEWS::VIEW_X_RAY);
  }
  else
  {
    this->ui->multiWidget->SetView(THAStdMultiWidget::VIEWS::VIEW_DEFAULT);
  }
}

void MainWindow::on_pushButtonNext_clicked(bool checked)
{
  // int numOfActions = 0;
  // numOfActions += this->casePlanning->GetActions()->actions().size();
  // QAction *action = this->actions->actions()[++this->currentActionIndex];
  // action->trigger();
  this->currentActionIndex++;
  int index = this->currentActionIndex;
  if (index < this->casePlanning->GetActions()->actions().size())
  {
    this->casePlanning->GetActions()->actions()[index]->trigger();
  }
  
  MITK_INFO << __func__;
}

void MainWindow::on_pushButtonBack_clicked(bool checked)
{
  // QAction *action = this->actions->actions()[--this->currentActionIndex];
  // action->trigger();
  this->currentActionIndex--;
  int index = this->currentActionIndex;
  if (index < this->casePlanning->GetActions()->actions().size())
  {
    this->casePlanning->GetActions()->actions()[index]->trigger();
  }

  MITK_INFO << __func__;
}

void MainWindow::OnCasePlanningActionsTriggered(QAction *action) const
{
  this->ui->stackedWidget->setCurrentWidget(this->casePlanning);
  if (this->casePlanning->GetActions()->actions().first() == action)
  {
    this->ui->pushButtonNext->setEnabled(true);
    this->ui->pushButtonBack->setEnabled(false);
  }
  else if (this->casePlanning->GetActions()->actions().last() == action)
  {
    this->ui->pushButtonNext->setEnabled(false);
    this->ui->pushButtonBack->setEnabled(true);
  }
  else
  {
    this->ui->pushButtonNext->setEnabled(true);
    this->ui->pushButtonBack->setEnabled(true);
  }
  
}