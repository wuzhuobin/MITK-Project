#include "GroupBoxGadget.h"
#include "ui_GroupBoxGadget.h"
// qt

// mitk
//#include <mitkDataStorage.h>
#include <mitkSurface.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>
#include <mitkPointSet.h>

// vtk
#include <vtkTransform.h>

GroupBoxGadget::GroupBoxGadget(int orientation,
 const QString &femoralHeadCor,
 const QString &femoralStem,
 const QString &femoralHead,
 const QString &cupCor,
 const QString &acetabularShell,
 const QString &acetabularLiner,
 QWidget *parent):
  QGroupBox(parent),
  mUi(new Ui::GroupBoxGadget),
  currentMode(DEFAULT),
  currentOrientation(orientation),
  translationSpeed(2),
  rotationSpeed(5),
	femoralHeadCor(femoralHeadCor),
  femoralStem(femoralStem),
  femoralHead(femoralHead),
	cupCor(cupCor),
  acetabularShell(acetabularShell),
  acetabularLiner(acetabularLiner)
{
  this->mUi->setupUi(this);
}

GroupBoxGadget::~GroupBoxGadget()
{
  delete this->mUi;
}

void GroupBoxGadget::internalTransform(double position[3], double orientation[3])
{
	//data
	mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
	mitk::Surface *surface[2] = {nullptr};
	mitk::DataNode *surfaceNode[2] = {nullptr};
  mitk::PointSet *cor = nullptr;

	switch (this->currentMode)
	{
	case CUP: {
    cor = ds->GetNamedObject<mitk::PointSet>(this->cupCor.toStdString());
		surface[0] = ds->GetNamedObject<mitk::Surface>(this->acetabularShell.toStdString());
		surfaceNode[0] = ds->GetNamedNode(this->acetabularShell.toStdString());
		surface[1] = ds->GetNamedObject<mitk::Surface>(this->acetabularLiner.toStdString());
		surfaceNode[1] = ds->GetNamedNode(this->acetabularLiner.toStdString());
  }
  break;
  case STEM:
  {
    cor = ds->GetNamedObject<mitk::PointSet>(this->femoralHeadCor.toStdString());
		surface[0] = ds->GetNamedObject<mitk::Surface>(this->femoralStem.toStdString());
		surfaceNode[0] = ds->GetNamedNode(this->femoralStem.toStdString());
		surface[1] = ds->GetNamedObject<mitk::Surface>(this->femoralHead.toStdString());
		surfaceNode[1] = ds->GetNamedNode(this->femoralHead.toStdString());
  }
  break;
  default: // DEFAULT
  break;
  }
  mitk::Point3D center = cor->GetPoint(0);
  mitk::Point3D newCenter;
  newCenter[0] = center[0] + position[0];
  newCenter[1] = center[1] + position[1];
  newCenter[2] = center[2] + position[2];
  cor->SetPoint(0, newCenter);
  // mitk::AffineTransform3D::InputPointType center = cor->GetPOint();
  for (int i = 0; i < 2; ++i)
  {
    mitk::BaseGeometry *geo = surface[i]->GetGeometry();
    vtkMatrix4x4 *matrix = geo->GetVtkMatrix();
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply();
    transform->SetMatrix(matrix);
    transform->Translate(-center[0], -center[1], -center[2]);
    transform->RotateY(orientation[1]);
    transform->RotateX(orientation[0]);
    transform->RotateZ(orientation[2]);
    transform->Translate(center[0], center[1], center[2]);
    transform->Translate(position[0], position[1], position[2]);
    geo->SetIndexToWorldTransformByVtkMatrixWithoutChangingSpacing(transform->GetMatrix());
    // MITK_INFO << *transform;
  }
  // surface[1]->SetGeometry(geo);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void GroupBoxGadget::on_pushButtonAntiClockwise_clicked(bool checked)
{
  Q_UNUSED(checked);
  double orientation[3] = {0, 0, 0};
  double position[3] = {0, 0, 0};
	switch (this->currentOrientation)
	{
	case AXIAL:
	{
		orientation[2] = 1.0 * this->rotationSpeed;
	}
	break;
	case SAGITTAL:
	{
		orientation[0] = -1.0 * this->rotationSpeed;
	}
	break;
	case CORONAL:
	{
		orientation[1] = 1.0 * this->rotationSpeed;
	}
	break;
	}
  this->internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonClockwise_clicked(bool checked)
{
  Q_UNUSED(checked);
  double orientation[3] = {0, 0, 0};
  double position[3] = {0, 0, 0};
	switch (this->currentOrientation)
	{
	case AXIAL:
	{
		orientation[2] = -1.0 * this->rotationSpeed;
	}
	break;
	case SAGITTAL:
	{
		orientation[0] = 1.0 * this->rotationSpeed;
	}
	break;
	case CORONAL:
	{
		orientation[1] = -1.0 * this->rotationSpeed;
	}
	break;
	}
  this->internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonDown_clicked(bool checked)
{
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (this->currentOrientation)
  {
  case AXIAL:
  {
    position[1] = 1.0 * this->translationSpeed;
  }
  break;
  case SAGITTAL:
  case CORONAL:
  {
    position[2] = -1.0 * this->translationSpeed;
  }
  break;
  }
  this->internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonLeft_clicked(bool checked)
{
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (this->currentOrientation)
  {
  case SAGITTAL:
  {
    position[1] = -1.0 * this->translationSpeed;
  }
  break;
  case AXIAL:
  case CORONAL:
  {
    position[0] = -1.0 * this->translationSpeed;
  }
  break;
  }
  this->internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonRight_clicked(bool checked)
{
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (this->currentOrientation)
  {
  case SAGITTAL:
  {
    position[1] = 1.0 * this->translationSpeed;
  }
  break;
  case AXIAL:
  case CORONAL:
  {
    position[0] = 1.0 * this->translationSpeed;
  }
  break;
  }
  this->internalTransform(position, orientation);
}

void GroupBoxGadget::on_pushButtonUp_clicked(bool checked)
{
  Q_UNUSED(checked);
  double position[3] = {0, 0, 0};
  double orientation[3] = {0, 0, 0};
  switch (this->currentOrientation)
  {
  case AXIAL:
  {
    position[1] = -1.0 * this->translationSpeed;
  }
  break;
  case SAGITTAL:
  case CORONAL:
  {
    position[2] = 1.0 * this->translationSpeed;
  }
  break;
  }
  this->internalTransform(position, orientation);
}


