#include "AcetabularPrepWidget.h"

#include "ReamingFilter2.h"
#include "ui_AcetabularPrepWidget.h"

// mitk
#include <mitkDataStorage.h>
#include <mitkLogMacros.h>
#include <mitkLookupTable.h>
#include <mitkLookupTableProperty.h>
#include <mitkPointSet.h>
#include <mitkRenderingManager.h>
#include <mitkSurface.h>
#include <mitkVtkScalarModeProperty.h>

// vtk
#include <vtkLookupTable.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

AcetabularPrepWidget::AcetabularPrepWidget(QWidget* parent) :
    QStackedWidget(parent),
    mUi(new Ui::AcetabularPrepWidget),
    mReamingFilter(vtkSmartPointer<ReamingFilter2>::New())
{
  mUi->setupUi(this);
}

AcetabularPrepWidget::~AcetabularPrepWidget()
{
  delete mUi;
}

void AcetabularPrepWidget::action_Pelvis_Checkpoint_triggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->PelvisCheckpoint);
}

void AcetabularPrepWidget::action_Pelvis_Landmark_triggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->PelvisLandmark);
}

void AcetabularPrepWidget::action_Pelvis_Registration_triggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->PelvisRegistration);
}

void AcetabularPrepWidget::action_RIO_Registratoin_triggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->RIORegistration);
}

void AcetabularPrepWidget::action_Cup_Reaming_triggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->CupReaming);
}

void AcetabularPrepWidget::action_Cup_Impaction_triggered(bool checked)
{
  Q_UNUSED(checked);
  setCurrentWidget(mUi->CupImpaction);
}

void AcetabularPrepWidget::setPelvisLandmarkIndex(int index)
{
  if (index < 0)
  {
    pelvisLandmarkIndex = 0;
  }
  else if (index > 3)
  {
    pelvisLandmarkIndex = 3;
  }
  else
  {
    pelvisLandmarkIndex = index;
  }

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* posteriorAcetabulumNode = ds->GetNamedNode("posterior_acetabulum");
  auto* anteriorAcetabulumNode = ds->GetNamedNode("anterior_acetabulum");
  auto* superiorAcetabulumNode = ds->GetNamedNode("superior_acetabulum");

  switch (pelvisLandmarkIndex)
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

vtkSmartPointer<vtkPolyData> AcetabularPrepWidget::GetTransformedVtkPolyData(
    mitk::Surface* surface)
{
  auto* vtkMatrix = surface->GetGeometry()->GetVtkMatrix();
  auto spacing = surface->GetGeometry()->GetSpacing();
  auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->SetMatrix(vtkMatrix);
  transform->Scale(1 / spacing[0], 1 / spacing[1], 1 / spacing[2]);
  auto transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformFilter->SetInputData(surface->GetVtkPolyData());
  transformFilter->SetTransform(transform);
  transformFilter->Update();
  return transformFilter->GetOutput();
}

void AcetabularPrepWidget::setPelvisRegistrationIndex(int index)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* registrationPoints =
      ds->GetNamedObject<mitk::PointSet>("registration_points");

  if (index < 0)
  {
    pelvisRegistrationIndex = 0;
  }
  else if (index > registrationPoints->GetSize())
  {
    pelvisRegistrationIndex = registrationPoints->GetSize();
  }
  else
  {
    pelvisRegistrationIndex = index;
  }

  for (int i = 0; i < registrationPoints->GetSize(); ++i)
  {
    registrationPoints->SetSelectInfo(i, false);
  }
  if (pelvisRegistrationIndex != registrationPoints->GetSize())
  {
    registrationPoints->SetSelectInfo(pelvisRegistrationIndex, true);
    mUi->pushButtonCaptureRegistration->setEnabled(true);
    mUi->pushButtonVerifyRegistration->setEnabled(false);
    if (pelvisRegistrationIndex == 0)
    {
      mUi->pushButtonClearLastPoint->setEnabled(false);
    }
    else
    {
      mUi->pushButtonClearLastPoint->setEnabled(true);
    }
  }
  else
  {
    mUi->pushButtonCaptureRegistration->setEnabled(false);
    mUi->pushButtonVerifyRegistration->setEnabled(true);
  }
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void AcetabularPrepWidget::transformReamer(Orientation orientation,
                                           double t,
                                           double r)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* reamer = ds->GetNamedObject<mitk::Surface>("reamer");
  auto* matrix = reamer->GetGeometry()->GetVtkMatrix();
  auto center = reamer->GetGeometry()->GetCenter();
  auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply();
  transform->SetMatrix(matrix);
  transform->Translate(-center[0], -center[1], -center[2]);
  transform->RotateY(orientation == Orientation::Y ? r : 0);
  transform->RotateX(orientation == Orientation::X ? r : 0);
  transform->RotateZ(orientation == Orientation::Z ? r : 0);
  transform->Translate(orientation == Orientation::X ? t : 0,
                       orientation == Orientation::Y ? t : 0,
                       orientation == Orientation::Z ? t : 0);
  transform->Translate(center[0], center[1], center[2]);
  transform->Update();

  auto transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  transformPolyData->SetInputData(reamer->GetVtkPolyData());
  transformPolyData->SetTransform(transform);
  transformPolyData->Update();
  reamer->GetVtkPolyData()->ShallowCopy(transformPolyData->GetOutput());

  mReamingFilter->Update();

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void AcetabularPrepWidget::on_pushButtonCaptureLandmark_clicked(bool checked)
{
  Q_UNUSED(checked);
  setPelvisLandmarkIndex(++pelvisLandmarkIndex);
}

void AcetabularPrepWidget::on_pushButtonClearLandmark_clicked(bool checked)
{
  Q_UNUSED(checked);
  setPelvisLandmarkIndex(--pelvisLandmarkIndex);
}

void AcetabularPrepWidget::on_pushButtonClearAllPelvis_clicked(bool checked)
{
  Q_UNUSED(checked);
  setPelvisLandmarkIndex(0);
}

void AcetabularPrepWidget::on_pushButtonCaptureRegistration_clicked(
    bool checked)
{
  Q_UNUSED(checked);
  setPelvisRegistrationIndex(++pelvisRegistrationIndex);
}

void AcetabularPrepWidget::on_pushButtonClearLastPoint_clicked(bool checked)
{
  Q_UNUSED(checked);
  setPelvisRegistrationIndex(--pelvisRegistrationIndex);
}

void AcetabularPrepWidget::on_pushButtonClearAllPoints_clicked(bool checked)
{
  setPelvisRegistrationIndex(0);
}

void AcetabularPrepWidget::on_pushButtonVerifyRegistration_clicked(bool checked)
{
}

void AcetabularPrepWidget::on_pushButtonXRA_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::X, 0, 5);
}

void AcetabularPrepWidget::on_pushButtonXRM_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::X, 0, -5);
}

void AcetabularPrepWidget::on_pushButtonXTA_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::X, 5, 0);
}

void AcetabularPrepWidget::on_pushButtonXTM_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::X, -5, 0);
}

void AcetabularPrepWidget::on_pushButtonYRA_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::Y, 0, 5);
}

void AcetabularPrepWidget::on_pushButtonYRM_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::Y, 0, -5);
}

void AcetabularPrepWidget::on_pushButtonYTA_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::Y, 5, 0);
}

void AcetabularPrepWidget::on_pushButtonYTM_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::Y, -5, 0);
}

void AcetabularPrepWidget::on_pushButtonZRA_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::Z, 0, 5);
}

void AcetabularPrepWidget::on_pushButtonZRM_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::Z, 0, -5);
}

void AcetabularPrepWidget::on_pushButtonZTA_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::Z, 5, 0);
}

void AcetabularPrepWidget::on_pushButtonZTM_clicked(bool checked)
{
  Q_UNUSED(checked);
  transformReamer(Orientation::Z, -5, 0);
}

void AcetabularPrepWidget::on_AcetabularPrepWidget_currentChanged(int index)
{
  Q_UNUSED(index);
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  if (currentWidget() == mUi->PelvisLandmark)
  {
    auto* pelvisNode = ds->GetNamedNode("pelvis");
    pelvisNode->SetVisibility(true);
    setPelvisLandmarkIndex(pelvisLandmarkIndex);
  }
  else if (currentWidget() == mUi->PelvisRegistration)
  {
    auto* pelvisRegistrationNode = ds->GetNamedNode("pelvis_registration");
    auto* registrationPoints = ds->GetNamedNode("registration_points");
    pelvisRegistrationNode->SetVisibility(true);
    registrationPoints->SetVisibility(true);
    setPelvisRegistrationIndex(pelvisRegistrationIndex);
  }
  else if (currentWidget() == mUi->CupReaming)
  {
    auto* acetabularShell =
        ds->GetNamedObject<mitk::Surface>("acetabular_shell");

    auto* reamerTrajectoryNode = ds->GetNamedNode("reamer_trajectory");
    // reamerTrajectoryNode->SetVisibility(true);
    // reamerTrajectoryNode->SetOpacity(0.2);
    // reamerTrajectoryNode->SetColor(0, 1, 0);
    auto* reamerTrajectory =
        static_cast<mitk::Surface*>(reamerTrajectoryNode->GetData());
    reamerTrajectory->GetGeometry()
        ->SetIndexToWorldTransformWithoutChangingSpacing(
            acetabularShell->GetGeometry()->GetIndexToWorldTransform());

    auto* reamerNode = ds->GetNamedNode("reamer");
    reamerNode->SetVisibility(true);
    reamerNode->SetColor(0, 0, 1);
    auto* reamer = static_cast<mitk::Surface*>(reamerNode->GetData());
    reamer->GetGeometry()->SetIndexToWorldTransformWithoutChangingSpacing(
        acetabularShell->GetGeometry()->GetIndexToWorldTransform());

    auto* overlay = ds->GetNamedObject<mitk::Image>("overlay");
    auto* overlayMatrix = overlay->GetGeometry()->GetVtkMatrix();
    auto overlaySpacing = overlay->GetGeometry()->GetSpacing();
    auto transform = vtkSmartPointer<vtkTransform>::New();
    transform->SetMatrix(overlayMatrix);
    transform->Scale(
        1 / overlaySpacing[0], 1 / overlaySpacing[1], 1 / overlaySpacing[2]);

    mReamingFilter->SetPelvis(overlay->GetVtkImageData());
    // Since it is transformed manually, no need to re-transformed
    // mReamingFilter->SetReamerTrajectory(
    //     GetTransformedVtkPolyData(reamerTrajectory));
    // mReamingFilter->SetReamer(GetTransformedVtkPolyData(reamer));
    mReamingFilter->SetReamerTrajectory(reamerTrajectory->GetVtkPolyData());
    mReamingFilter->SetReamer(reamer->GetVtkPolyData());
    mReamingFilter->SetSize(3);
    mReamingFilter->SetImageTransform(transform);
    mReamingFilter->SetExtent(100, 250, 200, 300, 1150, 1270);
    mReamingFilter->Update();

    mitk::DataNode::Pointer reamingPelvisNode =
        ds->GetNamedNode("reaming_pelvis");
    if (!reamingPelvisNode)
    {
      auto vtkLut = vtkSmartPointer<vtkLookupTable>::New();
      vtkLut->SetTableRange(0, 3);
      vtkLut->SetNumberOfTableValues(4);
      vtkLut->SetTableValue(0, 0, 0, 0, 0);
      vtkLut->SetTableValue(1, 0, 1, 0, 1);
      vtkLut->SetTableValue(2, 1, 1, 1, 1);
      vtkLut->SetTableValue(3, 1, 0, 0, 1);
      vtkLut->Build();

      auto lut = mitk::LookupTable::New();
      lut->SetVtkLookupTable(vtkLut);

      auto lutProperty = mitk::LookupTableProperty::New(lut);

      reamingPelvisNode = mitk::DataNode::New();
      reamingPelvisNode->SetName("reaming_pelvis");
      reamingPelvisNode->SetData(mitk::Surface::New());
      reamingPelvisNode->SetProperty("LookupTable", lutProperty);
      reamingPelvisNode->SetProperty(
          "scalar mode", mitk::VtkScalarModeProperty::New("Default"));
      reamingPelvisNode->SetBoolProperty("color mode", true);
      reamingPelvisNode->SetBoolProperty("scalar visibility", true);
      reamingPelvisNode->SetFloatProperty("ScalarsRangeMinimum",
                                          vtkLut->GetTableRange()[0]);
      reamingPelvisNode->SetFloatProperty("ScalarsRangeMaximum",
                                          vtkLut->GetTableRange()[1]);

      ds->Add(reamingPelvisNode);
    }
    auto* reamingPelvis =
        static_cast<mitk::Surface*>(reamingPelvisNode->GetData());
    reamingPelvis->SetVtkPolyData(mReamingFilter->GetOutput());
    reamingPelvisNode->SetVisibility(true);
  }
  else if (currentWidget() == mUi->CupImpaction)
  {
    auto* reamingPelvisNode = ds->GetNamedNode("reaming_pelvis");
    if (!reamingPelvisNode)
    {
      return;
    }
    reamingPelvisNode->SetVisibility(true);
    auto acetabularShell =
        ds->GetNamedObject<mitk::Surface>("acetabular_shell");
    auto acetabularShellImpacting = acetabularShell->Clone();

    auto acetabularShellImpactingNode = mitk::DataNode::New();
    acetabularShellImpactingNode->SetData(acetabularShellImpacting);
    acetabularShellImpactingNode->SetName("acetabular_shell_impacting");
    acetabularShellImpactingNode->SetVisibility(true);
    acetabularShellImpactingNode->SetColor(0, 0, 1);
    acetabularShellImpactingNode->SetOpacity(0.5);

    ds->Add(acetabularShellImpactingNode);
  }

  mitk::RenderingManager::GetInstance()->InitializeViewsByBoundingObjects(
      mitk::RenderingManager::GetInstance()->GetDataStorage());
}