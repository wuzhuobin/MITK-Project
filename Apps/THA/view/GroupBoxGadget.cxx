#include "GroupBoxGadget.h"
#include "ui_GroupBoxGadget.h"
// #include "THADataStorage.h"
// qt
#include <QDebug>
// vtk
#include <vtkTransformPolyDataFilter.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

// mitk
//#include <mitkDataStorage.h>
//#include <mitkSurface.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>

GroupBoxGadget::GroupBoxGadget(int orientation,
 const QString &femoralStem,
 const QString &femoralNeck,
 const QString &femoralHead,
 const QString &acetabularShell,
 const QString &acetabularInsert,
 QWidget *parent):
  QGroupBox(parent),
  ui(new Ui::GroupBoxGadget),
  currentMode(DEFAULT),
  currentOrientation(orientation),
  translationSpeed(2),
  rotationSpeed(5),
  femoralStem(femoralStem),
  femoralNeck(femoralNeck),
  femoralHead(femoralHead),
  acetabularShell(acetabularShell),
  acetabularInsert(acetabularInsert)
{
  this->ui->setupUi(this);
}

GroupBoxGadget::~GroupBoxGadget()
{
  delete this->ui;
}

void GroupBoxGadget::setButtonShow(bool b)
{
	this->ui->pushButtonDown->setVisible(b);
	this->ui->pushButtonLeft->setVisible(b);
	this->ui->pushButtonRight->setVisible(b);
	this->ui->pushButtonUp->setVisible(b);
}

void GroupBoxGadget::internalTransform(double position[3], double orientation[3])
{
	//data 
  mitk::Surface *surface_cup[5] = {nullptr};
  mitk::DataNode *surfaceNode_cup[5] = {nullptr};
	mitk::Surface *surface_stem[3] = { nullptr };
	mitk::DataNode *surfaceNode_stem[3] = { nullptr };

  mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  switch (this->currentMode)
  {
  case CUP:
	{
		//data 
		surface_cup[0] = ds->GetNamedObject<mitk::Surface>(this->acetabularShell.toStdString());
		surfaceNode_cup[0] = ds->GetNamedNode(this->acetabularShell.toStdString());
		surface_cup[1] = ds->GetNamedObject<mitk::Surface>(this->acetabularInsert.toStdString());
		surfaceNode_cup[1] = ds->GetNamedNode(this->acetabularInsert.toStdString());
		surface_cup[2] = ds->GetNamedObject<mitk::Surface>(this->femoralStem.toStdString());
		surfaceNode_cup[2] = ds->GetNamedNode(this->femoralStem.toStdString());
		surface_cup[3] = ds->GetNamedObject<mitk::Surface>(this->femoralNeck.toStdString());
		surfaceNode_cup[3] = ds->GetNamedNode(this->femoralNeck.toStdString());
		surface_cup[4] = ds->GetNamedObject<mitk::Surface>(this->femoralHead.toStdString());
		surfaceNode_cup[4] = ds->GetNamedNode(this->femoralHead.toStdString());
		this->CupTransform(surface_cup, surfaceNode_cup, position, orientation);

		//mitk::Vector3D position;
		//position[0] = 84.551;
		//position[1] = -141.718;
		//position[2] = 1406.19;
		//mitk::Vector3D orientation;
		//orientation[0] = 0;
		//orientation[1] = 0;
		//orientation[2] = 0;

		//for (int i = 0;i<5;i++)
		//{
		//	surfaceNode_cup[i]->SetProperty("position", mitk::Vector3DProperty::New(position));
		//	surfaceNode_cup[i]->SetProperty("orientation", mitk::Vector3DProperty::New(orientation));
		//	surfaceNode_cup_off[i]->SetProperty("position", mitk::Vector3DProperty::New(position));
		//	surfaceNode_cup_off[i]->SetProperty("orientation", mitk::Vector3DProperty::New(orientation));
		//}
		//std::string directory = "D:\\THAHipStorage\\3\\THAhip.mitk";
		//mitk::SceneIO::Pointer sceneIO = mitk::SceneIO::New();
		//mitk::DataStorage *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
		//sceneIO->SaveScene(ds->GetAll(), ds, directory);
  }
  break;
  case STEM:
  {
		//data
		surface_stem[0] = ds->GetNamedObject<mitk::Surface>(this->femoralStem.toStdString());
		surfaceNode_stem[0] = ds->GetNamedNode(this->femoralStem.toStdString());
		surface_stem[1] = ds->GetNamedObject<mitk::Surface>(this->femoralNeck.toStdString());
		surfaceNode_stem[1] = ds->GetNamedNode(this->femoralNeck.toStdString());
		surface_stem[2] = ds->GetNamedObject<mitk::Surface>(this->femoralHead.toStdString());
		surfaceNode_stem[2] = ds->GetNamedNode(this->femoralHead.toStdString());
		this->StemTransform(surface_stem, surfaceNode_stem, position, orientation);
  }
  break;
  default: // DEFAULT
  break;
  }
}

void GroupBoxGadget::CupTransform(mitk::Surface *surface_cup[5], mitk::DataNode *surfaceNode_cup[5], double position[3], double orientation[3])
{
	for (int i = 0; i < 5; ++i)
		{
			if (surface_cup[i] == nullptr || surfaceNode_cup[i] == nullptr)
			{
				continue;
			}
			mitk::Vector3D position_cup;
			surfaceNode_cup[i]->GetPropertyValue("position", position_cup);
			mitk::Vector3D orientation_cup;
			surfaceNode_cup[i]->GetPropertyValue("orientation", orientation_cup);
			// Ref vtk's vtkProp3D::ComputeMatrix
			// shift back to actor's origin
      // this->Transform->PostMultiply();
			// this->Transform->Translate(-this->Origin[0],
			//                           -this->Origin[1],
			//                           -this->Origin[2]);

			// // scale
			// this->Transform->Scale(this->Scale[0],
			//                       this->Scale[1],
			//                       this->Scale[2]);

			// // rotate
			// this->Transform->RotateY(this->Orientation[1]);
			// this->Transform->RotateX(this->Orientation[0]);
			// this->Transform->RotateZ(this->Orientation[2]);

			// // move back from origin and translate
			// this->Transform->Translate(this->Origin[0] + this->Position[0],
			//                           this->Origin[1] + this->Position[1],
			//                           this->Origin[2] + this->Position[2]);

			vtkSmartPointer<vtkTransform> transform =
				vtkSmartPointer<vtkTransform>::New();
      transform->PostMultiply();
			if (i < 2)
			{
				transform->Translate(-position_cup[0], -position_cup[1], -position_cup[2]);
				transform->RotateY(orientation[1]);
				transform->RotateX(orientation[0]);
				transform->RotateZ(orientation[2]);
				transform->Translate(position_cup[0], position_cup[1], position_cup[2]);
				transform->Translate(position);
			}
			else
			{
				transform->Translate(position);
			}

			vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyDataFilter =
				vtkSmartPointer<vtkTransformPolyDataFilter>::New();
			transformPolyDataFilter->SetInputData(surface_cup[i]->GetVtkPolyData());
			transformPolyDataFilter->SetTransform(transform);
			transformPolyDataFilter->Update();
			surface_cup[i]->SetVtkPolyData(transformPolyDataFilter->GetOutput());
			for (int j = 0; j < 3; ++j)
			{
				position_cup[j] = position_cup[j] + position[j];
				orientation_cup[j] = orientation_cup[j] + orientation[j];
				if (orientation_cup[j] > 360.0)
				{
					orientation_cup[j] -= 360;
				}
				else if (orientation_cup[j] < 0) {
					orientation_cup[j] += 360;
				}
			}
			surfaceNode_cup[i]->SetProperty("position", mitk::Vector3DProperty::New(position_cup));
			surfaceNode_cup[i]->GetProperty("position")->Modified();
			surfaceNode_cup[i]->SetProperty("orientation", mitk::Vector3DProperty::New(orientation_cup));
			surfaceNode_cup[i]->GetProperty("orientation")->Modified();
			MITK_INFO << "new position: " <<
				position_cup[0] << ' ' <<
				position_cup[1] << ' ' <<
				position_cup[2] << ' ';
			MITK_INFO << "new orientation: " <<
				orientation_cup[0] << ' ' <<
				orientation_cup[1] << ' ' <<
				orientation_cup[2] << ' ';
			mitk::RenderingManager::GetInstance()->RequestUpdateAll();
		}
}

void GroupBoxGadget::StemTransform(mitk::Surface *surface_stem[3], mitk::DataNode *surfaceNode_stem[3], double position[3], double orientation[3])
{
	for (int i = 0; i < 3; ++i)
	{
		if (surface_stem[i] == nullptr || surfaceNode_stem[i] == nullptr)
		{
			continue;
		}
		mitk::Vector3D position_stem;
		surfaceNode_stem[i]->GetPropertyValue("position", position_stem);
		mitk::Vector3D orientation_stem;
		surfaceNode_stem[i]->GetPropertyValue("orientation", orientation_stem);

		vtkSmartPointer<vtkTransform> transform =
			vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply();
		transform->Translate(-position_stem[0], -position_stem[1], -position_stem[2]);
		transform->RotateY(orientation[1]);
		transform->RotateX(orientation[0]);
		transform->RotateZ(orientation[2]);
		transform->Translate(position_stem[0], position_stem[1], position_stem[2]);
		vtkSmartPointer<vtkTransformPolyDataFilter> transformPolyDataFilter =
			vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		transformPolyDataFilter->SetInputData(surface_stem[i]->GetVtkPolyData());
		transformPolyDataFilter->SetTransform(transform);
		transformPolyDataFilter->Update();
		surface_stem[i]->SetVtkPolyData(transformPolyDataFilter->GetOutput());
		for (int j = 0; j < 3; ++j)
		{
			orientation_stem[j] = orientation_stem[j] + orientation[j];
			if (orientation_stem[j] > 360.0)
			{
				orientation_stem[j] -= 360;
			}
			else if (orientation_stem[j] < 0) 
			{
				orientation_stem[j] += 360;
			}
		}
		surfaceNode_stem[i]->SetProperty("orientation", mitk::Vector3DProperty::New(orientation_stem));
		surfaceNode_stem[i]->GetProperty("orientation")->Modified();
		MITK_INFO << "new orientation: " <<
			orientation_stem[0] << ' ' <<
			orientation_stem[1] << ' ' <<
			orientation_stem[2] << ' ';
		mitk::RenderingManager::GetInstance()->RequestUpdateAll();
	}
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


