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
#include <mitkImageToSurfaceFilter.h>
#include <mitkLabelSetImage.h>
#include <mitkNodePredicateNot.h>
#include <mitkNodePredicateProperty.h>
#include <mitkRenderingManager.h>
#include <mitkRenderingModeProperty.h>
#include <mitkSceneIO.h>
#include <usModuleRegistry.h>

// itk
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryFillholeImageFilter.h>
#include <itkBinaryMorphologicalClosingImageFilter.h>
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
#include <itkVotingBinaryHoleFillingImageFilter.h>
#include <itkVotingBinaryIterativeHoleFillingImageFilter.h>

// qt
#include <QFileDialog>
#include <QMessageBox>

using ImageType = itk::Image<short, 3>;
using LabelSetImageType = itk::Image<mitk::LabelSetImage::PixelType, 3>;
class BoneSegmentationWidgetPrivate
{
public:
  explicit BoneSegmentationWidgetPrivate(BoneSegmentationWidget* q) : q_ptr(q)
  {
  }
  Q_DECLARE_PUBLIC(BoneSegmentationWidget)
  BoneSegmentationWidget* q_ptr;
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

void BoneSegmentationWidget::showVoi(bool checked)
{
  mUi->toolButtonShowVoi->setChecked(checked);
}

void BoneSegmentationWidget::setVoi(bool checked)
{
  mUi->toolButtonSetVoi->setChecked(checked);
}

void BoneSegmentationWidget::doSegmentation(bool checked)
{
  on_toolButtonUnsharpMask_toggled(checked);
  on_toolButtonBodyMask_toggled(checked);
  on_toolButtonOtsuThresholdSliceBySlice_toggled(checked);
  mUi->toolButtonThreshold->setChecked(checked);
  if (checked)
  {
    on_toolButtonUnsharpMask_toggled(!checked);
    on_toolButtonBodyMask_toggled(!checked);
    on_toolButtonOtsuThresholdSliceBySlice_toggled(!checked);
  }
}

void BoneSegmentationWidget::imageToSurface(bool checked)
{
  mUi->toolButtonImageToSurface->setChecked(checked);
}

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

    using UnsharpMaskImageFilterType =
        itk::UnsharpMaskImageFilter<ImageType, ImageType>;
    auto unsharpMask = UnsharpMaskImageFilterType::New();
    unsharpMask->SetInput(imageCroppedItk);
    unsharpMask->SetAmount(mUi->doubleSpinBoxUnsharpMaskAmount->value());
    unsharpMask->SetSigmas(mUi->doubleSpinBoxUnsharpMaskSigma->value());
    unsharpMask->SetThreshold(mUi->doubleSpinBoxUnsharpMaskThreshold->value());
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
    using OtsuMultipleThresholdsFilterType =
        itk::OtsuMultipleThresholdsImageFilter<ImageType, ImageType>;
    auto otsuMultipleThresholds = OtsuMultipleThresholdsFilterType::New();
    otsuMultipleThresholds->SetInput(imageUnsharpMaskItk);
    otsuMultipleThresholds->SetNumberOfThresholds(
        mUi->spinBoxBodyMaskNumberOfThresholds->value());
    otsuMultipleThresholds->Update();

    using BinaryThresholdFilterType =
        itk::BinaryThresholdImageFilter<ImageType, ImageType>;
    auto binaryThreshold = BinaryThresholdFilterType::New();
    binaryThreshold->SetInput(otsuMultipleThresholds->GetOutput());
    binaryThreshold->SetLowerThreshold(mUi->spinBoxBodyMaskThreshold->value());
    binaryThreshold->SetUpperThreshold(
        mUi->spinBoxBodyMaskNumberOfThresholds->value());
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

  mitk::DataNode::Pointer imageConncetedComponentNode =
      ds->GetNamedNode("image_connceted_component");
  if (imageConncetedComponentNode == nullptr)
  {
    imageConncetedComponentNode = mitk::DataNode::New();
    imageConncetedComponentNode->SetName("image_connceted_component");
    ds->Add(imageConncetedComponentNode);
  }

  auto* imageUnsharpMask =
      ds->GetNamedObject<mitk::Image>("image_unsharp_mask");
  auto* imageBodyMask = ds->GetNamedObject<mitk::Image>("image_body_mask");
  if (checked && imageUnsharpMask != nullptr && imageBodyMask != nullptr)
  {
    auto imageUnsharpMaskItk =
        mitk::ImageToItkImage<ImageType::PixelType, ImageType::ImageDimension>(
            imageUnsharpMask);
    auto imageBodyMaskItk =
        mitk::ImageToItkImage<ImageType::PixelType, ImageType::ImageDimension>(
            imageBodyMask);

    auto region = imageUnsharpMaskItk->GetBufferedRegion();

    using SliceType =
        itk::Image<ImageType::PixelType, ImageType::ImageDimension - 1>;
    using TileImageFilterType = itk::TileImageFilter<SliceType, ImageType>;
    auto tileImage = TileImageFilterType::New();
    for (auto z = 0u; z < region.GetSize()[2]; ++z)
    {
      mUi->progressBarOtsuThresholdSliceBySlice->setValue((z + 1) * 100 /
                                                          region.GetSize()[2]);

      auto tmpImage = SliceType::New();

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
      extractImageFilterMask->SetInput(imageBodyMaskItk);
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

      tmpImage->Graft(otsuThresholdImageFilter->GetOutput());

      if (mUi->checkBoxBinaryMorphologicalClosing->isChecked())
      {
        using BinaryBallStructuringElement =
            itk::BinaryBallStructuringElement<SliceType::PixelType,
                                              SliceType::ImageDimension>;
        BinaryBallStructuringElement structuringElement;
        structuringElement.SetRadius(
            mUi->spinBoxBinaryMorpholocialClosingRadius->value());
        structuringElement.CreateStructuringElement();

        using BinaryMorphologicalClosingImageFilter =
            itk::BinaryMorphologicalClosingImageFilter<
                SliceType,
                SliceType,
                BinaryBallStructuringElement>;
        auto binaryMorphologicalClosingImageFilter =
            BinaryMorphologicalClosingImageFilter::New();
        binaryMorphologicalClosingImageFilter->SetInput(tmpImage);
        binaryMorphologicalClosingImageFilter->SetKernel(structuringElement);
        // binaryMorphologicalClosingImageFilter->SetBackgroundValue(0);
        binaryMorphologicalClosingImageFilter->SetForegroundValue(1);
        binaryMorphologicalClosingImageFilter->Update();
        tmpImage->Graft(binaryMorphologicalClosingImageFilter->GetOutput());
      }

      if (mUi->radioButtonBinaryFillHole->isChecked())
      {
        using BinaryFillHoleImageFilter =
            itk::BinaryFillholeImageFilter<SliceType>;
        auto binaryFillHoleImageFilter = BinaryFillHoleImageFilter::New();
        binaryFillHoleImageFilter->SetInput(tmpImage);
        binaryFillHoleImageFilter->SetForegroundValue(1);
        binaryFillHoleImageFilter->Update();
        tmpImage->Graft(binaryFillHoleImageFilter->GetOutput());
      }
      else if (mUi->radioButtonVotingBinaryHoleFilling->isChecked())
      {
        using VotingBinaryHoleFillingImageFilter =
            itk::VotingBinaryHoleFillingImageFilter<SliceType, SliceType>;
        auto votingBinaryHoleFillingImageFilter =
            VotingBinaryHoleFillingImageFilter::New();
        votingBinaryHoleFillingImageFilter->SetInput(
            otsuThresholdImageFilter->GetOutput());
        votingBinaryHoleFillingImageFilter->SetForegroundValue(1);
        votingBinaryHoleFillingImageFilter->SetRadius(
            {static_cast<unsigned int>(
                 mUi->spinBoxVotingBinaryHoleFillingRadius->value()),
             static_cast<unsigned int>(
                 mUi->spinBoxVotingBinaryHoleFillingRadius->value())});
        votingBinaryHoleFillingImageFilter->Update();
        tmpImage->Graft(votingBinaryHoleFillingImageFilter->GetOutput());
      }
      else if (mUi->radioButtonVotingBinaryIterativeHoleFilling->isChecked())
      {
        using VotingBinaryIterativeHoleFillingImageFilter =
            itk::VotingBinaryIterativeHoleFillingImageFilter<SliceType>;
        auto votingBinaryIterativeHoleFillingImageFilter =
            VotingBinaryIterativeHoleFillingImageFilter::New();
        votingBinaryIterativeHoleFillingImageFilter->SetInput(
            otsuThresholdImageFilter->GetOutput());
        votingBinaryIterativeHoleFillingImageFilter->SetForegroundValue(1);
        votingBinaryIterativeHoleFillingImageFilter->SetRadius(
            {static_cast<unsigned int>(
                 mUi->spinBoxVotingBinaryHoleFillingRadius->value()),
             static_cast<unsigned int>(
                 mUi->spinBoxVotingBinaryHoleFillingRadius->value())});
        votingBinaryIterativeHoleFillingImageFilter
            ->SetMaximumNumberOfIterations(
                mUi->spinBoxVotingBinaryHoleFillingIterative->value());
        votingBinaryIterativeHoleFillingImageFilter->Update();
        tmpImage->Graft(
            votingBinaryIterativeHoleFillingImageFilter->GetOutput());
      }

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
        binaryMorphologicalOpeningImageFilter->SetInput(tmpImage);
        binaryMorphologicalOpeningImageFilter->SetKernel(structuringElement);
        binaryMorphologicalOpeningImageFilter->SetBackgroundValue(0);
        binaryMorphologicalOpeningImageFilter->SetForegroundValue(1);
        binaryMorphologicalOpeningImageFilter->Update();
        tmpImage->Graft(binaryMorphologicalOpeningImageFilter->GetOutput());
      }
      tileImage->SetInput(z, tmpImage);
    }
    TileImageFilterType::LayoutArrayType layout;
    layout[0] = 1;
    layout[1] = 1;
    layout[2] = 0;
    tileImage->SetLayout(layout);
    tileImage->Update();

    using ConnectedComponentFilterType =
        itk::ConnectedComponentImageFilter<ImageType, ImageType>;
    auto connectedComponent = ConnectedComponentFilterType::New();
    connectedComponent->SetInput(tileImage->GetOutput());
    connectedComponent->Update();

    using RelabelComponentFilterType =
        itk::RelabelComponentImageFilter<ImageType, ImageType>;
    auto relabelComponent = RelabelComponentFilterType::New();
    relabelComponent->SetInput(connectedComponent->GetOutput());
    relabelComponent->Update();

    auto imageConnectedComponent = mitk::LabelSetImage::New();
    imageConnectedComponent->InitializeByLabeledImage(
        mitk::GrabItkImageMemory(relabelComponent->GetOutput(),
                                 nullptr,
                                 imageUnsharpMask->GetGeometry()));

    imageConncetedComponentNode->SetData(imageConnectedComponent);
    mUi->spinBoxNumberOfComponents->setValue(
        connectedComponent->GetObjectCount());
  }

  imageConncetedComponentNode->SetVisibility(checked);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void BoneSegmentationWidget::on_toolButtonThreshold_toggled(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::DataNode::Pointer imageSegmentationNode =
      ds->GetNamedNode("image_segmentation");
  if (imageSegmentationNode == nullptr)
  {
    imageSegmentationNode = mitk::DataNode::New();
    imageSegmentationNode->SetName("image_segmentation");
    ds->Add(imageSegmentationNode);
  }
  auto* imageConnectedComponent =
      ds->GetNamedObject<mitk::LabelSetImage>("image_connceted_component");
  if (checked && imageConnectedComponent != nullptr)
  {
    auto imageConnectedComponentItk =
        mitk::ImageToItkImage<LabelSetImageType::PixelType,
                              LabelSetImageType::ImageDimension>(
            imageConnectedComponent);

    using ThresholdImageFilterType =
        itk::ThresholdImageFilter<LabelSetImageType>;
    auto threshold = ThresholdImageFilterType::New();
    threshold->SetInput(imageConnectedComponentItk);
    threshold->SetLower(mUi->spinBoxSliceBySliceThresholdLower->value());
    threshold->SetUpper(mUi->spinBoxSliceBySliceThresholdUpper->value());
    threshold->SetOutsideValue(0);
    threshold->Update();

    auto segmentation =
        mitk::GrabItkImageMemory(threshold->GetOutput(),
                                 nullptr,
                                 imageConnectedComponent->GetGeometry());
    imageSegmentationNode->SetData(segmentation);
  }
  imageSegmentationNode->SetVisibility(checked);
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void BoneSegmentationWidget::on_toolButtonImageToSurface_toggled(bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  mitk::DataNode::Pointer surfaceSegmentationNode =
      ds->GetNamedNode("surface_segmentation");
  if (surfaceSegmentationNode == nullptr)
  {
    surfaceSegmentationNode = mitk::DataNode::New();
    surfaceSegmentationNode->SetName("surface_segmentation");
    ds->Add(surfaceSegmentationNode);
  }

  auto* imageSegmentation =
      ds->GetNamedObject<mitk::Image>("image_segmentation");
  if (checked && imageSegmentation != nullptr)
  {
    auto imageToSurface = mitk::ImageToSurfaceFilter::New();
    imageToSurface->SetInput(imageSegmentation);
    imageToSurface->SetSmooth(true);
    imageToSurface->Update();
    surfaceSegmentationNode->SetData(imageToSurface->GetOutput());
  }
  surfaceSegmentationNode->SetVisibility(checked);
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
  auto segmentation = ds->GetNamedObject<mitk::Image>("image_segmentation");
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

void BoneSegmentationWidget::on_toolButtonRedo_clicked(bool checked)
{
  Q_UNUSED(checked);
  mUi->buttonGroup->checkedButton()->toggled(false);
  mUi->buttonGroup->checkedButton()->toggled(true);
}