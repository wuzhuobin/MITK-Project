#include "AcetabularPrepWidget.h"
#include "ui_AcetabularPrepWidget.h"
#include "AcetabularPrepView.h"
#include "mitkReamingFilter.h"

// mitk
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkDataStorage.h>
#include <mitkPointSet.h>
#include <mitkSurface.h>
#include <mitkLookupTableProperty.h>
#include <mitkLookupTable.h>
#include <mitkVtkScalarModeProperty.h>

// vtk
#include <vtkLookupTable.h>

AcetabularPrepWidget::AcetabularPrepWidget(AcetabularPrepView *view, QWidget *parent)
    : QStackedWidget(parent)
    , mUi(new Ui::AcetabularPrepWidget)
    , mView(view)
    , reamingFilter(mitk::ReamingFilter::New())
{
  mUi->setupUi(this);
}

AcetabularPrepWidget::~AcetabularPrepWidget()
{
  delete mUi;
}

void AcetabularPrepWidget::Action_Pelvis_Checkpoint_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(mUi->PelvisCheckpoint);
}

void AcetabularPrepWidget::Action_Pelvis_Landmark_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(mUi->PelvisLandmark);
}

void AcetabularPrepWidget::Action_Pelvis_Registration_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(mUi->PelvisRegistration);
}

void AcetabularPrepWidget::Action_RIO_Registratoin_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(mUi->RIORegistration);
}

void AcetabularPrepWidget::Action_Cup_Reaming_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(mUi->CupReaming);
}

void AcetabularPrepWidget::Action_Cup_Impaction_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(mUi->CupImpaction);
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
    mUi->checkBoxPosteriorAcetabulum->setChecked(true);
    mUi->checkBoxAnteriorAcetabulum->setChecked(false);
    mUi->checkBoxSuperiorAcetabulum->setChecked(false);
    mUi->pushButtonCaptureLandmark->setEnabled(true);
    mUi->pushButtonClearLandmark->setEnabled(false);
    mUi->pushButtonClearAllPelvis->setEnabled(false);

    posteriorAcetabulumNode->SetVisibility(true);
    anteriorAcetabulumNode->SetVisibility(false);
    superiorAcetabulumNode->SetVisibility(false);
    break;
  case 1:
    mUi->checkBoxPosteriorAcetabulum->setChecked(true);
    mUi->checkBoxAnteriorAcetabulum->setChecked(true);
    mUi->checkBoxSuperiorAcetabulum->setChecked(false);
    mUi->pushButtonCaptureLandmark->setEnabled(true);
    mUi->pushButtonClearLandmark->setEnabled(true);
    mUi->pushButtonClearAllPelvis->setEnabled(false);

    posteriorAcetabulumNode->SetVisibility(false);
    anteriorAcetabulumNode->SetVisibility(true);
    superiorAcetabulumNode->SetVisibility(false);
    break;
  case 2:
    mUi->checkBoxPosteriorAcetabulum->setChecked(true);
    mUi->checkBoxAnteriorAcetabulum->setChecked(true);
    mUi->checkBoxSuperiorAcetabulum->setChecked(true);
    mUi->pushButtonCaptureLandmark->setEnabled(true);
    mUi->pushButtonClearLandmark->setEnabled(true);
    mUi->pushButtonClearAllPelvis->setEnabled(false);

    posteriorAcetabulumNode->SetVisibility(false);
    anteriorAcetabulumNode->SetVisibility(false);
    superiorAcetabulumNode->SetVisibility(true);
    break;
  case 3:
    mUi->checkBoxPosteriorAcetabulum->setChecked(true);
    mUi->checkBoxAnteriorAcetabulum->setChecked(true);
    mUi->checkBoxSuperiorAcetabulum->setChecked(true);
    mUi->pushButtonCaptureLandmark->setEnabled(false);
    mUi->pushButtonClearLandmark->setEnabled(true);
    mUi->pushButtonClearAllPelvis->setEnabled(true);

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
    mUi->pushButtonCaptureRegistration->setEnabled(true);
    mUi->pushButtonVerifyRegistration->setEnabled(false);
    if (this->pelvisRegistrationIndex == 0) {
      mUi->pushButtonClearLastPoint->setEnabled(false);
    }
    else {
      mUi->pushButtonClearLastPoint->setEnabled(true);
    }
  }
  else {
    mUi->pushButtonCaptureRegistration->setEnabled(false);
    mUi->pushButtonVerifyRegistration->setEnabled(true);
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void AcetabularPrepWidget::transformReamer(unsigned int orientation, double t, double r)
{
  auto *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto *surface = ds->GetNamedObject<mitk::Surface>("reamer");
  auto *matrix = surface->GetGeometry()->GetVtkMatrix();
  auto center = surface->GetGeometry()->GetCenter();
  auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply();
  transform->SetMatrix(matrix);
  transform->Translate(-center[0], -center[1], -center[2]);
  transform->RotateY(orientation == ORIENTATION::Y ? r: 0);
  transform->RotateX(orientation == ORIENTATION::X ? r: 0);
  transform->RotateZ(orientation == ORIENTATION::Z ? r: 0);
  transform->Translate(
    orientation == ORIENTATION::X ? t: 0,
    orientation == ORIENTATION::Y ? t: 0,
    orientation == ORIENTATION::Z ? t: 0
  );
  transform->Translate(center[0], center[1], center[2]);
  transform->Update();
  surface->GetGeometry()->SetIndexToWorldTransformByVtkMatrixWithoutChangingSpacing(transform->GetMatrix());

  this->realReamer->GetGeometry()->SetIndexToWorldTransformByVtkMatrixWithoutChangingSpacing(transform->GetMatrix());
  this->realReamer->Modified();

  // auto *reamerTrajectory = ds->GetNamedObject<mitk::Surface>("reamer_trajectory");
  // reamerTrajectory->GetGeometry()->SetIndexToWorldTransformByVtkMatrixWithoutChangingSpacing(transform->GetMatrix());
  // reamerTrajectory->Modified();

  // this->reamingFilter->SetReamer(this->realReamer);
  // this->reamingFilter->SetShell(reamerTrajectory);
  this->reamingFilter->Update();

  // auto *reamingPelvisNode = ds->GetNamedNode("reaming_pelvis");
  // reamingPelvisNode->SetData(this->reamingFilter->GetOutput());


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

void AcetabularPrepWidget::on_pushButtonXRA_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::X, 0, 10);
}

void AcetabularPrepWidget::on_pushButtonXRM_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::X, 0, -10);
}

void AcetabularPrepWidget::on_pushButtonXTA_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::X, 10, 0);
}

void AcetabularPrepWidget::on_pushButtonXTM_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::X, -10, 0);
}

void AcetabularPrepWidget::on_pushButtonYRA_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::Y, 0, 10);
}

void AcetabularPrepWidget::on_pushButtonYRM_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::Y, 0, -10);
}

void AcetabularPrepWidget::on_pushButtonYTA_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::Y, 10, 0);
}

void AcetabularPrepWidget::on_pushButtonYTM_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::Y, -10, 0);
}

void AcetabularPrepWidget::on_pushButtonZRA_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::Z, 0, 10);
}

void AcetabularPrepWidget::on_pushButtonZRM_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::Z, 0, -10);
}

void AcetabularPrepWidget::on_pushButtonZTA_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::Z, 10, 0);
}

void AcetabularPrepWidget::on_pushButtonZTM_clicked(bool checked)
{
  Q_UNUSED(checked);
  this->transformReamer(ORIENTATION::Z, -10, 0);
}

void AcetabularPrepWidget::on_AcetabularPrepWidget_currentChanged(int index)
{
  Q_UNUSED(index);

  if (currentWidget() == mUi->pageAcetabularPinInstallationAndAcquisition)
  {
    mView->updateView(AcetabularPrepView::View::AcetabularPinInstallationAndAcquisition);
  }
  else if (currentWidget() == mUi->pageAcetabularLandmarkAcquisition)
  {
    mView->updateView(AcetabularPrepView::View::AcetabularLandmarkAcquisition);
  }
  // else if (currentWidget() == mUi)
  // if (this->currentWidget() == mUi)

  //mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  //if (this->currentWidget() == mUi->PelvisLandmark) {
  //  mitk::DataNode *pelvisNode = ds->GetNamedNode("pelvis");
  //  pelvisNode->SetVisibility(true);
  //  this->SetPelvisLandmarkIndex(this->pelvisLandmarkIndex);
  //}
  //else if (this->currentWidget() == mUi->PelvisRegistration) {
  //  mitk::DataNode *pelvisRegistrationNode = ds->GetNamedNode("pelvis_registration");
  //  mitk::DataNode *registrationPoints = ds->GetNamedNode("registration_points");
  //  pelvisRegistrationNode->SetVisibility(true);
  //  registrationPoints->SetVisibility(true);
  //  this->SetPelvisRegistrationIndex(this->pelvisRegistrationIndex);
  //}
  //else if (this->currentWidget() == mUi->CupReaming) {
  //  // mitk::DataNode *pelvisNode = ds->GetNamedNode("pelvis");
  //  // pelvisNode->SetVisibility(true);

  //  mitk::DataNode *reamerTrajectoryNode = ds->GetNamedNode("reamer_trajectory");
  //  // reamerTrajectoryNode->SetVisibility(true);
  //  reamerTrajectoryNode->SetVisibility(false);
  //  reamerTrajectoryNode->SetOpacity(0.2);
  //  reamerTrajectoryNode->SetColor(1, 0, 0);

  //  mitk::DataNode *reamerNode = ds->GetNamedNode("reamer");
  //  reamerNode->SetVisibility(true);
  //  reamerNode->SetOpacity(0.2);
  //  reamerNode->SetColor(0, 0, 1);

  //  mitk::DataNode::Pointer reamingPelvisNode = ds->GetNamedNode("reaming_pelvis");
  //  if (!reamingPelvisNode)
  //  {
  //    reamingPelvisNode = mitk::DataNode::New();
  //    reamingPelvisNode->SetName("reaming_pelvis");

  //    ds->Add(reamingPelvisNode);
  //  }
  //  auto *overlay = ds->GetNamedObject<mitk::Image>("overlay");
  //  auto *reamerTrajectory = ds->GetNamedObject<mitk::Surface>("reamer_trajectory");

  //  this->realReamer = reamerTrajectory->Clone();
  //  this->realReamer->GetGeometry()->SetIndexToWorldTransformWithoutChangingSpacing(
  //    reamerNode->GetData()->GetGeometry()->GetIndexToWorldTransform()
  //  );
  //  auto acetabularShell = ds->GetNamedObject<mitk::Surface>("acetabular_shell");
  //  reamerTrajectory->GetGeometry()->SetIndexToWorldTransformWithoutChangingSpacing(
  //    acetabularShell->GetGeometry()->GetIndexToWorldTransform()
  //  );

  //  // auto realReamerNode = mitk::DataNode::New();
  //  // realReamerNode->SetName("real_reamer");
  //  // realReamerNode->SetData(this->realReamer);
  //  // realReamerNode->SetOpacity(0.2);
  //  // ds->Add(realReamerNode);

  //  this->reamingFilter->SetInput(overlay);
  //  this->reamingFilter->SetShell(reamerTrajectory);
  //  this->reamingFilter->SetReamer(this->realReamer);
  //  this->reamingFilter->SetThreshold(3);
  //  this->reamingFilter->SmoothOn();
  //  this->reamingFilter->Update();

  //  auto vtkLut = vtkSmartPointer<vtkLookupTable>::New();
  //  vtkLut->SetTableRange(0, 3);
  //  vtkLut->SetNumberOfTableValues(4);
  //  vtkLut->SetTableValue(0, 0, 0, 1, 0);
  //  vtkLut->SetTableValue(1, 1, 1, 1, 1);
  //  vtkLut->SetTableValue(2, 1, 0, 0, 1);
  //  vtkLut->SetTableValue(3, 0, 1, 0, 1);
  //  vtkLut->Build();

  //  auto lut = mitk::LookupTable::New();
  //  lut->SetVtkLookupTable(vtkLut);

  //  auto lutProperty = mitk::LookupTableProperty::New(lut);

  //  reamingPelvisNode->SetData(this->reamingFilter->GetOutput());
  //  reamingPelvisNode->SetProperty("LookupTable", lutProperty);
  //  reamingPelvisNode->SetProperty("scalar mode", mitk::VtkScalarModeProperty::New("Default"));
  //  reamingPelvisNode->SetBoolProperty("color mode", true);
  //  reamingPelvisNode->SetBoolProperty("scalar visibility", true);
  //  reamingPelvisNode->SetFloatProperty("ScalarsRangeMinimum", vtkLut->GetTableRange()[0]);
  //  reamingPelvisNode->SetFloatProperty("ScalarsRangeMaximum", vtkLut->GetTableRange()[1]);
  //  reamingPelvisNode->SetVisibility(true);
  //}
  //else if (this->currentWidget() == mUi->CupImpaction)
  //{
  //  auto *reamingPelvisNode = ds->GetNamedNode("reaming_pelvis");
  //  if (!reamingPelvisNode)
  //  {
  //    return;
  //  }
  //  reamingPelvisNode->SetVisibility(true);
  //  auto acetabularShell = ds->GetNamedObject<mitk::Surface>("acetabular_shell");
  //  auto acetabularShellImpacting = acetabularShell->Clone();

  //  auto acetabularShellImpactingNode = mitk::DataNode::New();
  //  acetabularShellImpactingNode->SetData(acetabularShellImpacting);
  //  acetabularShellImpactingNode->SetName("acetabular_shell_impacting");
  //  acetabularShellImpactingNode->SetVisibility(true);
  //  acetabularShellImpactingNode->SetColor(0, 0, 1);
  //  acetabularShellImpactingNode->SetOpacity(0.5);

  //  ds->Add(acetabularShellImpactingNode);
  //}

  //mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(
  //  mitk::RenderingManager::GetInstance()->GetDataStorage());
}