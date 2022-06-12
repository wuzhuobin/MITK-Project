#include "GroupBoxGadget.h"

#include "ui_GroupBoxGadget.h"
// qt

// mitk
//#include <mitkDataStorage.h>
#include <mitkBaseData.h>
#include <mitkPoint.h>
#include <mitkPointSet.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>
#include <mitkSurface.h>

// vtk
#include <vtkTransform.h>

static const struct GroupBoxGadgetInit
{
  GroupBoxGadgetInit() { Q_INIT_RESOURCE(GroupBoxGadget); }
} GGroupBoxGadgetInit;

GroupBoxGadget::GroupBoxGadget(Orientation orientation, QWidget* parent) :
    QGroupBox(parent),
    mUi(new Ui::GroupBoxGadget),
    mCurrentOrientation(orientation)
{
  mUi->setupUi(this);
}

GroupBoxGadget::~GroupBoxGadget()
{
  delete mUi;
}

void GroupBoxGadget::internalTransform(double position[3],
                                       double orientation[3])
{
  // data
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* data = ds->GetNamedObject<mitk::BaseData>(mTarget.toStdString());

  if (data == nullptr)
  {
    return;
  }
  auto* geo = data->GetGeometry();

  mitk::Point3D center = geo->GetCenter();
  mitk::PointSet* cor =
      ds->GetNamedObject<mitk::PointSet>(mTargetCor.toStdString());
  if (cor != nullptr)
  {
    center = cor->GetPoint(0);
    mitk::Point3D newCenter;
    newCenter[0] = center[0] + position[0];
    newCenter[1] = center[1] + position[1];
    newCenter[2] = center[2] + position[2];
    cor->SetPoint(0, newCenter);
  }

  vtkMatrix4x4* matrix = geo->GetVtkMatrix();
  vtkSmartPointer<vtkTransform> transform =
      vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply();
  transform->SetMatrix(matrix);
  transform->Translate(-center[0], -center[1], -center[2]);
  transform->RotateY(orientation[1]);
  transform->RotateX(orientation[0]);
  transform->RotateZ(orientation[2]);
  transform->Translate(center[0], center[1], center[2]);
  transform->Translate(position[0], position[1], position[2]);
  geo->SetIndexToWorldTransformByVtkMatrixWithoutChangingSpacing(
      transform->GetMatrix());
  // MITK_INFO << *transform;
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void GroupBoxGadget::on_pushButtonAntiClockwise_clicked(bool checked)
{
  Q_UNUSED(checked);
  double orientation[3] = {0, 0, 0};
  double position[3] = {0, 0, 0};
  switch (mCurrentOrientation)
  {
    case Orientation::AXIAL: {
      orientation[2] = 1.0 * mRotationSpeed;
    }
    break;
    case Orientation::SAGITTAL: {
      orientation[0] = -1.0 * mRotationSpeed;
    }
    break;
    case Orientation::CORONAL: {
      orientation[1] = 1.0 * mRotationSpeed;
    }
    break;
  }
  internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonClockwise_clicked(bool checked)
{
  Q_UNUSED(checked);
  double orientation[3] = {0, 0, 0};
  double position[3] = {0, 0, 0};
  switch (mCurrentOrientation)
  {
    case Orientation::AXIAL: {
      orientation[2] = -1.0 * mRotationSpeed;
    }
    break;
    case Orientation::SAGITTAL: {
      orientation[0] = 1.0 * mRotationSpeed;
    }
    break;
    case Orientation::CORONAL: {
      orientation[1] = -1.0 * mRotationSpeed;
    }
    break;
  }
  internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonDown_clicked(bool checked)
{
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (mCurrentOrientation)
  {
    case Orientation::AXIAL: {
      position[1] = 1.0 * mTranslationSpeed;
    }
    break;
    case Orientation::SAGITTAL:
    case Orientation::CORONAL: {
      position[2] = -1.0 * mTranslationSpeed;
    }
    break;
  }
  internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonLeft_clicked(bool checked)
{
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (mCurrentOrientation)
  {
    case Orientation::SAGITTAL: {
      position[1] = -1.0 * mTranslationSpeed;
    }
    break;
    case Orientation::AXIAL:
    case Orientation::CORONAL: {
      position[0] = -1.0 * mTranslationSpeed;
    }
    break;
  }
  internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonRight_clicked(bool checked)
{
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (mCurrentOrientation)
  {
    case Orientation::SAGITTAL: {
      position[1] = 1.0 * mTranslationSpeed;
    }
    break;
    case Orientation::AXIAL:
    case Orientation::CORONAL: {
      position[0] = 1.0 * mTranslationSpeed;
    }
    break;
  }
  internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonUp_clicked(bool checked)
{
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (mCurrentOrientation)
  {
    case Orientation::AXIAL: {
      position[1] = -1.0 * mTranslationSpeed;
    }
    break;
    case Orientation::SAGITTAL:
    case Orientation::CORONAL: {
      position[2] = 1.0 * mTranslationSpeed;
    }
    break;
  }
  internalTransform(position, orientation);
}
