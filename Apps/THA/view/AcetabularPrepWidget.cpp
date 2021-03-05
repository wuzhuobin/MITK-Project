#include "AcetabularPrepWidget.h"
#include "ui_AcetabularPrepWidget.h"

// mitk
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkDataStorage.h>
#include <mitkPointSet.h>

AcetabularPrepWidget::AcetabularPrepWidget(QWidget *parent)
    : QStackedWidget(parent)
    , ui(new Ui::AcetabularPrepWidget)
{
  this->ui->setupUi(this);
}

AcetabularPrepWidget::~AcetabularPrepWidget()
{
  delete this->ui;
}

void AcetabularPrepWidget::Action_Pelvis_Checkpoint_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisCheckpoint);
}

void AcetabularPrepWidget::Action_Pelvis_Landmark_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisLandmark);
}

void AcetabularPrepWidget::Action_Pelvis_Registration_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisRegistration);
}

void AcetabularPrepWidget::Action_RIO_Registratoin_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->RIORegistration);
}

void AcetabularPrepWidget::Action_Cup_Reaming_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->CupReaming);
}

void AcetabularPrepWidget::Action_Cup_Impaction_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->CupImpaction);
}

void AcetabularPrepWidget::SetPelvisLandmarkIndex(int index)
{
  if (index < 0) {
    this->pelvisLandmarkIndex = 0;
  }
  else if(index > 3) {
    this->pelvisLandmarkIndex = 3;
  }
  else {
    this->pelvisLandmarkIndex = index;
  }

  mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  mitk::DataNode *posteriorAcetabulumNode = ds->GetNamedNode("posterior_acetabulum");
  mitk::DataNode *anteriorAcetabulumNode = ds->GetNamedNode("anterior_acetabulum");
  mitk::DataNode *superiorAcetabulumNode = ds->GetNamedNode("superior_acetabulum");


  switch (this->pelvisLandmarkIndex)
  {
  case 0:
    this->ui->checkBoxPosteriorAcetabulum->setChecked(true);
    this->ui->checkBoxAnteriorAcetabulum->setChecked(false);
    this->ui->checkBoxSuperiorAcetabulum->setChecked(false);
    this->ui->pushButtonCaptureLandmark->setEnabled(true);
    this->ui->pushButtonClearLandmark->setEnabled(false);
    this->ui->pushButtonClearAllPelvis->setEnabled(false);

    posteriorAcetabulumNode->SetVisibility(true);
    anteriorAcetabulumNode->SetVisibility(false);
    superiorAcetabulumNode->SetVisibility(false);
    break;
  case 1:
    this->ui->checkBoxPosteriorAcetabulum->setChecked(true);
    this->ui->checkBoxAnteriorAcetabulum->setChecked(true);
    this->ui->checkBoxSuperiorAcetabulum->setChecked(false);
    this->ui->pushButtonCaptureLandmark->setEnabled(true);
    this->ui->pushButtonClearLandmark->setEnabled(true);
    this->ui->pushButtonClearAllPelvis->setEnabled(false);

    posteriorAcetabulumNode->SetVisibility(false);
    anteriorAcetabulumNode->SetVisibility(true);
    superiorAcetabulumNode->SetVisibility(false);
    break;
  case 2:
    this->ui->checkBoxPosteriorAcetabulum->setChecked(true);
    this->ui->checkBoxAnteriorAcetabulum->setChecked(true);
    this->ui->checkBoxSuperiorAcetabulum->setChecked(true);
    this->ui->pushButtonCaptureLandmark->setEnabled(true);
    this->ui->pushButtonClearLandmark->setEnabled(true);
    this->ui->pushButtonClearAllPelvis->setEnabled(false);

    posteriorAcetabulumNode->SetVisibility(false);
    anteriorAcetabulumNode->SetVisibility(false);
    superiorAcetabulumNode->SetVisibility(true);
    break;
  case 3:
    this->ui->checkBoxPosteriorAcetabulum->setChecked(true);
    this->ui->checkBoxAnteriorAcetabulum->setChecked(true);
    this->ui->checkBoxSuperiorAcetabulum->setChecked(true);
    this->ui->pushButtonCaptureLandmark->setEnabled(false);
    this->ui->pushButtonClearLandmark->setEnabled(true);
    this->ui->pushButtonClearAllPelvis->setEnabled(true);

    posteriorAcetabulumNode->SetVisibility(false);
    anteriorAcetabulumNode->SetVisibility(false);
    superiorAcetabulumNode->SetVisibility(false);
    break;
  default:
    break;
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void AcetabularPrepWidget::SetPelvisRegistrationIndex(int index)
{
  mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  mitk::PointSet *registrationPoints = ds->GetNamedObject<mitk::PointSet>("registration_points");

  if (index < 0) {
    this->pelvisRegistrationIndex = 0;
  }
  else if (index > registrationPoints->GetSize()) {
    this->pelvisRegistrationIndex = registrationPoints->GetSize();
  }
  else {
    this->pelvisRegistrationIndex = index;
  }

  for (int i = 0; i < registrationPoints->GetSize(); ++i) {
    registrationPoints->SetSelectInfo(i, false);
  }
  if (this->pelvisRegistrationIndex != registrationPoints->GetSize()) {
    registrationPoints->SetSelectInfo(this->pelvisRegistrationIndex, true);
    this->ui->pushButtonCaptureRegistration->setEnabled(true);
    this->ui->pushButtonVerifyRegistration->setEnabled(false);
    if (this->pelvisRegistrationIndex == 0) {
      this->ui->pushButtonClearLastPoint->setEnabled(false);
    }
    else {
      this->ui->pushButtonClearLastPoint->setEnabled(true);
    }
  }
  else {
    this->ui->pushButtonCaptureRegistration->setEnabled(false);
    this->ui->pushButtonVerifyRegistration->setEnabled(true);
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void AcetabularPrepWidget::on_pushButtonCaptureLandmark_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->SetPelvisLandmarkIndex(++this->pelvisLandmarkIndex);
}

void AcetabularPrepWidget::on_pushButtonClearLandmark_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->SetPelvisLandmarkIndex(--this->pelvisLandmarkIndex);
}

void AcetabularPrepWidget::on_pushButtonClearAllPelvis_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->SetPelvisLandmarkIndex(0);
}

void AcetabularPrepWidget::on_pushButtonCaptureRegistration_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->SetPelvisRegistrationIndex(++this->pelvisRegistrationIndex);
}

void AcetabularPrepWidget::on_pushButtonClearLastPoint_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->SetPelvisRegistrationIndex(--this->pelvisRegistrationIndex);
}

void AcetabularPrepWidget::on_pushButtonClearAllPoints_clicked(bool checked)
{
  this->SetPelvisRegistrationIndex(0);
}

void AcetabularPrepWidget::on_pushButtonVerifyRegistration_clicked(bool checked)
{
}

void AcetabularPrepWidget::on_AcetabularPrepWidget_currentChanged(int index)
{
  Q_UNUSED(index);
  mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  if (this->currentWidget() == this->ui->PelvisLandmark) {
    mitk::DataNode *pelvisNode = ds->GetNamedNode("pelvis");
    pelvisNode->SetVisibility(true);
    this->SetPelvisLandmarkIndex(this->pelvisLandmarkIndex);
  }
  else if (this->currentWidget() == this->ui->PelvisRegistration) {
    mitk::DataNode *pelvisRegistrationNode = ds->GetNamedNode("pelvis_registration");
    mitk::DataNode *registrationPoints = ds->GetNamedNode("registration_points");
    pelvisRegistrationNode->SetVisibility(true);
    registrationPoints->SetVisibility(true);
    this->SetPelvisRegistrationIndex(this->pelvisRegistrationIndex);
  }
  else if (this->currentWidget() == this->ui->CupReaming) {
    mitk::DataNode *pelvisNode = ds->GetNamedNode("pelvis");
    pelvisNode->SetVisibility(true);
  }
  mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(
    mitk::RenderingManager::GetInstance()->GetDataStorage());
}