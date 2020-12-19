#include "THAStdMultiWidget.h"
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
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
// qt
#include <QVBoxLayout>
// mitk
#include <mitkImage.h>
#include <mitkLogMacros.h>
#include <mitkSceneIO.h>
#include <mitkSurface.h>
#include <mitkLogMacros.h>
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
  mViewerMode(VIEWER_DEFAULT),
  mpPlanMode(PLAN_DEFAULT),
  registrationMode(false),
  mpGroupBoxGadget{
    new GroupBoxGadget(GroupBoxGadget::AXIAL, this),
    new GroupBoxGadget(GroupBoxGadget::SAGITTAL, this),
    new GroupBoxGadget(GroupBoxGadget::CORONAL, this)
  },
  mpStemParameterGadget(new StemParameterGadget(this)),
  mpImplantAssessmentGadget(new ImplantAssessmentGadget(this)),
  mpCupParameterGadget(new CupParameterGadget(this))
	//cupResultGadget(new CupResultGadget(this))
{
}
void THAStdMultiWidget::InitializeMultiWidget()
{
  this->SetDataStorage(mitk::RenderingManager::GetInstance()->GetDataStorage());
  QmitkStdMultiWidget::InitializeMultiWidget();
  // Add the displayed views to the DataStorage to see their positions in 2D and 3D
  this->AddDisplayPlaneSubTree();
  // show image plane in viewer and data storage
  // this->AddPlanesToDataStorage();
  // Disable the plane widget
  this->SetWidgetPlanesVisibility(false);
////////////////////////////////////////////////////////////////////////////////
/// Customization
////////////////////////////////////////////////////////////////////////////////

  this->GetRenderWindow4()->GetVtkRenderWindow()->SetMultiSamples(0);   // We do not support MSAA as it is incompatible with depth peeling
  this->GetRenderWindow4()->GetVtkRenderWindow()->SetAlphaBitPlanes(true); // Necessary for depth peeling
  this->GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetMaximumNumberOfPeels(50);
  this->GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetUseDepthPeeling(true);
  this->GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetUseDepthPeelingForVolumes(true);
  // Initialize bottom-right view as 3D view
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
  pRenderWindow1Layout->insertWidget(0, this->mpGroupBoxGadget[0], 0, Qt::AlignRight | Qt::AlignTop);
  this->GetRenderWindow1()->setLayout(pRenderWindow1Layout);

  QVBoxLayout *pRenderWindow2Layout = new QVBoxLayout(this->GetRenderWindow2());
  pRenderWindow2Layout->insertWidget(0, this->mpGroupBoxGadget[1], 0, Qt::AlignRight | Qt::AlignTop);
  this->GetRenderWindow2()->setLayout(pRenderWindow2Layout);

  QVBoxLayout *pRenderWindow3Layout = new QVBoxLayout(this->GetRenderWindow3());
  pRenderWindow3Layout->insertWidget(0, this->mpGroupBoxGadget[2], 0, Qt::AlignRight | Qt::AlignTop);
  this->GetRenderWindow3()->setLayout(pRenderWindow3Layout);

	QGridLayout *pRenderWindow4Layout = new QGridLayout(this->GetRenderWindow4());
	pRenderWindow4Layout->addWidget(this->mpStemParameterGadget, 0, 1, Qt::AlignRight | Qt::AlignBottom);
	pRenderWindow4Layout->addWidget(this->mpCupParameterGadget, 1, 1, Qt::AlignRight | Qt::AlignBottom);
	pRenderWindow4Layout->addWidget(this->mpImplantAssessmentGadget, 2, 1, Qt::AlignRight | Qt::AlignBottom);
  this->GetRenderWindow4()->setLayout(pRenderWindow4Layout);
////////////////////////////////////////////////////////////////////////////////
/// Gadget Connection
////////////////////////////////////////////////////////////////////////////////
  this->mpCupParameterGadget->observerCup();
	this->mpStemParameterGadget->observerStem();
////////////////////////////////////////////////////////////////////////////////
  this->SetRegistrationMode(this->registrationMode);
////////////////////////////////////////////////////////////////////////////////
/// Preset
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}

void THAStdMultiWidget::SetViewerMode(int mode)
{
  // if(this->viewerMode == mode) {
  //   return;
  // }
  this->mViewerMode = mode;
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
  
  switch (this->mViewerMode)
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

void THAStdMultiWidget::SetPlanMode(int mode)
{
  // if(this->planMode == mode) {
  //   return;
  // }
  this->mpPlanMode = mode;

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
  this->mpGroupBoxGadget[0]->setVisible(false);
  this->mpGroupBoxGadget[0]->setMode(GroupBoxGadget::DEFAULT);
  this->mpGroupBoxGadget[1]->setVisible(false);
  this->mpGroupBoxGadget[1]->setMode(GroupBoxGadget::DEFAULT);
  this->mpGroupBoxGadget[2]->setVisible(false);
  this->mpGroupBoxGadget[2]->setMode(GroupBoxGadget::DEFAULT);
  this->mpStemParameterGadget->setVisible(false);
  this->mpImplantAssessmentGadget->setVisible(false);
  this->mpCupParameterGadget->setVisible(false);

  switch (this->mpPlanMode)
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
    this->mpImplantAssessmentGadget->setVisible(true);
  }
  break;
  case PLAN_CUP:
  {
    this->mpGroupBoxGadget[0]->setVisible(true);
    this->mpGroupBoxGadget[0]->setMode(GroupBoxGadget::CUP);
    this->mpGroupBoxGadget[1]->setVisible(true);
    this->mpGroupBoxGadget[1]->setMode(GroupBoxGadget::CUP);
    this->mpGroupBoxGadget[2]->setVisible(true);
    this->mpGroupBoxGadget[2]->setMode(GroupBoxGadget::CUP);
		femoralNeckTransformedNode->SetColor(0.741, 0.741, 0.741);
		femoralHeadTransformedNode->SetColor(0.741, 0.741, 0.741);
		femoralStemTransformedNode->SetColor(0.741, 0.741, 0.741);
		this->mpGroupBoxGadget[0]->setButtonShow(true);
		this->mpGroupBoxGadget[1]->setButtonShow(true);
		this->mpGroupBoxGadget[2]->setButtonShow(true);
		this->mpCupParameterGadget->setVisible(true);
  }
  break;
  case PLAN_STEM:
  {
    this->mpGroupBoxGadget[0]->setVisible(true);
    this->mpGroupBoxGadget[0]->setMode(GroupBoxGadget::STEM);
    this->mpGroupBoxGadget[1]->setVisible(true);
    this->mpGroupBoxGadget[1]->setMode(GroupBoxGadget::STEM);
    this->mpGroupBoxGadget[2]->setVisible(true);
    this->mpGroupBoxGadget[2]->setMode(GroupBoxGadget::STEM);
		acetabularShellTransformedNode->SetColor(0.741, 0.741, 0.741);
		acetabularInsertTransformedNode->SetColor(0.741, 0.741, 0.741);
		this->mpGroupBoxGadget[0]->setButtonShow(false);
		this->mpGroupBoxGadget[1]->setButtonShow(false);
		this->mpGroupBoxGadget[2]->setButtonShow(false);
		this->mpStemParameterGadget->setVisible(true);
    this->mpImplantAssessmentGadget->setVisible(true);
  }
  break;
  case PLAN_REDUCED:
  {
		this->mpCupParameterGadget->setVisible(true);
		this->mpStemParameterGadget->setVisible(true);
    this->mpImplantAssessmentGadget->setVisible(true);
  }
	break;
  default: //PLAN_DEFAULT
  {
		acetabularShellTransformedNode->SetVisibility(false);
		acetabularInsertTransformedNode->SetVisibility(false);
		femoralNeckTransformedNode->SetVisibility(false);
		femoralHeadTransformedNode->SetVisibility(false);
		femoralStemTransformedNode->SetVisibility(false);
    this->mpImplantAssessmentGadget->setVisible(true);
  }
  break;
  }
  this->ResetCrosshair();
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void THAStdMultiWidget::SetRegistrationMode(bool flag)
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
    this->SetViewerMode(this->mViewerMode);
    this->SetPlanMode(this->mpPlanMode);
  }
  else
  {
    this->mpGroupBoxGadget[0]->setVisible(false);
    this->mpGroupBoxGadget[1]->setVisible(false);
    this->mpGroupBoxGadget[2]->setVisible(false);
    this->mpStemParameterGadget->setVisible(false);
    this->mpImplantAssessmentGadget->setVisible(false);
    this->mpCupParameterGadget->setVisible(false);
    // this->changeLayoutToBig3D();
    this->GetMultiWidgetLayoutManager()->SetCurrentRenderWindowWidget(
      this->GetRenderWindowWidget(this->GetRenderWindow4()).get());
    this->GetMultiWidgetLayoutManager()->SetOneBigLayout();
    this->ResetCrosshair();
  }
}