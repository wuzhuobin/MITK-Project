/**
 * @file SCBDentalPlanningPipeline.cxx
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "SCBDentalPlanningPipeline.h"

// qt
#include <QMessageBox>

// mitk
#include <mitkAffineBaseDataInteractor3D.h>
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkImage.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkSurface.h>
#include <usModuleRegistry.h>

SCBDentalPlanningPipeline::SCBDentalPlanningPipeline(QWidget* parent) :
    QWidget(parent)
{
  this->setupUi(this);
  this->widgetBoneSegmentation->setVisible(false);
}

void SCBDentalPlanningPipeline::on_pushButtonAdvancedOption_clicked()
{
  //   SCBDentalReconstructionOptionDialog dialog(this);
  //   dialog.setWindowFlag(Qt::FramelessWindowHint);
  //   dialog.exec();
}

void SCBDentalPlanningPipeline::on_pushButtonAutoReconstruction_clicked()
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  auto* nerveLeftNode = ds->GetNamedNode("nerve_left");
  nerveLeftNode->SetVisibility(true);
  auto* nerveRightNode = ds->GetNamedNode("nerve_right");
  nerveRightNode->SetVisibility(true);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void SCBDentalPlanningPipeline::on_buttonGroupTeethPosition_buttonToggled(
    QAbstractButton* button, bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  if (!checked)
  {
    if (QMessageBox::No ==
        QMessageBox::question(this,
                              tr("Delete Implant"),
                              tr("Are you sure to delete this implant?")))
    {
      this->buttonGroupTeethPosition->blockSignals(true);
      button->setChecked(true);
      this->buttonGroupTeethPosition->blockSignals(false);
      return;
    }
    auto* implantNode =
        ds->GetNamedNode((QString("implant_") + button->text()).toStdString());
    if (!implantNode)
    {
      return;
    }
    ds->Remove(implantNode);
  }
  else
  {
    auto* image = ds->GetNamedObject<mitk::Image>("image");

    auto center = image->GetGeometry()->GetCenter();
    mitk::Vector3D translate(center.GetVnlVector());

    auto* originImplant = ds->GetNamedObject<mitk::Surface>("implant");

    auto implant = originImplant->Clone();
    implant->GetGeometry()->Translate(translate);

    auto implantNode = mitk::DataNode::New();
    implantNode->SetName((QString("implant_") + button->text()).toStdString());
    implantNode->SetData(implant);
    implantNode->SetVisibility(true);
    implantNode->SetBoolProperty("pickable", true);
    ds->Add(implantNode);

    auto affineDataInteractor = mitk::AffineBaseDataInteractor3D::New();
    affineDataInteractor->LoadStateMachine(
        "AffineInteraction3D.xml",
        us::ModuleRegistry::GetModule("MitkDataTypesExt"));
    affineDataInteractor->SetEventConfig(
        "AffineMouseConfig.xml",
        us::ModuleRegistry::GetModule("MitkDataTypesExt"));
    affineDataInteractor->SetDataNode(implantNode);
  }

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void SCBDentalPlanningPipeline::
    on_toolButtonSurgicalGuidePlanningShowVoi_toggled(bool checked)
{
  widgetBoneSegmentation->showVoi(checked);
}

void SCBDentalPlanningPipeline::
    on_toolButtonSurgicalGuidePlanningSetVoi_toggled(bool checked)
{
  widgetBoneSegmentation->setVoi(checked);
}

void SCBDentalPlanningPipeline::
    on_toolButtonSurgicalGuidePlanningDoSegmentation_toggled(bool checked)
{
  widgetBoneSegmentation->doSegmentation(checked);

  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto* imageSegmentationNode = ds->GetNamedNode("image_segmentation");
  imageSegmentationNode->SetOpacity(0.5);
  auto* imageNode = ds->GetNamedNode("image");
  imageNode->SetVisibility(true);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void SCBDentalPlanningPipeline::
    on_toolButtonSurgicalGuidePlanningImageToSurface_toggled(bool checked)
{
  widgetBoneSegmentation->imageToSurface(checked);
}

void SCBDentalPlanningPipeline::
    on_toolButtonSurgicalGuidePlanningAdvance_toggled(bool checked)
{
  this->widgetBoneSegmentation->setVisible(checked);
}