#include "THAStdMultiWidget.h"
// #include "THADataStorage.h"
#include "GroupBoxGadget.h"
#include "StemParameterGadget.h"
#include "ImplantAssessmentGadget.h"
#include "CupParameterGadget.h"
// vtk
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include "vtkLineSource.h"
#include "vtkPolyDataMapper.h"
#include <vtkActor.h>
// qt
#include <QVBoxLayout>
// mitk
#include <mitkImage.h>
#include <mitkLogMacros.h>
#include <mitkSceneIO.h>
#include <mitkSurface.h>
#include <mitkLogMacros.h>
// #include <mitkLocalStorageHandler.h>
#include <QmitkRenderWindow.h>

static const std::set<std::string> DEFAULT_VISIBLE_SET = {
  "hip_length_right",
  "hip_length_left",
  "trochanter",
  "COR",
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
  viewerMode(VIEWER_DEFAULT),
  planMode(PLAN_DEFAULT),
  registrationMode(false),
  groupBoxGadget{
    new GroupBoxGadget(GroupBoxGadget::AXIAL, this),
    new GroupBoxGadget(GroupBoxGadget::SAGITTAL, this),
    new GroupBoxGadget(GroupBoxGadget::CORONAL, this)
  },
  stemParameterGadget(new StemParameterGadget(this)),
  implantAssessmentGadget(new ImplantAssessmentGadget(this)),
  cupParameterGadget(new CupParameterGadget(this))
	//cupResultGadget(new CupResultGadget(this))
{
////////////////////////////////////////////////////////////////////////////////
/// Depth Peeling for Translucent Rendering 
////////////////////////////////////////////////////////////////////////////////
  // vtkRenderWindow *renderWindow = this->GetRenderWindow4()->GetVtkRenderWindow();
  // vtkRenderer *renderer = mitk::BaseRenderer::GetInstance(this->GetRenderWindow4()->renderWindow())->GetVtkRenderer();
  // // 1. Use a render window with alpha bits (as initial value is 0 (false)):
  // renderWindow->SetAlphaBitPlanes(true);

  // // 2. Force to not pick a framebuffer with a multisample buffer
  // // (as initial value is 8):
  // renderWindow->SetMultiSamples(0);

  // // 3. Choose to use depth peeling (if supported) (initial value is 0 (false)):
  // renderer->SetUseDepthPeeling(true);

  // // 4. Set depth peeling parameters
  // // - Set the maximum number of rendering passes (initial value is 4):
  // int maxNoOfPeels = 100;
  // renderer->SetMaximumNumberOfPeels(maxNoOfPeels);
  // // - Set the occlusion ratio (initial value is 0.0, exact image):
  // double occlusionRatio = 0.1;
  // renderer->SetOcclusionRatio(occlusionRatio);
////////////////////////////////////////////////////////////////////////////////
}
void THAStdMultiWidget::InitializeMultiWidget()
{
  QmitkStdMultiWidget::InitializeMultiWidget();
////////////////////////////////////////////////////////////////////////////////
/// Customization
////////////////////////////////////////////////////////////////////////////////

  this->GetRenderWindow4()->GetVtkRenderWindow()->SetMultiSamples(0);   // We do not support MSAA as it is incompatible with depth peeling
  this->GetRenderWindow4()->GetVtkRenderWindow()->SetAlphaBitPlanes(true); // Necessary for depth peeling
  this->GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetMaximumNumberOfPeels(50);
  this->GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetUseDepthPeeling(true);
  this->GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetUseDepthPeelingForVolumes(true);
  this->GetRenderWindow4()->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);

  this->GetRenderWindow1()->GetVtkRenderWindow()->SetWindowName(
    (this->GetRenderWindow1()->objectName() + '1').toStdString().c_str()
  );
  this->GetRenderWindow2()->GetVtkRenderWindow()->SetWindowName(
    (this->GetRenderWindow2()->objectName() + '2').toStdString().c_str()
  );
  this->GetRenderWindow3()->GetVtkRenderWindow()->SetWindowName(
    (this->GetRenderWindow3()->objectName() + '3').toStdString().c_str()
  );
  this->GetRenderWindow4()->GetVtkRenderWindow()->SetWindowName(
    (this->GetRenderWindow4()->objectName() + '4').toStdString().c_str()
  );

  // this->DisableStandardLevelWindow();
  QVBoxLayout *pRenderWindow1Layout = new QVBoxLayout(this->GetRenderWindow1());
  pRenderWindow1Layout->insertWidget(0, this->groupBoxGadget[0], 0, Qt::AlignRight | Qt::AlignTop);
  this->GetRenderWindow1()->setLayout(pRenderWindow1Layout);

  QVBoxLayout *pRenderWindow2Layout = new QVBoxLayout(this->GetRenderWindow2());
  pRenderWindow2Layout->insertWidget(0, this->groupBoxGadget[1], 0, Qt::AlignRight | Qt::AlignTop);
  this->GetRenderWindow2()->setLayout(pRenderWindow2Layout);

  QVBoxLayout *pRenderWindow3Layout = new QVBoxLayout(this->GetRenderWindow3());
  pRenderWindow3Layout->insertWidget(0, this->groupBoxGadget[2], 0, Qt::AlignRight | Qt::AlignTop);
  this->GetRenderWindow3()->setLayout(pRenderWindow3Layout);

	QGridLayout *pRenderWindow4Layout = new QGridLayout(this->GetRenderWindow4());
	pRenderWindow4Layout->addWidget(this->stemParameterGadget, 0, 1, Qt::AlignRight | Qt::AlignBottom);
	pRenderWindow4Layout->addWidget(this->cupParameterGadget, 1, 1, Qt::AlignRight | Qt::AlignBottom);
	pRenderWindow4Layout->addWidget(this->implantAssessmentGadget, 2, 1, Qt::AlignRight | Qt::AlignBottom);
  this->GetRenderWindow4()->setLayout(pRenderWindow4Layout);
	this->GetRenderWindow4()->setMaximumHeight(600);
////////////////////////////////////////////////////////////////////////////////
/// Preset
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}

void THAStdMultiWidget::setViewerMode(int mode)
{
  // if(this->viewerMode == mode) {
  //   return;
  // }
  this->viewerMode = mode;
  // this->DisableStandardLevelWindow();
  mitk::BaseRenderer *renderer1 =
      mitk::BaseRenderer::GetInstance(this->GetRenderWindow1()->renderWindow());
  mitk::BaseRenderer *renderer2 =
      mitk::BaseRenderer::GetInstance(this->GetRenderWindow2()->renderWindow());
  mitk::BaseRenderer *renderer3 =
      mitk::BaseRenderer::GetInstance(this->GetRenderWindow3()->renderWindow());

  mitk::DataNode *imageNode = this->GetDataStorage()->GetNamedNode("image");
  mitk::DataNode *pelvisNode = this->GetDataStorage()->GetNamedNode("pelvis");
  mitk::DataNode *femurLeftNode = this->GetDataStorage()->GetNamedNode("femur_left");
  mitk::DataNode *femurRightNode = this->GetDataStorage()->GetNamedNode("femur_right");
  imageNode->SetVisibility(false);
  pelvisNode->SetVisibility(false);
  femurRightNode->SetVisibility(false);
  femurLeftNode->SetVisibility(false);
  imageNode->SetProperty("volumerendering", mitk::BoolProperty::New(false));
  this->SetWidgetPlaneVisibility("pelvis", false, renderer1);
  this->SetWidgetPlaneVisibility("pelvis", false, renderer2);
  this->SetWidgetPlaneVisibility("pelvis", false, renderer3);
  this->SetWidgetPlaneVisibility("femur_left", false, renderer1);
  this->SetWidgetPlaneVisibility("femur_left", false, renderer2);
  this->SetWidgetPlaneVisibility("femur_left", false, renderer3);
  this->SetWidgetPlaneVisibility("femur_right", false, renderer1);
  this->SetWidgetPlaneVisibility("femur_right", false, renderer2);
  this->SetWidgetPlaneVisibility("femur_right", false, renderer3);
  
  switch (this->viewerMode)
  {
  case VIEWER_2D:
  {
    imageNode->SetVisibility(true);
    //this->EnableStandardLevelWindow();
    this->GetMultiWidgetLayoutManager()->SetDefaultLayout();
		//this->changeLayoutTo2DImagesLeft();
    this->GetRenderWindow4()->setVisible(false);
  }
  break;
  case VIEWER_3D:
  {
    //this->EnableStandardLevelWindow();
    imageNode->SetVisibility(true);
    pelvisNode->SetVisibility(true);
    femurRightNode->SetVisibility(true);
    femurLeftNode->SetVisibility(true);
    this->SetWidgetPlaneVisibility("pelvis", true, renderer1);
    this->SetWidgetPlaneVisibility("pelvis", true, renderer2);
    this->SetWidgetPlaneVisibility("pelvis", true, renderer3);
    //this->changeLayoutToDefault();
		this->GetMultiWidgetLayoutManager()->SetAll2DLeft3DRightLayout();
  }
  break;
  case VIEWER_XRAY:
  {
		// this->changeLayoutToBig3D();
    this->GetMultiWidgetLayoutManager()->SetCurrentRenderWindowWidget(
      this->GetRenderWindowWidget(this->GetRenderWindow4()).get());
    this->GetMultiWidgetLayoutManager()->SetOneBigLayout();
		pelvisNode->SetVisibility(true);
		femurRightNode->SetVisibility(true);
		femurLeftNode->SetVisibility(true);
		//imageNode->SetVisibility(true);
		//this->changeLayoutToBig3D();
		//mitk::BaseRenderer *renderer = mitk::BaseRenderer::GetInstance(this->GetRenderWindow4()->renderWindow());
		//imageNode->SetProperty("volumerendering", mitk::BoolProperty::New(true));
  }
  break;
  default: // VIEWER_DEFAULT
  {
    // this->changeLayoutToBig3D();
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
  this->ResetCrosshair();
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void THAStdMultiWidget::setPlanMode(int mode)
{
  // if(this->planMode == mode) {
  //   return;
  // }
  this->planMode = mode;

  mitk::DataNode *acetabularShellTransformedNode = this->GetDataStorage()->GetNamedNode("acetabular_shell_transformed");
  mitk::DataNode *acetabularInsertTransformedNode = this->GetDataStorage()->GetNamedNode("acetabular_insert_transformed");
  mitk::DataNode *femoralNeckTransformedNode = this->GetDataStorage()->GetNamedNode("femoral_neck_transformed");
  mitk::DataNode *femoralHeadTransformedNode = this->GetDataStorage()->GetNamedNode("femoral_head_transformed");
  mitk::DataNode *femoralStemTransformedNode = this->GetDataStorage()->GetNamedNode("femoral_stem_transformed");
  mitk::DataNode *trochanterNode = this->GetDataStorage()->GetNamedNode("trochanter");
  mitk::DataNode *corNode = this->GetDataStorage()->GetNamedNode("COR");
  mitk::DataNode *psisNode = this->GetDataStorage()->GetNamedNode("PSIS");
  mitk::DataNode *asisNode = this->GetDataStorage()->GetNamedNode("ASIS");
  mitk::DataNode *hipLengthRight = this->GetDataStorage()->GetNamedNode("hip_length_right");
  mitk::DataNode *hipLengthLeft = this->GetDataStorage()->GetNamedNode("hip_length_left");
  mitk::DataNode *pelvisMidline = this->GetDataStorage()->GetNamedNode("pelvis_midline");
	acetabularShellTransformedNode->SetVisibility(true);
	acetabularInsertTransformedNode->SetVisibility(true);
	femoralNeckTransformedNode->SetVisibility(true);
	femoralHeadTransformedNode->SetVisibility(true);
	femoralStemTransformedNode->SetVisibility(true);
	acetabularShellTransformedNode->SetColor(0, 1, 0);
	acetabularInsertTransformedNode->SetColor(1, 1, 1);
	femoralNeckTransformedNode->SetColor(0, 0, 1);
	femoralHeadTransformedNode->SetColor(1, 1, 0);
	femoralStemTransformedNode->SetColor(0, 0, 1);
  trochanterNode->SetVisibility(false);
  corNode->SetVisibility(false);
  psisNode->SetVisibility(false);
  asisNode->SetVisibility(false);
  hipLengthRight->SetVisibility(false);
  hipLengthLeft->SetVisibility(false);
  pelvisMidline->SetVisibility(false);
  this->groupBoxGadget[0]->setVisible(false);
  this->groupBoxGadget[0]->setMode(GroupBoxGadget::DEFAULT);
  this->groupBoxGadget[1]->setVisible(false);
  this->groupBoxGadget[1]->setMode(GroupBoxGadget::DEFAULT);
  this->groupBoxGadget[2]->setVisible(false);
  this->groupBoxGadget[2]->setMode(GroupBoxGadget::DEFAULT);
  this->stemParameterGadget->setVisible(false);
  this->implantAssessmentGadget->setVisible(false);
  this->cupParameterGadget->setVisible(false);

  switch (this->planMode)
  {
  case PLAN_PREOP:
  {
		acetabularShellTransformedNode->SetVisibility(false);
		acetabularInsertTransformedNode->SetVisibility(false);
		femoralNeckTransformedNode->SetVisibility(false);
		femoralHeadTransformedNode->SetVisibility(false);
		femoralStemTransformedNode->SetVisibility(false);
    trochanterNode->SetVisibility(true);
    corNode->SetVisibility(true);
    psisNode->SetVisibility(true);
    asisNode->SetVisibility(true);
    hipLengthRight->SetVisibility(true);
    hipLengthLeft->SetVisibility(true);
    pelvisMidline->SetVisibility(true);
    this->implantAssessmentGadget->setVisible(true);
  }
  break;
  case PLAN_CUP:
  {
    this->groupBoxGadget[0]->setVisible(true);
    this->groupBoxGadget[0]->setMode(GroupBoxGadget::CUP);
    this->groupBoxGadget[1]->setVisible(true);
    this->groupBoxGadget[1]->setMode(GroupBoxGadget::CUP);
    this->groupBoxGadget[2]->setVisible(true);
    this->groupBoxGadget[2]->setMode(GroupBoxGadget::CUP);
		femoralNeckTransformedNode->SetColor(0.741, 0.741, 0.741);
		femoralHeadTransformedNode->SetColor(0.741, 0.741, 0.741);
		femoralStemTransformedNode->SetColor(0.741, 0.741, 0.741);
		this->groupBoxGadget[0]->setButtonShow(true);
		this->groupBoxGadget[1]->setButtonShow(true);
		this->groupBoxGadget[2]->setButtonShow(true);
		this->cupParameterGadget->setVisible(true);
  }
  break;
  case PLAN_STEM:
  {
    this->groupBoxGadget[0]->setVisible(true);
    this->groupBoxGadget[0]->setMode(GroupBoxGadget::STEM);
    this->groupBoxGadget[1]->setVisible(true);
    this->groupBoxGadget[1]->setMode(GroupBoxGadget::STEM);
    this->groupBoxGadget[2]->setVisible(true);
    this->groupBoxGadget[2]->setMode(GroupBoxGadget::STEM);
		acetabularShellTransformedNode->SetColor(0.741, 0.741, 0.741);
		acetabularInsertTransformedNode->SetColor(0.741, 0.741, 0.741);
		this->groupBoxGadget[0]->setButtonShow(false);
		this->groupBoxGadget[1]->setButtonShow(false);
		this->groupBoxGadget[2]->setButtonShow(false);
		this->stemParameterGadget->setVisible(true);
    this->implantAssessmentGadget->setVisible(true);
  }
  break;
  case PLAN_REDUCED:
  {
		this->cupParameterGadget->setVisible(true);
		this->stemParameterGadget->setVisible(true);
    this->implantAssessmentGadget->setVisible(true);
  }
	break;
  default: //PLAN_DEFAULT
  {
		acetabularShellTransformedNode->SetVisibility(false);
		acetabularInsertTransformedNode->SetVisibility(false);
		femoralNeckTransformedNode->SetVisibility(false);
		femoralHeadTransformedNode->SetVisibility(false);
		femoralStemTransformedNode->SetVisibility(false);
    this->implantAssessmentGadget->setVisible(true);
  }
  break;
  }
  this->ResetCrosshair();
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void THAStdMultiWidget::setRegistrationMode(bool flag)
{
  this->registrationMode = flag;
////////////////////////////////////////////////////////////////////////////////
/// Set Visibility
////////////////////////////////////////////////////////////////////////////////
  mitk::DataStorage::SetOfObjects::ConstPointer all = this->GetDataStorage()->GetAll();

  for(mitk::DataNode *one: *all)
  {
    one->SetVisibility(false);
	//MITK_INFO << DEFAULT_VISIBLE_SET.cend();
	//MITK_INFO << DEFAULT_VISIBLE_SET.find(one->GetName());
    if (DEFAULT_VISIBLE_SET.cend() != DEFAULT_VISIBLE_SET.find(one->GetName()))
    {
      one->SetVisibility(!this->registrationMode);
    }
  }

  // if not registration mode, just go back to the state which it used to be.
  if(!this->registrationMode)
  {
    this->setViewerMode(this->viewerMode);
    this->setPlanMode(this->planMode);
  }
  else
  {
    this->groupBoxGadget[0]->setVisible(false);
    this->groupBoxGadget[1]->setVisible(false);
    this->groupBoxGadget[2]->setVisible(false);
    this->stemParameterGadget->setVisible(false);
    this->implantAssessmentGadget->setVisible(false);
    this->cupParameterGadget->setVisible(false);
    // this->changeLayoutToBig3D();
    this->GetMultiWidgetLayoutManager()->SetCurrentRenderWindowWidget(
      this->GetRenderWindowWidget(this->GetRenderWindow4()).get());
    this->GetMultiWidgetLayoutManager()->SetOneBigLayout();
    this->ResetCrosshair();
  }
}

void THAStdMultiWidget::visualize(mitk::DataStorage *ds, QString side)
{
  this->SetDataStorage(ds);
  this->InitializeMultiWidget();
  this->implantAssessmentGadget->SetSide(side);

  // Initialize bottom-right view as 3D view
  // this->GetRenderWindow4()->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);

  // Add the displayed views to the DataStorage to see their positions in 2D and 3D
  this->AddDisplayPlaneSubTree();
  // show image plane in viewer and data storage
  // this->AddPlanesToDataStorage();
  // Disable the plane widget
  this->SetWidgetPlanesVisibility(false);

  // mitk::BaseRenderer *renderer1 =
  //     mitk::BaseRenderer::GetInstance(this->GetRenderWindow1()->renderWindow());
  // mitk::BaseRenderer *renderer2 =
  //     mitk::BaseRenderer::GetInstance(this->GetRenderWindow2()->renderWindow());
  // mitk::BaseRenderer *renderer3 =
  //     mitk::BaseRenderer::GetInstance(this->GetRenderWindow3()->renderWindow());
////////////////////////////////////////////////////////////////////////////////
/// Gadget Connection
////////////////////////////////////////////////////////////////////////////////
  this->cupParameterGadget->observerCup();
	this->stemParameterGadget->observerStem();
////////////////////////////////////////////////////////////////////////////////
  this->setRegistrationMode(this->registrationMode);
}

//void THAStdMultiWidget::setActualResult(bool b)
//{
//	//this->cupParameterGadget->setActualResult(b);
//	this->cupParameterGadget->setVisible(b);
//}
