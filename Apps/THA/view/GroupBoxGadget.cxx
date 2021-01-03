#include "GroupBoxGadget.h"
#include "ui_GroupBoxGadget.h"
// qt

// mitk
//#include <mitkDataStorage.h>
#include <mitkSurface.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>

// vtk
#include <vtkTransform.h>

GroupBoxGadget::GroupBoxGadget(int orientation,
 const QString &femoralStem,
 const QString &femoralHead,
 const QString &acetabularShell,
 const QString &acetabularLiner,
 QWidget *parent):
  QGroupBox(parent),
  ui(new Ui::GroupBoxGadget),
  currentMode(DEFAULT),
  currentOrientation(orientation),
  translationSpeed(2),
  rotationSpeed(5),
  femoralStem(femoralStem),
  femoralHead(femoralHead),
  acetabularShell(acetabularShell),
  acetabularLiner(acetabularLiner)
{
  this->ui->setupUi(this);
}

GroupBoxGadget::~GroupBoxGadget()
{
  delete this->ui;
}

void GroupBoxGadget::internalTransform(double position[3], double orientation[3])
{
	//data
	mitk::Surface *surface[2] = {nullptr};
	mitk::DataNode *surfaceNode[2] = {nullptr};
	mitk::Surface *surface_stem[2] = {nullptr};
	mitk::DataNode *surfaceNode_stem[2] = {nullptr};

	mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

	switch (this->currentMode)
	{
	case CUP: {
		surface[0] = ds->GetNamedObject<mitk::Surface>(this->acetabularShell.toStdString());
		surfaceNode[0] = ds->GetNamedNode(this->acetabularShell.toStdString());
		surface[1] = ds->GetNamedObject<mitk::Surface>(this->acetabularLiner.toStdString());
		surfaceNode[1] = ds->GetNamedNode(this->acetabularLiner.toStdString());
  }
  break;
  case STEM:
  {
		surface[0] = ds->GetNamedObject<mitk::Surface>(this->femoralStem.toStdString());
		surfaceNode[0] = ds->GetNamedNode(this->femoralStem.toStdString());
		surface[1] = ds->GetNamedObject<mitk::Surface>(this->femoralHead.toStdString());
		surfaceNode[1] = ds->GetNamedNode(this->femoralHead.toStdString());
  }
  break;
  default: // DEFAULT
  break;
  }

  mitk::AffineTransform3D::InputPointType center;
  for (int i = 0; i < 2; ++i)
  {
    float origin[3];
    surfaceNode[i]->GetFloatProperty("origin.x", origin[0]);
    surfaceNode[i]->GetFloatProperty("origin.y", origin[1]);
    surfaceNode[i]->GetFloatProperty("origin.z", origin[2]);
    center[0] = origin[0];
    center[1] = origin[1];
    center[2] = origin[2];
    mitk::BaseGeometry *geo = surface[i]->GetGeometry();
    vtkMatrix4x4 *matrix = geo->GetVtkMatrix();
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply();
    transform->SetMatrix(matrix);
    transform->Translate(-origin[0], -origin[1], -origin[2]);
    transform->RotateY(orientation[1]);
    transform->RotateX(orientation[0]);
    transform->RotateZ(orientation[2]);
    transform->Translate(origin[0], origin[1], origin[2]);
    transform->Translate(position[0], position[1], position[2]);
    geo->SetIndexToWorldTransformByVtkMatrixWithoutChangingSpacing(transform->GetMatrix());
    // MITK_INFO << *transform;
    surfaceNode[i]->SetFloatProperty("origin.x", origin[0] + position[0]);
    surfaceNode[i]->SetFloatProperty("origin.y", origin[1] + position[1]);
    surfaceNode[i]->SetFloatProperty("origin.z", origin[2] + position[2]);
  }
  // surface[1]->SetGeometry(geo);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

// void GroupBoxGadget::CupTransform(mitk::Surface *surface[5], mitk::DataNode *surfaceNode[5], double position[3], double orientation[3])
// {
// 	for (int i = 0; i < 5; ++i)
// 		{
// 			if (surface[i] == nullptr || surfaceNode[i] == nullptr)
// 			{
// 				continue;
// 			}
// 			mitk::Vector3D position_cup;
// 			surfaceNode[i]->GetPropertyValue("position", position_cup);
// 			mitk::Vector3D orientation_cup;
// 			surfaceNode[i]->GetPropertyValue("orientation", orientation_cup);
// 			// Ref vtk's vtkProp3D::ComputeMatrix
// 			// shift back to actor's origin
//       // this->Transform->PostMultiply();
// 			// this->Transform->Translate(-this->Origin[0],
// 			//                           -this->Origin[1],
// 			//                           -this->Origin[2]);

// 			// // scale
// 			// this->Transform->Scale(this->Scale[0],
// 			//                       this->Scale[1],
// 			//                       this->Scale[2]);

// 			// // rotate
// 			// this->Transform->RotateY(this->Orientation[1]);
// 			// this->Transform->RotateX(this->Orientation[0]);
// 			// this->Transform->RotateZ(this->Orientation[2]);

// 			// // move back from origin and translate
// 			// this->Transform->Translate(this->Origin[0] + this->Position[0],
// 			//                           this->Origin[1] + this->Position[1],
// 			//                           this->Origin[2] + this->Position[2]);

// 			vtkSmartPointer<vtkTransform> transform =
// 				vtkSmartPointer<vtkTransform>::New();
//       transform->PostMultiply();
// 			if (i < 2)
// 			{
// 				transform->Translate(-position_cup[0], -position_cup[1], -position_cup[2]);
// 				transform->RotateY(orientation[1]);
// 				transform->RotateX(orientation[0]);
// 				transform->RotateZ(orientation[2]);
// 				transform->Translate(position_cup[0], position_cup[1], position_cup[2]);
// 				transform->Translate(position);
// 			}
// 			else
// 			{
// 				transform->Translate(position);
// 			}

// 			vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyDataFilter =
// 				vtkSmartPointer<vtkTransformPolyDataFilter>::New();
// 			transformPolyDataFilter->SetInputData(surface[i]->GetVtkPolyData());
// 			transformPolyDataFilter->SetTransform(transform);
// 			transformPolyDataFilter->Update();
// 			surface[i]->SetVtkPolyData(transformPolyDataFilter->GetOutput());
// 			for (int j = 0; j < 3; ++j)
// 			{
// 				position_cup[j] = position_cup[j] + position[j];
// 				orientation_cup[j] = orientation_cup[j] + orientation[j];
// 				if (orientation_cup[j] > 360.0)
// 				{
// 					orientation_cup[j] -= 360;
// 				}
// 				else if (orientation_cup[j] < 0) {
// 					orientation_cup[j] += 360;
// 				}
// 			}
// 			surfaceNode[i]->SetProperty("position", mitk::Vector3DProperty::New(position_cup));
// 			surfaceNode[i]->GetProperty("position")->Modified();
// 			surfaceNode[i]->SetProperty("orientation", mitk::Vector3DProperty::New(orientation_cup));
// 			surfaceNode[i]->GetProperty("orientation")->Modified();
// 			MITK_INFO << "new position: " <<
// 				position_cup[0] << ' ' <<
// 				position_cup[1] << ' ' <<
// 				position_cup[2] << ' ';
// 			MITK_INFO << "new orientation: " <<
// 				orientation_cup[0] << ' ' <<
// 				orientation_cup[1] << ' ' <<
// 				orientation_cup[2] << ' ';
// 			mitk::RenderingManager::GetInstance()->RequestUpdateAll();
// 		}
// }

// void GroupBoxGadget::StemTransform(mitk::Surface *surface_stem[3], mitk::DataNode *surfaceNode_stem[3], double position[3], double orientation[3])
// {
// 	for (int i = 0; i < 3; ++i)
// 	{
// 		if (surface_stem[i] == nullptr || surfaceNode_stem[i] == nullptr)
// 		{
// 			continue;
// 		}
// 		mitk::Vector3D position_stem;
// 		surfaceNode_stem[i]->GetPropertyValue("position", position_stem);
// 		mitk::Vector3D orientation_stem;
// 		surfaceNode_stem[i]->GetPropertyValue("orientation", orientation_stem);

// 		vtkSmartPointer<vtkTransform> transform =
// 			vtkSmartPointer<vtkTransform>::New();
//     transform->PostMultiply();
// 		transform->Translate(-position_stem[0], -position_stem[1], -position_stem[2]);
// 		transform->RotateY(orientation[1]);
// 		transform->RotateX(orientation[0]);
// 		transform->RotateZ(orientation[2]);
// 		transform->Translate(position_stem[0], position_stem[1], position_stem[2]);
// 		vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyDataFilter =
// 			vtkSmartPointer<vtkTransformPolyDataFilter>::New();
// 		transformPolyDataFilter->SetInputData(surface_stem[i]->GetVtkPolyData());
// 		transformPolyDataFilter->SetTransform(transform);
// 		transformPolyDataFilter->Update();
// 		surface_stem[i]->SetVtkPolyData(transformPolyDataFilter->GetOutput());
// 		for (int j = 0; j < 3; ++j)
// 		{
// 			orientation_stem[j] = orientation_stem[j] + orientation[j];
// 			if (orientation_stem[j] > 360.0)
// 			{
// 				orientation_stem[j] -= 360;
// 			}
// 			else if (orientation_stem[j] < 0) 
// 			{
// 				orientation_stem[j] += 360;
// 			}
// 		}
// 		surfaceNode_stem[i]->SetProperty("orientation", mitk::Vector3DProperty::New(orientation_stem));
// 		surfaceNode_stem[i]->GetProperty("orientation")->Modified();
// 		MITK_INFO << "new orientation: " <<
// 			orientation_stem[0] << ' ' <<
// 			orientation_stem[1] << ' ' <<
// 			orientation_stem[2] << ' ';
// 		mitk::RenderingManager::GetInstance()->RequestUpdateAll();
// 	}
// }

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


