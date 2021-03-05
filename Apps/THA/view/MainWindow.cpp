#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CasePlanningWidget.h"
#include "AcetabularPrepWidget.h"
#include "IOController.h"

// qt
#include <QFileDialog>
#include <QActionGroup>

// mitk
#include <mitkRenderingManager.h>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent),
  ui(new Ui::MainWindow),
  actionGroup(new QActionGroup(this)),
  casePlanning(new CasePlanningWidget(this)),
  acetabularPrep(new AcetabularPrepWidget(this))
{
  this->ui->setupUi(this);

  connect(this->actionGroup, &QActionGroup::triggered, this, &MainWindow::OnActionsTriggered);
  // the actions should be added in correct order.

  // CasePlanningWidget
  this->actionGroup->addAction(this->ui->action_Pelvis_CT_Landmark);
  this->actionGroup->addAction(this->ui->action_Implant_Planning);
  this->actionGroup->addAction(this->ui->action_Broach_Tracking);
  connect(this->ui->action_Implant_Planning, &QAction::triggered,
    this->casePlanning, &CasePlanningWidget::Action_Implant_Planning_triggered);
  connect(this->ui->action_Broach_Tracking, &QAction::triggered,
    this->casePlanning, &CasePlanningWidget::Action_Broach_Tracking_triggered);
  connect(this->ui->action_Pelvis_CT_Landmark, &QAction::triggered,
    this->casePlanning, &CasePlanningWidget::Action_Pelvis_CT_Landmark_triggered);

  // AcetabularPrepWidget
  this->actionGroup->addAction(this->ui->action_RIO_Registratoin);
  this->actionGroup->addAction(this->ui->action_Pelvis_Checkpoint);
  this->actionGroup->addAction(this->ui->action_Pelvis_Landmark);
  this->actionGroup->addAction(this->ui->action_Pelvis_Registration);
  this->actionGroup->addAction(this->ui->action_Cup_Reaming);
  this->actionGroup->addAction(this->ui->action_Cup_Impaction);
  connect(this->ui->action_RIO_Registratoin, &QAction::triggered,
    this->acetabularPrep, &AcetabularPrepWidget::Action_RIO_Registratoin_triggered);
  connect(this->ui->action_Pelvis_Checkpoint, &QAction::triggered,
    this->acetabularPrep, &AcetabularPrepWidget::Action_Pelvis_Checkpoint_triggered);
  connect(this->ui->action_Pelvis_Landmark, &QAction::triggered,
    this->acetabularPrep, &AcetabularPrepWidget::Action_Pelvis_Landmark_triggered);
  connect(this->ui->action_Pelvis_Registration, &QAction::triggered,
    this->acetabularPrep, &AcetabularPrepWidget::Action_Pelvis_Registration_triggered);
  connect(this->ui->action_Cup_Reaming, &QAction::triggered,
    this->acetabularPrep, &AcetabularPrepWidget::Action_Cup_Reaming_triggered);
  connect(this->ui->action_Cup_Impaction, &QAction::triggered,
    this->acetabularPrep, &AcetabularPrepWidget::Action_Cup_Impaction_triggered);
  
  this->ui->stackedWidget->addWidget(this->casePlanning);
  this->ui->stackedWidget->addWidget(this->acetabularPrep);

  mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  this->ui->levelWindow->SetDataStorage(ds);

  // hide contrast, luminance, zoom buttons
  this->ui->toolButtonContrast->setVisible(false);
  this->ui->toolButtonLuminance->setVisible(false);
  this->ui->toolButtonZoom->setVisible(false);

  QString fileName = QCoreApplication::arguments().size() > 1 ? QCoreApplication::arguments()[1] : "";
  if (!fileName.isEmpty())
  {
    IOController::GetInstance()->LoadScene(fileName);
    this->ui->multiWidget->InitializeMultiWidget();
    this->SetCurrentActionIndex(0);
  }
}

MainWindow::~MainWindow()
{
  delete this->ui;
}

void MainWindow::Test()
{
  MITK_INFO << __func__;
  this->SetCurrentActionIndex(this->actionGroup->actions().indexOf(this->ui->action_Cup_Reaming));
  // this->ui->stackedWidgetViewer->setCurrentWidget(this->ui->pageImage);
}

void MainWindow::SetCurrentActionIndex(int index)
{
  // bounding
  MITK_INFO << __func__;
  MITK_INFO << index;
  if (index >= this->actionGroup->actions().size()) {
    this->currentActionIndex = this->actionGroup->actions().size() - 1;
  }
  else if (index < 0) {
    this->currentActionIndex = 0;
  }
  else {
    this->currentActionIndex = index;
  }

  this->actionGroup->actions()[this->currentActionIndex]->trigger();
}

void MainWindow::on_radioButtonOptions_toggled(bool checked)
{
  if (checked)
  {
    QString fileName =
      QFileDialog::getOpenFileName(this, "Scene", QString(), tr("MITK (*.mitk)"));
    IOController::GetInstance()->LoadScene(fileName);
    this->ui->multiWidget->InitializeMultiWidget();
    this->SetCurrentActionIndex(0);
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

void MainWindow::on_buttonGroupWorkflow_buttonClicked(QAbstractButton *button)
{
  if (button == this->ui->radioButtonPreOpRIOCheck)
  {

  }
  else if (button == this->ui->radioButtonFemoralPrep)
  {

  }
  else if (button == this->ui->radioButtonCasePlanning)
  {
    this->SetCurrentActionIndex(this->actionGroup->actions().indexOf(this->ui->action_Pelvis_CT_Landmark));
  }
  else if (button == this->ui->radioButtonAcetabularPrep)
  {
    this->SetCurrentActionIndex(this->actionGroup->actions().indexOf(this->ui->action_RIO_Registratoin));
  }
  else if (button == this->ui->radioButtonFinalResult)
  {

  }
}

void MainWindow::on_pushButtonNext_clicked(bool checked)
{
  MITK_INFO << __func__;

  this->SetCurrentActionIndex(++this->currentActionIndex);
}

void MainWindow::on_pushButtonBack_clicked(bool checked)
{
  MITK_INFO << __func__;
  this->SetCurrentActionIndex(--this->currentActionIndex);
}

void MainWindow::OnActionsTriggered(QAction * action) const
{
  if (action == this->actionGroup->actions().last()) {
    this->ui->pushButtonNext->setEnabled(false);
    this->ui->pushButtonBack->setEnabled(true);
  }
  else if (action == this->actionGroup->actions().first())
  {
    this->ui->pushButtonNext->setEnabled(true);
    this->ui->pushButtonBack->setEnabled(false);
  }
  else
  {
    this->ui->pushButtonNext->setEnabled(true);
    this->ui->pushButtonBack->setEnabled(true);
  }

  if (this->actionGroup->actions().indexOf(action) <= this->actionGroup->actions().indexOf(this->ui->action_Broach_Tracking)) {
    this->ui->radioButtonCasePlanning->setChecked(true);
    this->ui->stackedWidget->setCurrentWidget(this->casePlanning);
  }
  else if (this->actionGroup->actions().indexOf(action) <= this->actionGroup->actions().indexOf(this->ui->action_Pelvis_Registration)) {
    this->ui->radioButtonAcetabularPrep->setChecked(true);
    this->ui->stackedWidget->setCurrentWidget(this->acetabularPrep);
  }

  if (action == this->ui->action_RIO_Registratoin) {
    this->ui->stackedWidgetViewer->setCurrentWidget(this->ui->pageImage);
    this->ui->imageWidget->SetMode(ImageWidget::MODE::RIO_REGISTRATION);
    this->ui->frameViewAndMode->setVisible(false);
  }
  else if (action == this->ui->action_Pelvis_Checkpoint) {
    this->ui->stackedWidgetViewer->setCurrentWidget(this->ui->pageImage);
    this->ui->imageWidget->SetMode(ImageWidget::MODE::PELVIS_CHECKPOINT);
    this->ui->frameViewAndMode->setVisible(false);
  }
  else if (action == this->ui->action_Pelvis_Landmark) {
    this->ui->stackedWidgetViewer->setCurrentWidget(this->ui->pageMultiWidget);
    this->ui->multiWidget->SetOther(true);
    this->ui->frameViewAndMode->setVisible(false);
  }
  else if (action == this->ui->action_Pelvis_Registration) {
    this->ui->stackedWidgetViewer->setCurrentWidget(this->ui->pageMultiWidget);
    this->ui->multiWidget->SetOther(true);
    this->ui->frameViewAndMode->setVisible(false);
  }
  else if (action == this->ui->action_Cup_Reaming) {
    this->ui->stackedWidgetViewer->setCurrentWidget(this->ui->multiWidget);
    this->ui->multiWidget->SetOther(true);
    this->ui->frameViewAndMode->setVisible(false);
  }
  else if (action == this->ui->action_Cup_Impaction) {
    this->ui->stackedWidgetViewer->setCurrentWidget(this->ui->multiWidget);
    this->ui->multiWidget->SetOther(true);
    this->ui->frameViewAndMode->setVisible(false);
  }
  else {
    this->ui->multiWidget->SetOther(false);
    this->ui->frameViewAndMode->setVisible(true);
    this->ui->stackedWidgetViewer->setCurrentWidget(this->ui->pageMultiWidget);
  }
}
