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

GroupBoxGadget::GroupBoxGadget(Orientation orientation, QWidget *parent)
    : QGroupBox(parent), mUi(new Ui::GroupBoxGadget),
      currentOrientation(orientation) {
  this->mUi->setupUi(this);
}

GroupBoxGadget::~GroupBoxGadget() { delete this->mUi; }

void GroupBoxGadget::internalTransform(double position[3],
                                       double orientation[3]) {
  // data
  mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();
  mitk::DataNode *dataNode = nullptr;
  mitk::BaseData *data = nullptr;

  if (!(dataNode = ds->GetNamedNode(this->target.toStdString())) ||
      !(data = dataNode->GetData())) {
    return;
  }

  mitk::PointSet *cor = nullptr;
  if (!(cor = ds->GetNamedObject<mitk::PointSet>(
            this->targetCor.toStdString()))) {
    return;
  }

  mitk::Point3D center = cor->GetPoint(0);
  mitk::Point3D newCenter;
  newCenter[0] = center[0] + position[0];
  newCenter[1] = center[1] + position[1];
  newCenter[2] = center[2] + position[2];
  cor->SetPoint(0, newCenter);
  mitk::BaseGeometry *geo = data->GetGeometry();
  vtkMatrix4x4 *matrix = geo->GetVtkMatrix();
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

void GroupBoxGadget::on_pushButtonAntiClockwise_clicked(bool checked) {
  Q_UNUSED(checked);
  double orientation[3] = {0, 0, 0};
  double position[3] = {0, 0, 0};
  switch (this->currentOrientation) {
  case Orientation::AXIAL: {
    orientation[2] = 1.0 * this->rotationSpeed;
  } break;
  case Orientation::SAGITTAL: {
    orientation[0] = -1.0 * this->rotationSpeed;
  } break;
  case Orientation::CORONAL: {
    orientation[1] = 1.0 * this->rotationSpeed;
  } break;
  }
  this->internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonClockwise_clicked(bool checked) {
  Q_UNUSED(checked);
  double orientation[3] = {0, 0, 0};
  double position[3] = {0, 0, 0};
  switch (this->currentOrientation) {
  case Orientation::AXIAL: {
    orientation[2] = -1.0 * this->rotationSpeed;
  } break;
  case Orientation::SAGITTAL: {
    orientation[0] = 1.0 * this->rotationSpeed;
  } break;
  case Orientation::CORONAL: {
    orientation[1] = -1.0 * this->rotationSpeed;
  } break;
  }
  this->internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonDown_clicked(bool checked) {
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (this->currentOrientation) {
  case Orientation::AXIAL: {
    position[1] = 1.0 * this->translationSpeed;
  } break;
  case Orientation::SAGITTAL:
  case Orientation::CORONAL: {
    position[2] = -1.0 * this->translationSpeed;
  } break;
  }
  this->internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonLeft_clicked(bool checked) {
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (this->currentOrientation) {
  case Orientation::SAGITTAL: {
    position[1] = -1.0 * this->translationSpeed;
  } break;
  case Orientation::AXIAL:
  case Orientation::CORONAL: {
    position[0] = -1.0 * this->translationSpeed;
  } break;
  }
  this->internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonRight_clicked(bool checked) {
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (this->currentOrientation) {
  case Orientation::SAGITTAL: {
    position[1] = 1.0 * this->translationSpeed;
  } break;
  case Orientation::AXIAL:
  case Orientation::CORONAL: {
    position[0] = 1.0 * this->translationSpeed;
  } break;
  }
  this->internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonUp_clicked(bool checked) {
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (this->currentOrientation) {
  case Orientation::AXIAL: {
    position[1] = -1.0 * this->translationSpeed;
  } break;
  case Orientation::SAGITTAL:
  case Orientation::CORONAL: {
    position[2] = 1.0 * this->translationSpeed;
  } break;
  }
  this->internalTransform(position, orientation);
}
