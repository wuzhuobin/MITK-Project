/**
 * @file BoneSegmentationWidget.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-31
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "BoneSegmentationWidget.h"

#include "ui_BoneSegmentationWidget.h"

// mitk
#include <mitkBoundingShapeCropper.h>
#include <mitkBoundingShapeInteractor.h>
#include <mitkBoundingShapeObjectFactory.h>
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkGeometryData.h>
#include <mitkRenderingManager.h>
#include <usModuleRegistry.h>

BoneSegmentationWidget::BoneSegmentationWidget(QWidget* parent) :
    QWidget(parent), mUi(std::make_unique<Ui::BoneSegmentationWidget>())
{
  mUi->setupUi(this);
}

BoneSegmentationWidget::~BoneSegmentationWidget() = default;

void BoneSegmentationWidget::on_toolButtonShowVoi_toggled(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  mitk::DataNode::Pointer voiNode = ds->GetNamedNode("voi");
  if (voiNode != nullptr)
  {
    voiNode->SetVisibility(checked);
    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
    return;
  }

  if (checked)
  {
    auto geoData = mitk::GeometryData::New();
    voiNode = mitk::DataNode::New();
    voiNode->SetName("voi");
    voiNode->SetData(geoData);

    ds->Add(voiNode);

    mitk::RegisterBoundingShapeObjectFactory();
    auto boundingShapeInteractor = mitk::BoundingShapeInteractor::New();
    boundingShapeInteractor->LoadStateMachine(
        "BoundingShapeInteraction.xml",
        us::ModuleRegistry::GetModule("MitkBoundingShape"));
    boundingShapeInteractor->SetEventConfig(
        "BoundingShapeMouseConfig.xml",
        us::ModuleRegistry::GetModule("MitkBoundingShape"));

    boundingShapeInteractor->SetDataNode(voiNode);
  }
}
// int main(int argc, char* argv[])
// {
//   auto imageFileName = "../../../Data/CTA 1.0 CE.nrrd";
//   auto image = mitk::IOUtil::Load<mitk::Image>(imageFileName);

//   auto itkImage =
//       mitk::ImageToItkImage<ImageType::PixelType, ImageType::ImageDimension>(
//           image);

//   auto spacing = itkImage->GetSpacing();

//   using UnsharpMaskImageFilterType =
//       itk::UnsharpMaskImageFilter<ImageType, ImageType>;
//   auto unsharpMaskImageFilter = UnsharpMaskImageFilterType::New();
//   unsharpMaskImageFilter->SetAmount(1);
//   unsharpMaskImageFilter->SetInput(itkImage);
//   unsharpMaskImageFilter->SetSigmas(spacing);
//   unsharpMaskImageFilter->Update();

//   auto otsuThresholdImage = ImageType::New();
//   otsuThresholdSliceBySlice(unsharpMaskImageFilter->GetOutput(),
//                             otsuThresholdImage);
//   MITK_INFO << otsuThresholdImage;

//   auto otsuThresholdedImage = mitk::Image::New();
//   otsuThresholdedImage->InitializeByItk(otsuThresholdImage.GetPointer());
//   otsuThresholdedImage->SetVolume(otsuThresholdImage->GetBufferPointer());
//   otsuThresholdedImage->SetGeometry(image->GetGeometry());

//   mitk::IOUtil::Save(otsuThresholdedImage,
//                      "../../../Data/otsuThresholded.nrrd");

//   using ConnectedComponentFilterType =
//       itk::ConnectedComponentImageFilter<ImageType, ImageType>;
//   auto connectedComponentFilter = ConnectedComponentFilterType::New();
//   connectedComponentFilter->SetInput(otsuThresholdImage);
//   connectedComponentFilter->Update();

//   auto connectedComponentImage = mitk::Image::New();
//   connectedComponentImage->InitializeByItk(
//       connectedComponentFilter->GetOutput());
//   connectedComponentImage->SetVolume(
//       connectedComponentFilter->GetOutput()->GetBufferPointer());
//   connectedComponentImage->SetGeometry(image->GetGeometry());
//   mitk::IOUtil::Save(connectedComponentImage,
//                      "../../../Data/connectedComponentImage.nrrd");

//   using RelabelComponentFilterType =
//       itk::RelabelComponentImageFilter<ImageType, ImageType>;
//   auto relabelComponentFilter = RelabelComponentFilterType::New();
//   relabelComponentFilter->SetInput(connectedComponentFilter->GetOutput());
//   relabelComponentFilter->Update();
//   using ThresholdImageFilterType = itk::ThresholdImageFilter<ImageType>;
//   auto thresholdImageFilter = ThresholdImageFilterType::New();
//   thresholdImageFilter->SetInput(relabelComponentFilter->GetOutput());
//   thresholdImageFilter->SetLower(1);
//   thresholdImageFilter->SetUpper(6);
//   thresholdImageFilter->SetOutsideValue(0);
//   thresholdImageFilter->Update();

//   auto thresholdedImage = mitk::Image::New();
//   thresholdedImage->InitializeByItk(thresholdImageFilter->GetOutput());
//   thresholdedImage->SetVolume(
//       thresholdImageFilter->GetOutput()->GetBufferPointer());
//   thresholdedImage->SetGeometry(image->GetGeometry());
//   mitk::IOUtil::Save(thresholdedImage,
//   "../../../Data/thresholdedImage.nrrd");

//   return 0;
// }