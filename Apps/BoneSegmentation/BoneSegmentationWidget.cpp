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
#include <mitkIOUtil.h>
#include <mitkITKImageImport.h>
#include <mitkImageToItk.h>
#include <mitkNodePredicateNot.h>
#include <mitkNodePredicateProperty.h>
#include <mitkRenderingManager.h>
#include <mitkSceneIO.h>
#include <usModuleRegistry.h>

// itk
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryFillholeImageFilter.h>
#include <itkBinaryMorphologicalOpeningImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkExtractImageFilter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>
#include <itkOtsuThresholdImageFilter.h>
#include <itkRelabelComponentImageFilter.h>
#include <itkThresholdImageFilter.h>
#include <itkTileImageFilter.h>
#include <itkUnsharpMaskImageFilter.h>

// qt
#include <QFileDialog>
#include <QMessageBox>

using ImageType = itk::Image<short, 3>;
using UnsharpMaskImageFilterType =
    itk::UnsharpMaskImageFilter<ImageType, ImageType>;
using OtsuMultipleThresholdsFilterType =
    itk::OtsuMultipleThresholdsImageFilter<ImageType, ImageType>;
using BinaryThresholdFilterType =
    itk::BinaryThresholdImageFilter<ImageType, ImageType>;
using SliceType =
    itk::Image<ImageType::PixelType, ImageType::ImageDimension - 1>;
using TileImageFilterType = itk::TileImageFilter<SliceType, ImageType>;
using ConnectedComponentFilterType =
    itk::ConnectedComponentImageFilter<ImageType, ImageType>;
using RelabelComponentFilterType =
    itk::RelabelComponentImageFilter<ImageType, ImageType>;
using ThresholdImageFilterType = itk::ThresholdImageFilter<ImageType>;
class BoneSegmentationWidgetPrivate
{
public:
  explicit BoneSegmentationWidgetPrivate(BoneSegmentationWidget* q) : q_ptr(q)
  {
  }
  Q_DECLARE_PUBLIC(BoneSegmentationWidget)
  BoneSegmentationWidget* q_ptr;
  // OtsuMultipleThresholdsFilterType::Pointer otsuMultipleThresholds =
  //     OtsuMultipleThresholdsFilterType::New();
  // BinaryThresholdFilterType::Pointer binaryThreshold =
  //     BinaryThresholdFilterType::New();
  // TileImageFilterType::Pointer tileImage = TileImageFilterType::New();
  // ConnectedComponentFilterType::Pointer connectedComponent =
  //     ConnectedComponentFilterType::New();
  // RelabelComponentFilterType::Pointer relabelComponent =
  //     RelabelComponentFilterType::New();
  // ThresholdImageFilterType::Pointer threshold =
  // ThresholdImageFilterType::New();
};

BoneSegmentationWidget::BoneSegmentationWidget(QWidget* parent) :
    QWidget(parent),
    mUi(std::make_unique<Ui::BoneSegmentationWidget>()),
    d_ptr(std::make_unique<BoneSegmentationWidgetPrivate>(this))
{
  mUi->setupUi(this);
  mitk::RegisterBoundingShapeObjectFactory();
}

BoneSegmentationWidget::~BoneSegmentationWidget() = default;

void BoneSegmentationWidget::on_toolButtonShowVoi_toggled(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::DataNode::Pointer voiNode = ds->GetNamedNode("voi");
  if (voiNode == nullptr)
  {
    auto* image = ds->GetNamedObject<mitk::Image>("image");

    auto voi = mitk::GeometryData::New();
    voi->SetGeometry(image->GetGeometry()->Clone());
    voiNode = mitk::DataNode::New();
    voiNode->SetName("voi");
    voiNode->SetData(voi);
    ds->Add(voiNode);

    auto boundingShapeInteractor = mitk::BoundingShapeInteractor::New();
    boundingShapeInteractor->LoadStateMachine(
        "BoundingShapeInteraction.xml",
        us::ModuleRegistry::GetModule("MitkBoundingShape"));
    boundingShapeInteractor->SetEventConfig(
        "BoundingShapeMouseConfig.xml",
        us::ModuleRegistry::GetModule("MitkBoundingShape"));
    boundingShapeInteractor->SetDataNode(voiNode);
  }
  voiNode->SetVisibility(checked);

  auto* imageNode = ds->GetNamedNode("image");
  imageNode->SetVisibility(checked);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void BoneSegmentationWidget::on_toolButtonSetVoi_toggled(bool checked)
{
  Q_D(BoneSegmentationWidget);
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::DataNode::Pointer imageCroppedNode = ds->GetNamedNode("image_cropped");
  if (imageCroppedNode == nullptr)
  {
    imageCroppedNode = mitk::DataNode::New();
    imageCroppedNode->SetName("image_cropped");
    ds->Add(imageCroppedNode);
  }

  auto* voi = ds->GetNamedObject<mitk::GeometryData>("voi");
  if (checked && voi != nullptr)
  {
    auto* image = ds->GetNamedObject<mitk::Image>("image");
    auto cropper = mitk::BoundingShapeCropper::New();
    cropper->SetInput(image);
    cropper->SetGeometry(voi);
    cropper->SetUseWholeInputRegion(false);
    cropper->Update();

    auto* imageCropped = cropper->GetOutput();
    imageCroppedNode->SetData(imageCropped);
  }

  imageCroppedNode->SetVisibility(checked);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void BoneSegmentationWidget::on_toolButtonUnsharpMask_toggled(bool checked)
{
  Q_D(BoneSegmentationWidget);
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::DataNode::Pointer imageUnsharpMaskNode =
      ds->GetNamedNode("image_unsharp_mask");
  if (imageUnsharpMaskNode == nullptr)
  {
    imageUnsharpMaskNode = mitk::DataNode::New();
    imageUnsharpMaskNode->SetName("image_unsharp_mask");
    ds->Add(imageUnsharpMaskNode);
  }

  auto* imageCropped = ds->GetNamedObject<mitk::Image>("image_cropped");
  if (checked && imageCropped != nullptr)
  {
    auto imageCroppedItk =
        mitk::ImageToItkImage<ImageType::PixelType, ImageType::ImageDimension>(
            imageCropped);
    auto spacing = imageCroppedItk->GetSpacing();

    auto unsharpMask = UnsharpMaskImageFilterType::New();
    unsharpMask->SetInput(imageCroppedItk);
    unsharpMask->SetAmount(1);
    unsharpMask->SetSigmas(spacing);
    unsharpMask->Update();

    auto imageUnsharpMask = mitk::GrabItkImageMemory(
        unsharpMask->GetOutput(), nullptr, imageCropped->GetGeometry());

    imageUnsharpMaskNode->SetData(imageUnsharpMask);
  }

  imageUnsharpMaskNode->SetVisibility(checked);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void BoneSegmentationWidget::on_toolButtonBodyMask_toggled(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::DataNode::Pointer imageBodyMaskNode =
      ds->GetNamedNode("image_body_mask");
  if (imageBodyMaskNode == nullptr)
  {
    imageBodyMaskNode = mitk::DataNode::New();
    imageBodyMaskNode->SetName("image_body_mask");
    ds->Add(imageBodyMaskNode);
  }

  auto* imageUnsharpMask =
      ds->GetNamedObject<mitk::Image>("image_unsharp_mask");
  if (checked && imageUnsharpMask != nullptr)
  {
    auto imageUnsharpMaskItk =
        mitk::ImageToItkImage<ImageType::PixelType, ImageType::ImageDimension>(
            imageUnsharpMask);
    auto otsuMultipleThresholds = OtsuMultipleThresholdsFilterType::New();
    otsuMultipleThresholds->SetInput(imageUnsharpMaskItk);
    otsuMultipleThresholds->SetNumberOfThresholds(2);
    otsuMultipleThresholds->Update();

    auto binaryThreshold = BinaryThresholdFilterType::New();
    binaryThreshold->SetInput(otsuMultipleThresholds->GetOutput());
    binaryThreshold->SetLowerThreshold(2);
    binaryThreshold->SetUpperThreshold(2);
    binaryThreshold->SetInsideValue(1);
    binaryThreshold->SetOutsideValue(0);
    binaryThreshold->Update();
    auto imageBodyMask = mitk::GrabItkImageMemory(
        binaryThreshold->GetOutput(), nullptr, imageUnsharpMask->GetGeometry());
    imageBodyMaskNode->SetData(imageBodyMask);
  }
  imageBodyMaskNode->SetVisibility(checked);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void BoneSegmentationWidget::on_toolButtonOtsuThresholdSliceBySlice_toggled(
    bool checked)
{
  Q_D(BoneSegmentationWidget);
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::DataNode::Pointer segmentationNode = ds->GetNamedNode("segmentation");
  if (segmentationNode == nullptr)
  {
    segmentationNode = mitk::DataNode::New();
    segmentationNode->SetName("segmentation");
    ds->Add(segmentationNode);
  }

  auto* imageUnsharpMask =
      ds->GetNamedObject<mitk::Image>("image_unsharp_mask");
  auto imageUnsharpMaskItk =
      mitk::ImageToItkImage<ImageType::PixelType, ImageType::ImageDimension>(
          imageUnsharpMask);
  if (checked && imageUnsharpMask != nullptr)
  {
    auto otsuMultipleThresholds = OtsuMultipleThresholdsFilterType::New();
    otsuMultipleThresholds->SetInput(imageUnsharpMaskItk);
    otsuMultipleThresholds->SetNumberOfThresholds(2);
    otsuMultipleThresholds->Update();

    auto binaryThreshold = BinaryThresholdFilterType::New();
    binaryThreshold->SetInput(otsuMultipleThresholds->GetOutput());
    binaryThreshold->SetLowerThreshold(2);
    binaryThreshold->SetUpperThreshold(2);
    binaryThreshold->SetInsideValue(1);
    binaryThreshold->SetOutsideValue(0);
    binaryThreshold->Update();

    auto region = imageUnsharpMaskItk->GetBufferedRegion();

    auto tileImage = TileImageFilterType::New();
    for (auto z = 0; z < region.GetSize()[2]; ++z)
    {
      mUi->progressBarOtsuThresholdSliceBySlice->setValue((z + 1) * 100 /
                                                          region.GetSize()[2]);

      ImageType::RegionType extractionRegion;
      auto index = region.GetIndex();
      index[2] = z;
      auto size = region.GetSize();
      size[2] = 0;
      extractionRegion.SetIndex(index);
      extractionRegion.SetSize(size);

      using ExtractImageFilter = itk::ExtractImageFilter<ImageType, SliceType>;
      auto extractImageFilterInput = ExtractImageFilter::New();
      extractImageFilterInput->SetInput(imageUnsharpMaskItk);
      extractImageFilterInput->SetDirectionCollapseToSubmatrix();
      extractImageFilterInput->SetExtractionRegion(extractionRegion);
      extractImageFilterInput->Update();

      auto extractImageFilterMask = ExtractImageFilter::New();
      extractImageFilterMask->SetInput(binaryThreshold->GetOutput());
      extractImageFilterMask->SetDirectionCollapseToSubmatrix();
      extractImageFilterMask->SetExtractionRegion(extractionRegion);
      extractImageFilterMask->Update();

      using OtsuThresholdImageFilterType =
          itk::OtsuThresholdImageFilter<SliceType, SliceType, SliceType>;
      auto otsuThresholdImageFilter = OtsuThresholdImageFilterType::New();
      otsuThresholdImageFilter->SetInput(extractImageFilterInput->GetOutput());
      otsuThresholdImageFilter->SetMaskImage(
          extractImageFilterMask->GetOutput());
      otsuThresholdImageFilter->SetMaskValue(1);
      otsuThresholdImageFilter->MaskOutputOn();
      otsuThresholdImageFilter->SetInsideValue(0);
      otsuThresholdImageFilter->SetOutsideValue(1);
      otsuThresholdImageFilter->Update();

      // using VotingBinaryHoleFillingImageFilter =
      //     itk::VotingBinaryHoleFillingImageFilter<SliceType, SliceType>;
      // auto votingBinaryHoleFillingImageFilter =
      //     VotingBinaryHoleFillingImageFilter::New();
      // votingBinaryHoleFillingImageFilter->SetInput(
      //     otsuThresholdImageFilter->GetOutput());
      // votingBinaryHoleFillingImageFilter->SetForegroundValue(1);
      // votingBinaryHoleFillingImageFilter->Update();

      using BinaryFillHoleImageFilter =
          itk::BinaryFillholeImageFilter<SliceType>;
      auto binaryFillHoleImageFilter = BinaryFillHoleImageFilter::New();
      binaryFillHoleImageFilter->SetInput(
          otsuThresholdImageFilter->GetOutput());
      binaryFillHoleImageFilter->SetForegroundValue(1);
      binaryFillHoleImageFilter->Update();

      if (mUi->checkBoxBinaryMorphologicalOpening->isChecked())
      {
        using BinaryBallStructuringElement =
            itk::BinaryBallStructuringElement<SliceType::PixelType,
                                              SliceType::ImageDimension>;
        BinaryBallStructuringElement structuringElement;
        structuringElement.SetRadius(
            mUi->spinBoxStructuringElementRadius->value());
        structuringElement.CreateStructuringElement();

        using BinaryMorphologicalOpeningImageFilter =
            itk::BinaryMorphologicalOpeningImageFilter<
                SliceType,
                SliceType,
                BinaryBallStructuringElement>;
        auto binaryMorphologicalOpeningImageFilter =
            BinaryMorphologicalOpeningImageFilter::New();
        binaryMorphologicalOpeningImageFilter->SetInput(
            binaryFillHoleImageFilter->GetOutput());
        binaryMorphologicalOpeningImageFilter->SetKernel(structuringElement);
        binaryMorphologicalOpeningImageFilter->SetBackgroundValue(0);
        binaryMorphologicalOpeningImageFilter->SetForegroundValue(1);
        binaryMorphologicalOpeningImageFilter->Update();
        tileImage->SetInput(z,
                            binaryMorphologicalOpeningImageFilter->GetOutput());
      }
      else
      {
        tileImage->SetInput(z, binaryFillHoleImageFilter->GetOutput());
      }
    }
    TileImageFilterType::LayoutArrayType layout;
    layout[0] = 1;
    layout[1] = 1;
    layout[2] = 0;
    tileImage->SetLayout(layout);
    tileImage->Update();

    auto connectedComponent = ConnectedComponentFilterType::New();
    connectedComponent->SetInput(tileImage->GetOutput());
    connectedComponent->Update();

    auto relabelComponent = RelabelComponentFilterType::New();
    relabelComponent->SetInput(connectedComponent->GetOutput());
    relabelComponent->Update();

    auto threshold = ThresholdImageFilterType::New();
    threshold->SetInput(relabelComponent->GetOutput());
    threshold->SetLower(mUi->spinBoxThresholdLower->value());
    threshold->SetUpper(mUi->spinBoxThresholdUpper->value());
    threshold->SetOutsideValue(0);
    threshold->Update();

    auto* image = ds->GetNamedObject<mitk::Image>("image");

    auto segmentation = mitk::GrabItkImageMemory(
        threshold->GetOutput(), nullptr, imageUnsharpMask->GetGeometry());
    segmentationNode->SetData(segmentation);
  }

  segmentationNode->SetVisibility(checked);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void BoneSegmentationWidget::on_toolButtonSave_clicked(bool checked)
{
  Q_UNUSED(checked);

  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save File"), "", tr("MITK scene files (*.mitk)"));
  if (fileName.isEmpty())
  {
    return;
  }
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto isNotHelperObject =
      mitk::NodePredicateNot::New(mitk::NodePredicateProperty::New(
          "helper object", mitk::BoolProperty::New(true)));
  auto nodesToBeSaved = ds->GetSubset(isNotHelperObject);

  auto sceneIO = mitk::SceneIO::New();
  auto success = sceneIO->SaveScene(nodesToBeSaved, ds, fileName.toStdString());
  if (!success)
  {
    QMessageBox::warning(this, "Save Error", "Could not save scene.");
  }
}

void BoneSegmentationWidget::on_toolButtonSaveSegmentation_clicked(bool checked)
{
  Q_UNUSED(checked);

  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save File"), "", tr("NIFTI files (*.nii.gz)"));
  if (fileName.isEmpty())
  {
    return;
  }
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  auto segmentation = ds->GetNamedObject<mitk::Image>("segmentation");
  if (segmentation == nullptr)
  {
    QMessageBox::warning(this, "Save Error", "No segmentation found.");
    return;
  }
  try
  {
    mitk::IOUtil::Save(segmentation, fileName.toStdString());
  }
  catch (const mitk::Exception& e)
  {
    QMessageBox::warning(this, "Save Error", e.what());
  }
}