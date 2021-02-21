#include "THAStdMultiWidget.h"
#include "GroupBoxGadget.h"
#include "StemParameterGadget.h"
#include "ImplantAssessmentGadget.h"
#include "CupParameterGadget.h"
// vtk
// #include <vtkRenderWindow.h>
// #include <vtkRenderer.h>
// #include <vtkTransformPolyDataFilter.h>
// #include <vtkSmartPointer.h>
// #include <vtkTransform.h>
// #include <vtkLineSource.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkActor.h>

// qt
#include <QGridLayout>

// mitk
// #include <mitkImage.h>
// #include <mitkLogMacros.h>
// #include <mitkSceneIO.h>
// #include <mitkSurface.h>
#include <mitkLogMacros.h>
#include <QmitkRenderWindow.h>

static const std::set<std::string> DEFAULT_VISIBLE_SET = {
  "hip_length_right",
  "hip_length_left",
  "trochanter",
  "native_cor",
  "PSIS",
  "ASIS",
  "femoral_neck_transformed",
  "femoral_head_transformed",
  "femoarl_stem_transformed",
  "acetabular_shell_transformed",
  "acetabular_insert_transformed",
  "pelvis",
  "femur_left",
  "femur_right",
  "image"};

THAStdMultiWidget::THAStdMultiWidget(
  QWidget *parent,
  Qt::WindowFlags f,
  const QString &name):
  QmitkStdMultiWidget(parent, f, name),
  groupBoxGadget{
    new GroupBoxGadget(GroupBoxGadget::AXIAL, this),
    new GroupBoxGadget(GroupBoxGadget::SAGITTAL, this),
    new GroupBoxGadget(GroupBoxGadget::CORONAL, this),
    new GroupBoxGadget(GroupBoxGadget::CORONAL, this)
  },
  stemParameterGadget{
    new StemParameterGadget(this),
    new StemParameterGadget(this),
    new StemParameterGadget(this),
    new StemParameterGadget(this)
  },
  implantAssessmentGadget{
    new ImplantAssessmentGadget(this),
    new ImplantAssessmentGadget(this),
    new ImplantAssessmentGadget(this),
    new ImplantAssessmentGadget(this)
  },
  cupParameterGadget{
    new CupParameterGadget(this),
    new CupParameterGadget(this),
    new CupParameterGadget(this),
    new CupParameterGadget(this)
  }
{
}

void THAStdMultiWidget::InitializeMultiWidget()
{
  this->SetDataStorage(mitk::RenderingManager::GetInstance()->GetDataStorage());
  QmitkStdMultiWidget::InitializeMultiWidget();
  // Add the displayed views to the DataStorage to see their positions in 2D and 3D
  this->AddDisplayPlaneSubTree();
  // show image plane in viewer and data storage
  this->AddPlanesToDataStorage();
  // Disable the plane widget
  this->SetCrosshairVisibility(true);
////////////////////////////////////////////////////////////////////////////////
/// Customization
////////////////////////////////////////////////////////////////////////////////
  
  for (int i = 1; i <= 3; ++i)
  {
    this->SetWidgetPlaneVisibility(
      this->GetWidgetPlane(i)->GetName().c_str(), 
      false, 
      mitk::BaseRenderer::GetInstance(this->GetRenderWindow4()->GetVtkRenderWindow())
    );
  }

  // this->GetRenderWindow4()->GetVtkRenderWindow()->SetMultiSamples(0);   // We do not support MSAA as it is incompatible with depth peeling
  // this->GetRenderWindow4()->GetVtkRenderWindow()->SetAlphaBitPlanes(true); // Necessary for depth peeling
  // this->GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetMaximumNumberOfPeels(50);
  // this->GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetUseDepthPeeling(true);
  // this->GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetUseDepthPeelingForVolumes(true);
  // // Initialize bottom-right view as 3D view
  // this->GetRenderWindow4()->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);

  // this->GetRenderWindow1()->GetVtkRenderWindow()->SetWindowName(
  //   (this->GetRenderWindow1()->objectName() + '1').toStdString().c_str()
  // );
  // this->GetRenderWindow2()->GetVtkRenderWindow()->SetWindowName(
  //   (this->GetRenderWindow2()->objectName() + '2').toStdString().c_str()
  // );
  // this->GetRenderWindow3()->GetVtkRenderWindow()->SetWindowName(
  //   (this->GetRenderWindow3()->objectName() + '3').toStdString().c_str()
  // );
  // this->GetRenderWindow4()->GetVtkRenderWindow()->SetWindowName(
  //   (this->GetRenderWindow4()->objectName() + '4').toStdString().c_str()
  // );

  for(int i = 0; i < 4; ++i)
  {
    QmitkRenderWindow *renderWindow = this->GetRenderWindow(i);
    QGridLayout *pRenderWindow1Layout = new QGridLayout(renderWindow);
    pRenderWindow1Layout->addWidget(this->groupBoxGadget[i], 0, 1, Qt::AlignRight | Qt::AlignTop);
    pRenderWindow1Layout->addWidget(this->stemParameterGadget[i], 1, 1, Qt::AlignRight | Qt::AlignBottom);
    pRenderWindow1Layout->addWidget(this->cupParameterGadget[i], 2, 1, Qt::AlignRight | Qt::AlignBottom);
    pRenderWindow1Layout->addWidget(this->implantAssessmentGadget[i], 3, 1, Qt::AlignRight | Qt::AlignBottom);
    renderWindow->setLayout(pRenderWindow1Layout);
  }
////////////////////////////////////////////////////////////////////////////////
/// Gadget Connection
////////////////////////////////////////////////////////////////////////////////
  
  for(int i = 0; i < 4; ++i)
  {
    this->cupParameterGadget[i]->ObserverCup();
    this->stemParameterGadget[i]->ObserverStem();
  }
////////////////////////////////////////////////////////////////////////////////
  // this->SetRegistrationMode(this->registrationMode);
  this->SetView(VIEWS::VIEW_DEFAULT);
  this->SetMode(MODES::MODE_DEFAULT);
  this->UpdateViewMode();
////////////////////////////////////////////////////////////////////////////////
/// Preset
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkSelectEnclosedPoints.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPointDataToCellData.h>
#include <vtkTransform.h>

void THAStdMultiWidget::SetView(int mode)
{
  this->view = mode;
  this->UpdateViewMode();
}

void THAStdMultiWidget::SetMode(int mode)
{
  this->mode = mode;
  this->UpdateViewMode();
}

void THAStdMultiWidget::SetOther(bool other)
{
  this->other = other;
  this->UpdateViewMode();
}

bool THAStdMultiWidget::GetOther() const
{
  return this->other;
}

void THAStdMultiWidget::UpdateViewMode()
{
////////////////////////////////////////////////////////////////////////////////
/// Set Visibility
////////////////////////////////////////////////////////////////////////////////
  for (int i = 0; i < 4; ++i)
  {
    this->groupBoxGadget[i]->setVisible(false);
    this->groupBoxGadget[i]->setMode(GroupBoxGadget::DEFAULT);
    this->stemParameterGadget[i]->setVisible(false);
    this->implantAssessmentGadget[i]->setVisible(false);
    this->cupParameterGadget[i]->setVisible(false);
  }
  mitk::DataStorage::SetOfObjects::ConstPointer all = this->GetDataStorage()->GetAll();
  for (mitk::DataNode *one : *all) {
    one->SetVisibility(false);
  }
  mitk::DataNode *cupCor = this->GetDataStorage()->GetNamedNode("cup_cor");
  mitk::DataNode *acetabularLiner = this->GetDataStorage()->GetNamedNode("acetabular_liner");
  mitk::DataNode *acetabularShell = this->GetDataStorage()->GetNamedNode("acetabular_shell");
  mitk::DataNode *femoralHeadCor = this->GetDataStorage()->GetNamedNode("femoral_head_cor");
  mitk::DataNode *femoralStem = this->GetDataStorage()->GetNamedNode("femoral_stem");
  mitk::DataNode *femoralHead = this->GetDataStorage()->GetNamedNode("femoral_head");
  mitk::DataNode *trochanterNode = this->GetDataStorage()->GetNamedNode("trochanter");
  mitk::DataNode *trochanterRightLineNode = this->GetDataStorage()->GetNamedNode("trochanter_right_line");
  mitk::DataNode *trochanterLeftLineNode = this->GetDataStorage()->GetNamedNode("trochanter_left_line");
  mitk::DataNode *nativeCorNode = this->GetDataStorage()->GetNamedNode("native_cor");
  mitk::DataNode *psisNode = this->GetDataStorage()->GetNamedNode("psis");
  mitk::DataNode *asisNode = this->GetDataStorage()->GetNamedNode("asis");
  mitk::DataNode *hipLengthRight = this->GetDataStorage()->GetNamedNode("hip_length_right");
  mitk::DataNode *hipLengthLeft = this->GetDataStorage()->GetNamedNode("hip_length_left");
  mitk::DataNode *pelvisMidline = this->GetDataStorage()->GetNamedNode("pelvis_midline");
  mitk::DataNode *femurLeftNode = this->GetDataStorage()->GetNamedNode("femur_left");
  mitk::DataNode *femurRightNode = this->GetDataStorage()->GetNamedNode("femur_right");
  mitk::DataNode *imageNode = this->GetDataStorage()->GetNamedNode("image");
  mitk::DataNode *pelvisNode = this->GetDataStorage()->GetNamedNode("pelvis");

  if (other)
  {
    this->GetMultiWidgetLayoutManager()->SetCurrentRenderWindowWidget(
        this->GetRenderWindowWidget(this->GetRenderWindow4()).get());
    this->GetMultiWidgetLayoutManager()->SetOneBigLayout();
  }
  else
  {
    // Opacity setting in mitkWorkbench may lead to volume rendering fail(show nothing).
    // while manually setting seems to work
    femurRightNode->SetOpacity(1);
    // Opacity setting in mitkWorkbench may lead to volume rendering fail(show nothing).
    // while manually setting seems to work
    femurLeftNode->SetOpacity(1);
    imageNode->SetProperty("volumerendering", mitk::BoolProperty::New(false));

    switch (this->view)
    {
    case VIEW_REAMING:
    {
      // this->GetMultiWidgetLayoutManager()->SetCurrentRenderWindowWidget(
      //   this->GetRenderWindowWidget(this->GetRenderWindow4()).get());
      // this->GetMultiWidgetLayoutManager()->SetOneBigLayout();
      this->GetMultiWidgetLayoutManager()->SetAll2DLeft3DRightLayout();
      imageNode->SetVisibility(true);
      mitk::Surface *pelvis = this->GetDataStorage()->GetNamedObject<mitk::Surface>("pelvis");
      mitk::Surface *acetabularShell = this->GetDataStorage()->GetNamedObject<mitk::Surface>("acetabular_shell");
      mitk::Surface *reamerTrajectory = this->GetDataStorage()->GetNamedObject<mitk::Surface>("reamer_trajectory");

      auto transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
      transformPolyData->SetInputData(reamerTrajectory->GetVtkPolyData());
      transformPolyData->SetTransform(acetabularShell->GetGeometry()->GetVtkTransform());
      transformPolyData->Update();

      auto selectedEnclosedPoints = vtkSmartPointer<vtkSelectEnclosedPoints>::New();
      selectedEnclosedPoints->SetInputData(pelvis->GetVtkPolyData());
      selectedEnclosedPoints->SetSurfaceData(transformPolyData->GetOutput());
      selectedEnclosedPoints->Update();
      vtkPolyData *selected = vtkPolyData::SafeDownCast(selectedEnclosedPoints->GetOutput());

      auto selectedPoints = selected->GetPointData()->GetArray("SelectedPoints");

      auto colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
      colors->SetNumberOfComponents(3);
      colors->SetName("colors");

      for (auto i = 0; i < selectedPoints->GetNumberOfTuples(); ++i)
      {
        selectedPoints->GetComponent(i, 0) == 1 ? colors->InsertNextTuple3(0, 255, 0) : colors->InsertNextTuple3(255, 255, 255);
      }
      selected->GetPointData()->SetScalars(colors);
      selected->GetPointData()->RemoveArray("SelectedPoints");

      mitk::Surface::Pointer reamingPelvis = this->GetDataStorage()->GetNamedObject<mitk::Surface>("reaming_pelvis");
      mitk::DataNode::Pointer reamingPelvisNode = this->GetDataStorage()->GetNamedNode("reaming_pelvis");
      if (reamingPelvis == nullptr)
      {
        reamingPelvis = mitk::Surface::New();
        reamingPelvisNode = mitk::DataNode::New();
        reamingPelvisNode->SetName("reaming_pelvis");
        reamingPelvisNode->SetData(reamingPelvis);
        reamingPelvisNode->SetBoolProperty("scalar visibility", true);
        this->GetDataStorage()->Add(reamingPelvisNode);
      }
      reamingPelvis->SetVtkPolyData(selected);
      reamingPelvisNode->SetVisibility(true);
    }
    break;
    case VIEW_CT:
    {
      // imageNode->SetVisibility(true);
      // //this->EnableStandardLevelWindow();
      // this->GetMultiWidgetLayoutManager()->SetDefaultLayout();
      // //this->changeLayoutTo2DImagesLeft();
      // this->GetRenderWindow4()->setVisible(false);
      this->GetMultiWidgetLayoutManager()->SetOnly2DHorizontalLayout();
      imageNode->SetVisibility(true);
    }
    break;
    case VIEW_3D_SLICER:
    {
      // //this->EnableStandardLevelWindow();
      // this->SetWidgetPlaneVisibility("pelvis", true, renderer1);
      // this->SetWidgetPlaneVisibility("pelvis", true, renderer2);
      // this->SetWidgetPlaneVisibility("pelvis", true, renderer3);
      this->GetMultiWidgetLayoutManager()->SetAll2DLeft3DRightLayout();
      imageNode->SetVisibility(true);
      pelvisNode->SetVisibility(true);
      // femurRightNode->SetVisibility(true);
      // femurLeftNode->SetVisibility(true);
    }
    break;
    case VIEW_X_RAY:
    {
      this->GetMultiWidgetLayoutManager()->SetCurrentRenderWindowWidget(
          this->GetRenderWindowWidget(this->GetRenderWindow4()).get());
      this->GetMultiWidgetLayoutManager()->SetOneBigLayout();
      imageNode->SetVisibility(true);
      imageNode->SetProperty("volumerendering", mitk::BoolProperty::New(true));
    }
    break;
    default: // VIEW_DEFAULT
    {
      this->GetMultiWidgetLayoutManager()->SetCurrentRenderWindowWidget(
          this->GetRenderWindowWidget(this->GetRenderWindow4()).get());
      this->GetMultiWidgetLayoutManager()->SetOneBigLayout();
      pelvisNode->SetVisibility(true);
      femurRightNode->SetVisibility(true);
      femurLeftNode->SetVisibility(true);
      // mitk::BaseRenderer *renderer = mitk::BaseRenderer::GetInstance(this->GetRenderWindow4()->renderWindow());
      // this->SetWidgetPlaneVisibility("image", false, renderer);
    }
    break;
    }

    switch (this->mode)
    {
    case MODE_PRE_OP:
    {
      trochanterNode->SetVisibility(true);
      trochanterRightLineNode->SetVisibility(true);
      trochanterLeftLineNode->SetVisibility(true);
      nativeCorNode->SetVisibility(true);
      // psisNode->SetVisibility(true);
      asisNode->SetVisibility(true);
      // hipLengthRight->SetVisibility(true);
      // hipLengthLeft->SetVisibility(true);
      pelvisMidline->SetVisibility(true);
      femurRightNode->SetVisibility(true);
      femurLeftNode->SetVisibility(true);
      this->implantAssessmentGadget[3]->setVisible(true);
    }
    break;
    case MODE_CUP_PLAN:
    {
      cupCor->SetVisibility(true);
      acetabularLiner->SetVisibility(true);
      acetabularShell->SetVisibility(true);
      nativeCorNode->SetVisibility(true);
      femurRightNode->SetVisibility(false);
      femurLeftNode->SetVisibility(false);
      this->groupBoxGadget[0]->setVisible(true);
      this->groupBoxGadget[0]->setMode(GroupBoxGadget::CUP);
      this->groupBoxGadget[1]->setVisible(true);
      this->groupBoxGadget[1]->setMode(GroupBoxGadget::CUP);
      this->groupBoxGadget[2]->setVisible(true);
      this->groupBoxGadget[2]->setMode(GroupBoxGadget::CUP);
      this->cupParameterGadget[3]->setVisible(true);
      this->implantAssessmentGadget[3]->setVisible(true);
    }
    break;
    case MODE_STEM_PLAN:
    {
      femoralHeadCor->SetVisibility(true);
      femoralHead->SetVisibility(true);
      femoralStem->SetVisibility(true);
      nativeCorNode->SetVisibility(true);
      femurRightNode->SetVisibility(false);
      femurLeftNode->SetVisibility(false);
      this->groupBoxGadget[0]->setVisible(true);
      this->groupBoxGadget[0]->setMode(GroupBoxGadget::STEM);
      this->groupBoxGadget[1]->setVisible(true);
      this->groupBoxGadget[1]->setMode(GroupBoxGadget::STEM);
      this->groupBoxGadget[2]->setVisible(true);
      this->groupBoxGadget[2]->setMode(GroupBoxGadget::STEM);
      this->stemParameterGadget[3]->setVisible(true);
      this->implantAssessmentGadget[3]->setVisible(true);
    }
    break;
    case MODE_REDUCED:
    {
      trochanterNode->SetVisibility(true);
      trochanterRightLineNode->SetVisibility(true);
      trochanterLeftLineNode->SetVisibility(true);
      asisNode->SetVisibility(true);
      pelvisMidline->SetVisibility(true);
      acetabularLiner->SetVisibility(true);
      acetabularShell->SetVisibility(true);
      cupCor->SetVisibility(true);
      femoralHead->SetVisibility(true);
      femoralStem->SetVisibility(true);
      femoralHeadCor->SetVisibility(true);
      nativeCorNode->SetVisibility(true);
      // Opacity setting in mitkWorkbench may lead to volume rendering fail(show nothing).
      // while manually setting seems to work
      femurRightNode->SetOpacity(0.5);
      femurRightNode->SetVisibility(true);
      // Opacity setting in mitkWorkbench may lead to volume rendering fail(show nothing).
      // while manually setting seems to work
      femurLeftNode->SetOpacity(0.5);
      femurLeftNode->SetVisibility(true);
      cupCor->SetVisibility(true);
      this->implantAssessmentGadget[3]->setVisible(true);
    }
    break;
    default: //MODE_DEFAULT
    {
      // acetabularLiner->SetVisibility(false);
      // acetabularShell->SetVisibility(false);
      // femoralHead->SetVisibility(false);
      // femoralStem->SetVisibility(false);
      if (this->view != VIEWS::VIEW_REAMING && this->view != VIEWS::VIEW_X_RAY)
      {
        femurRightNode->SetVisibility(true);
        femurLeftNode->SetVisibility(true);
      }
      this->implantAssessmentGadget[3]->setVisible(true);
    }
    break;
    }

    this->ResetCrosshair();
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  }
}
