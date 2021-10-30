#include "THAStdMultiWidget.h"

#include "CupParameterGadget.h"
#include "GroupBoxGadget.h"
#include "ImplantAssessmentGadget.h"
#include "ReamingFilter2.h"
#include "StemParameterGadget.h"

// vtk
#include <vtkTransformPolyDataFilter.h>

// qt
#include <QGridLayout>
#include <algorithm>

// mitk
// #include <mitkImage.h>
// #include <mitkLogMacros.h>
// #include <mitkSceneIO.h>
// #include <mitkSurface.h>
#include <QmitkRenderWindow.h>
#include <mitkBaseRenderer.h>
#include <mitkCommon.h>
#include <mitkLogMacros.h>
#include <mitkMapper.h>
#include <mitkVtkPropRenderer.h>

static const std::set<std::string> DEFAULT_VISIBLE_SET = {"hip_length_right",
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

THAStdMultiWidget::THAStdMultiWidget(QWidget* parent, Qt::WindowFlags f, const QString& name) :
    QmitkStdMultiWidget(parent, f, name),
    mGroupBoxGadget{new GroupBoxGadget(GroupBoxGadget::AXIAL, this),
                    new GroupBoxGadget(GroupBoxGadget::SAGITTAL, this),
                    new GroupBoxGadget(GroupBoxGadget::CORONAL, this),
                    new GroupBoxGadget(GroupBoxGadget::CORONAL, this)},
    mStemParameterGadget{new StemParameterGadget(this),
                         new StemParameterGadget(this),
                         new StemParameterGadget(this),
                         new StemParameterGadget(this)},
    mImplantAssessmentGadget{new ImplantAssessmentGadget(this),
                             new ImplantAssessmentGadget(this),
                             new ImplantAssessmentGadget(this),
                             new ImplantAssessmentGadget(this)},
    mCupParameterGadget{new CupParameterGadget(this),
                        new CupParameterGadget(this),
                        new CupParameterGadget(this),
                        new CupParameterGadget(this)}
{
}

void THAStdMultiWidget::InitializeMultiWidget()
{
    SetDataStorage(mitk::RenderingManager::GetInstance()->GetDataStorage());
    QmitkStdMultiWidget::InitializeMultiWidget();
    // show image plane in viewer and data storage
    AddPlanesToDataStorage();
    SetCrosshairVisibility(false);
    ////////////////////////////////////////////////////////////////////////////////
    /// Customization
    ////////////////////////////////////////////////////////////////////////////////

    // Disable the plane widget in 3D view
    SetWidgetPlanesVisibility(false, mitk::BaseRenderer::GetInstance(GetRenderWindow4()->GetVtkRenderWindow()));

    // GetRenderWindow4()->GetVtkRenderWindow()->SetMultiSamples(0);   // We do not support MSAA as it is incompatible
    // with depth peeling GetRenderWindow4()->GetVtkRenderWindow()->SetAlphaBitPlanes(true); // Necessary for depth
    // peeling GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetMaximumNumberOfPeels(50);
    // GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetUseDepthPeeling(true);
    // GetRenderWindow4()->GetRenderer()->GetVtkRenderer()->SetUseDepthPeelingForVolumes(true);
    // // Initialize bottom-right view as 3D view
    // GetRenderWindow4()->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);

    // GetRenderWindow1()->GetVtkRenderWindow()->SetWindowName(
    //   (GetRenderWindow1()->objectName() + '1').toStdString().c_str()
    // );
    // GetRenderWindow2()->GetVtkRenderWindow()->SetWindowName(
    //   (GetRenderWindow2()->objectName() + '2').toStdString().c_str()
    // );
    // GetRenderWindow3()->GetVtkRenderWindow()->SetWindowName(
    //   (GetRenderWindow3()->objectName() + '3').toStdString().c_str()
    // );
    // GetRenderWindow4()->GetVtkRenderWindow()->SetWindowName(
    //   (GetRenderWindow4()->objectName() + '4').toStdString().c_str()
    // );

    for (int i = 0; i < 4; ++i)
    {
        auto* renderWindow = GetRenderWindow(i);
        auto* pRenderWindow1Layout = new QGridLayout(renderWindow);
        pRenderWindow1Layout->addWidget(mGroupBoxGadget[i], 0, 1, Qt::AlignRight | Qt::AlignTop);
        pRenderWindow1Layout->addWidget(mStemParameterGadget[i], 1, 1, Qt::AlignRight | Qt::AlignBottom);
        pRenderWindow1Layout->addWidget(mCupParameterGadget[i], 2, 1, Qt::AlignRight | Qt::AlignBottom);
        pRenderWindow1Layout->addWidget(mImplantAssessmentGadget[i], 3, 1, Qt::AlignRight | Qt::AlignBottom);
        renderWindow->setLayout(pRenderWindow1Layout);
    }
    ////////////////////////////////////////////////////////////////////////////////
    /// Gadget Connection
    ////////////////////////////////////////////////////////////////////////////////

    for (int i = 0; i < 4; ++i)
    {
        mCupParameterGadget[i]->ObserverCup();
        mStemParameterGadget[i]->ObserverStem();
    }
    ////////////////////////////////////////////////////////////////////////////////
    setView(VIEWS::VIEW_DEFAULT);
    setMode(MODES::MODE_DEFAULT);
    UpdateViewMode();
    ////////////////////////////////////////////////////////////////////////////////
    /// Preset
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
}

void THAStdMultiWidget::setView(int mode)
{
    mView = mode;
    UpdateViewMode();
}

void THAStdMultiWidget::setMode(int mode)
{
    mMode = mode;
    UpdateViewMode();
}

void THAStdMultiWidget::setOther(bool other)
{
    mOther = other;
    UpdateViewMode();
}

void THAStdMultiWidget::setMeanIntensityProjection(bool flag)
{
    mMeanIntensityProjection = flag;
    auto* imageNode = GetDataStorage()->GetNamedNode("image");
    imageNode->SetVisibility(true);
    GetMultiWidgetLayoutManager()->SetCurrentRenderWindowWidget(GetRenderWindowWidget(GetRenderWindow3()).get());
    GetMultiWidgetLayoutManager()->SetOneBigLayout();
    // SetWidgetPlanesVisibility(true, mitk::BaseRenderer::GetInstance(GetRenderWindow4()->GetVtkRenderWindow()));
    // SetWidgetPlanesVisibility(true);
    // GetWidgetPlane3()->SetVisibility(true, mitk::BaseRenderer::GetInstance(GetRenderWindow4()->GetVtkRenderWindow()));

    auto* renderer =
        static_cast<mitk::VtkPropRenderer*>(mitk::BaseRenderer::GetInstance(GetRenderWindow3()->GetVtkRenderWindow()));
    auto map = renderer->GetMappersMap();
    for(auto elem : map)
    {
        if (elem.second->GetDataNode() == imageNode)
        {
            MITK_INFO << elem.first;
            MITK_INFO << *elem.second;
        }
    }
    // mitk::Mapper* imageMapper;
    // auto cit = std::find_if(
    //     map.cbegin(), map.cend(), [&imageNode](auto elem) { return elem.second->GetDataNode() == imageNode; });
    // if (cit != map.cend())
    // {
    //     MITK_INFO << *cit->second;
    // }

    MITK_INFO << *imageNode->GetMapper(mitk::BaseRenderer::Standard3D);
    MITK_INFO << *imageNode->GetMapper(mitk::BaseRenderer::Standard2D);
}

void THAStdMultiWidget::UpdateViewMode()
{
    ////////////////////////////////////////////////////////////////////////////////
    /// Set Visibility
    ////////////////////////////////////////////////////////////////////////////////
    for (auto i = 0; i < 4; ++i)
    {
        mGroupBoxGadget[i]->setVisible(false);
        mGroupBoxGadget[i]->setMode(GroupBoxGadget::DEFAULT);
        mStemParameterGadget[i]->setVisible(false);
        mImplantAssessmentGadget[i]->setVisible(false);
        mCupParameterGadget[i]->setVisible(false);
    }
    mitk::DataStorage::SetOfObjects::ConstPointer all = GetDataStorage()->GetAll();
    for (const auto& one : *all)
    {
        bool isHelperObject = false;
        if (!one->GetBoolProperty("helper object", isHelperObject) || !isHelperObject)
        {
            one->SetVisibility(false);
        }
    }
    auto* cupCor = GetDataStorage()->GetNamedNode("cup_cor");
    auto* acetabularLiner = GetDataStorage()->GetNamedNode("acetabular_liner");
    auto* acetabularShell = GetDataStorage()->GetNamedNode("acetabular_shell");
    auto* femoralHeadCor = GetDataStorage()->GetNamedNode("femoral_head_cor");
    auto* femoralStem = GetDataStorage()->GetNamedNode("femoral_stem");
    auto* femoralHead = GetDataStorage()->GetNamedNode("femoral_head");
    auto* trochanterNode = GetDataStorage()->GetNamedNode("trochanter");
    auto* trochanterRightLineNode = GetDataStorage()->GetNamedNode("trochanter_right_line");
    auto* trochanterLeftLineNode = GetDataStorage()->GetNamedNode("trochanter_left_line");
    auto* nativeCorNode = GetDataStorage()->GetNamedNode("native_cor");
    auto* psisNode = GetDataStorage()->GetNamedNode("psis");
    auto* asisNode = GetDataStorage()->GetNamedNode("asis");
    auto* hipLengthRight = GetDataStorage()->GetNamedNode("hip_length_right");
    auto* hipLengthLeft = GetDataStorage()->GetNamedNode("hip_length_left");
    auto* pelvisMidline = GetDataStorage()->GetNamedNode("pelvis_midline");
    auto* femurLeftNode = GetDataStorage()->GetNamedNode("femur_left");
    auto* femurRightNode = GetDataStorage()->GetNamedNode("femur_right");
    auto* imageNode = GetDataStorage()->GetNamedNode("image");
    auto* pelvisNode = GetDataStorage()->GetNamedNode("pelvis");

    if (mOther)
    {
        GetMultiWidgetLayoutManager()->SetCurrentRenderWindowWidget(GetRenderWindowWidget(GetRenderWindow4()).get());
        GetMultiWidgetLayoutManager()->SetOneBigLayout();
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

        switch (mView)
        {
            case VIEW_REAMING: {
                GetMultiWidgetLayoutManager()->SetAll2DLeft3DRightLayout();
                imageNode->SetVisibility(true);
                auto* pelvis = GetDataStorage()->GetNamedObject<mitk::Surface>("pelvis");
                auto* acetabularShell = GetDataStorage()->GetNamedObject<mitk::Surface>("acetabular_shell");
                auto* reamerTrajectory = GetDataStorage()->GetNamedObject<mitk::Surface>("reamer_trajectory");

                auto transformPolyData = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
                transformPolyData->SetInputData(reamerTrajectory->GetVtkPolyData());
                transformPolyData->SetTransform(acetabularShell->GetGeometry()->GetVtkTransform());
                transformPolyData->Update();

                auto reamingFilter = vtkSmartPointer<ReamingFilter2>::New();
                reamingFilter->SetInputData(pelvis->GetVtkPolyData());
                reamingFilter->SetInputData(1, transformPolyData->GetOutput());
                reamingFilter->Update();
                vtkPolyData* selected = reamingFilter->GetOutput();

                mitk::Surface::Pointer reamingPelvis =
                    GetDataStorage()->GetNamedObject<mitk::Surface>("reaming_pelvis");
                mitk::DataNode::Pointer reamingPelvisNode = GetDataStorage()->GetNamedNode("reaming_pelvis");
                if (reamingPelvis == nullptr)
                {
                    reamingPelvis = mitk::Surface::New();
                    reamingPelvisNode = mitk::DataNode::New();
                    reamingPelvisNode->SetName("reaming_pelvis");
                    reamingPelvisNode->SetData(reamingPelvis);
                    reamingPelvisNode->SetBoolProperty("scalar visibility", true);
                    GetDataStorage()->Add(reamingPelvisNode);
                }
                reamingPelvis->SetVtkPolyData(selected);
                reamingPelvisNode->SetVisibility(true);
            }
            break;
            case VIEW_CT: {
                // imageNode->SetVisibility(true);
                // //EnableStandardLevelWindow();
                // GetMultiWidgetLayoutManager()->SetDefaultLayout();
                // //changeLayoutTo2DImagesLeft();
                // GetRenderWindow4()->setVisible(false);
                GetMultiWidgetLayoutManager()->SetOnly2DHorizontalLayout();
                imageNode->SetVisibility(true);
            }
            break;
            case VIEW_3D_SLICER: {
                // //EnableStandardLevelWindow();
                // SetWidgetPlaneVisibility("pelvis", true, renderer1);
                // SetWidgetPlaneVisibility("pelvis", true, renderer2);
                // SetWidgetPlaneVisibility("pelvis", true, renderer3);
                GetMultiWidgetLayoutManager()->SetAll2DLeft3DRightLayout();
                imageNode->SetVisibility(true);

                pelvisNode->SetVisibility(true);
                // femurRightNode->SetVisibility(true);
                // femurLeftNode->SetVisibility(true);
            }
            break;
            case VIEW_X_RAY: {
                GetMultiWidgetLayoutManager()->SetCurrentRenderWindowWidget(
                    GetRenderWindowWidget(GetRenderWindow4()).get());
                GetMultiWidgetLayoutManager()->SetOneBigLayout();
                imageNode->SetVisibility(true);
                imageNode->SetProperty("volumerendering", mitk::BoolProperty::New(true));
            }
            break;
            default:  // VIEW_DEFAULT
            {
                GetMultiWidgetLayoutManager()->SetCurrentRenderWindowWidget(
                    GetRenderWindowWidget(GetRenderWindow4()).get());
                GetMultiWidgetLayoutManager()->SetOneBigLayout();
                pelvisNode->SetVisibility(true);
                femurRightNode->SetVisibility(true);
                femurLeftNode->SetVisibility(true);
                // mitk::BaseRenderer *renderer = mitk::BaseRenderer::GetInstance(GetRenderWindow4()->renderWindow());
                // SetWidgetPlaneVisibility("image", false, renderer);
            }
            break;
        }

        switch (mMode)
        {
            case MODE_PRE_OP: {
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
                mImplantAssessmentGadget[3]->setVisible(true);
            }
            break;
            case MODE_CUP_PLAN: {
                cupCor->SetVisibility(true);
                acetabularLiner->SetVisibility(true);
                acetabularShell->SetVisibility(true);
                nativeCorNode->SetVisibility(true);
                femurRightNode->SetVisibility(false);
                femurLeftNode->SetVisibility(false);
                mGroupBoxGadget[0]->setVisible(true);
                mGroupBoxGadget[0]->setMode(GroupBoxGadget::CUP);
                mGroupBoxGadget[1]->setVisible(true);
                mGroupBoxGadget[1]->setMode(GroupBoxGadget::CUP);
                mGroupBoxGadget[2]->setVisible(true);
                mGroupBoxGadget[2]->setMode(GroupBoxGadget::CUP);
                mCupParameterGadget[3]->setVisible(true);
                mImplantAssessmentGadget[3]->setVisible(true);
            }
            break;
            case MODE_STEM_PLAN: {
                femoralHeadCor->SetVisibility(true);
                femoralHead->SetVisibility(true);
                femoralStem->SetVisibility(true);
                nativeCorNode->SetVisibility(true);
                femurRightNode->SetVisibility(false);
                femurLeftNode->SetVisibility(false);
                mGroupBoxGadget[0]->setVisible(true);
                mGroupBoxGadget[0]->setMode(GroupBoxGadget::STEM);
                mGroupBoxGadget[1]->setVisible(true);
                mGroupBoxGadget[1]->setMode(GroupBoxGadget::STEM);
                mGroupBoxGadget[2]->setVisible(true);
                mGroupBoxGadget[2]->setMode(GroupBoxGadget::STEM);
                mStemParameterGadget[3]->setVisible(true);
                mImplantAssessmentGadget[3]->setVisible(true);
            }
            break;
            case MODE_REDUCED: {
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
                mImplantAssessmentGadget[3]->setVisible(true);
            }
            break;
            default:  // MODE_DEFAULT
            {
                // acetabularLiner->SetVisibility(false);
                // acetabularShell->SetVisibility(false);
                // femoralHead->SetVisibility(false);
                // femoralStem->SetVisibility(false);
                if (mView != VIEWS::VIEW_REAMING && mView != VIEWS::VIEW_X_RAY)
                {
                    femurRightNode->SetVisibility(true);
                    femurLeftNode->SetVisibility(true);
                }
                mImplantAssessmentGadget[3]->setVisible(true);
            }
            break;
        }
    }
    ResetCrosshair();
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}
