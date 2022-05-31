/**
 * @file main.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-05-23
 *
 * @copyright Copyright (c) 2022
 *
 */
// mitk
#include <mitkIOUtil.h>
#include <mitkImage.h>
#include <mitkImageToItk.h>

// itk
#include <itkBinaryThresholdImageFilter.h>
#include <itkConfidenceConnectedImageFilter.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkExtractImageFilter.h>
#include <itkMaskImageFilter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>
#include <itkOtsuThresholdImageFilter.h>
#include <itkRelabelComponentImageFilter.h>
#include <itkThresholdImageFilter.h>
#include <itkTileImageFilter.h>

using ImageType = itk::Image<short, 3>;

int main2(int argc, char* argv[])
{
  auto imageFileName = "../../../Data/CTA 1.0 CE.nrrd";
  auto image = mitk::IOUtil::Load<mitk::Image>(imageFileName);

  using ImageType = itk::Image<short, 3>;
  auto itkImage =
      mitk::ImageToItkImage<ImageType::PixelType, ImageType::ImageDimension>(
          image);

  using BinaryThresholdFilterType =
      itk::BinaryThresholdImageFilter<ImageType, ImageType>;
  auto binaryThresholdFilter = BinaryThresholdFilterType::New();
  binaryThresholdFilter->SetInput(itkImage);
  binaryThresholdFilter->SetLowerThreshold(150);
  binaryThresholdFilter->SetInsideValue(1);
  binaryThresholdFilter->SetOutsideValue(0);
  binaryThresholdFilter->Update();

  using ConnectedComponentFilterType =
      itk::ConnectedComponentImageFilter<ImageType, ImageType>;
  auto connectedComponentFilter = ConnectedComponentFilterType::New();
  connectedComponentFilter->SetInput(binaryThresholdFilter->GetOutput());
  connectedComponentFilter->Update();

  using RelabelComponentFilterType =
      itk::RelabelComponentImageFilter<ImageType, ImageType>;
  auto relabelComponentFilter = RelabelComponentFilterType::New();
  relabelComponentFilter->SetInput(connectedComponentFilter->GetOutput());
  relabelComponentFilter->Update();

  auto relabeledImage = mitk::Image::New();
  relabeledImage->InitializeByItk(relabelComponentFilter->GetOutput());
  relabeledImage->SetVolume(
      relabelComponentFilter->GetOutput()->GetBufferPointer());
  relabeledImage->SetGeometry(image->GetGeometry());

  using ThresholdImageFilterType = itk::ThresholdImageFilter<ImageType>;
  auto thresholdImageFilter = ThresholdImageFilterType::New();
  thresholdImageFilter->SetInput(relabelComponentFilter->GetOutput());
  thresholdImageFilter->SetLower(1);
  thresholdImageFilter->SetUpper(5);
  thresholdImageFilter->SetOutsideValue(0);
  thresholdImageFilter->Update();

  auto thresholdedImage = mitk::Image::New();
  thresholdedImage->InitializeByItk(thresholdImageFilter->GetOutput());
  thresholdedImage->SetVolume(
      thresholdImageFilter->GetOutput()->GetBufferPointer());
  thresholdedImage->SetGeometry(image->GetGeometry());

  mitk::IOUtil::Save(thresholdedImage, "../../../Data/thresholded.nrrd");

  return 0;
}

void otsuThreshold(ImageType* input, ImageType* output)
{
  using OtsuMultipleThresholdsFilterType =
      itk::OtsuMultipleThresholdsImageFilter<ImageType, ImageType>;
  auto otsuMultipleThresholdsFilter = OtsuMultipleThresholdsFilterType::New();
  otsuMultipleThresholdsFilter->SetInput(input);
  otsuMultipleThresholdsFilter->SetNumberOfThresholds(2);
  otsuMultipleThresholdsFilter->Update();

  using BinaryThresholdFilterType =
      itk::BinaryThresholdImageFilter<ImageType, ImageType>;
  auto binaryThresholdFilter = BinaryThresholdFilterType::New();
  binaryThresholdFilter->SetInput(otsuMultipleThresholdsFilter->GetOutput());
  binaryThresholdFilter->SetLowerThreshold(2);
  binaryThresholdFilter->SetUpperThreshold(2);
  binaryThresholdFilter->SetInsideValue(1);
  binaryThresholdFilter->SetOutsideValue(0);
  binaryThresholdFilter->Update();

  using OtsuThresholdImageFilterType =
      itk::OtsuThresholdImageFilter<ImageType, ImageType, ImageType>;
  auto otsuThresholdImageFilter = OtsuThresholdImageFilterType::New();
  otsuThresholdImageFilter->SetInput(input);
  otsuThresholdImageFilter->SetMaskImage(binaryThresholdFilter->GetOutput());
  otsuThresholdImageFilter->SetMaskValue(1);
  otsuThresholdImageFilter->MaskOutputOn();
  otsuThresholdImageFilter->SetInsideValue(0);
  otsuThresholdImageFilter->SetOutsideValue(1);
  otsuThresholdImageFilter->Update();
  output->Graft(otsuThresholdImageFilter->GetOutput());
}

void otsuThresholdSliceBySlice(ImageType* input, ImageType* output)
{
  using OtsuMultipleThresholdsFilterType =
      itk::OtsuMultipleThresholdsImageFilter<ImageType, ImageType>;
  auto otsuMultipleThresholdsFilter = OtsuMultipleThresholdsFilterType::New();
  otsuMultipleThresholdsFilter->SetInput(input);
  otsuMultipleThresholdsFilter->SetNumberOfThresholds(2);
  otsuMultipleThresholdsFilter->Update();

  using BinaryThresholdFilterType =
      itk::BinaryThresholdImageFilter<ImageType, ImageType>;
  auto binaryThresholdFilter = BinaryThresholdFilterType::New();
  binaryThresholdFilter->SetInput(otsuMultipleThresholdsFilter->GetOutput());
  binaryThresholdFilter->SetLowerThreshold(2);
  binaryThresholdFilter->SetUpperThreshold(2);
  binaryThresholdFilter->SetInsideValue(1);
  binaryThresholdFilter->SetOutsideValue(0);
  binaryThresholdFilter->Update();

  using SliceType = itk::Image<short, 2>;
  using TileImageFilterType = itk::TileImageFilter<SliceType, ImageType>;
  auto tileImageFilter = TileImageFilterType::New();

  auto region = input->GetBufferedRegion();
  for (auto z = 0; z < region.GetSize()[2]; ++z)
  {
    MITK_INFO << "Progress: " << z;
    auto index = region.GetIndex();
    index[2] = z;
    auto size = region.GetSize();
    size[2] = 0;
    ImageType::RegionType extractionRegion;
    extractionRegion.SetIndex(index);
    extractionRegion.SetSize(size);

    using ExtractImageFilter = itk::ExtractImageFilter<ImageType, SliceType>;
    auto extractImageFilterInput = ExtractImageFilter::New();
    extractImageFilterInput->SetInput(input);
    extractImageFilterInput->SetDirectionCollapseToSubmatrix();
    extractImageFilterInput->SetExtractionRegion(extractionRegion);
    extractImageFilterInput->Update();

    auto extractImageFilterMask = ExtractImageFilter::New();
    extractImageFilterMask->SetInput(binaryThresholdFilter->GetOutput());
    extractImageFilterMask->SetDirectionCollapseToSubmatrix();
    extractImageFilterMask->SetExtractionRegion(extractionRegion);
    extractImageFilterMask->Update();

    using OtsuThresholdImageFilterType =
        itk::OtsuThresholdImageFilter<SliceType, SliceType, SliceType>;
    auto otsuThresholdImageFilter = OtsuThresholdImageFilterType::New();
    otsuThresholdImageFilter->SetInput(extractImageFilterInput->GetOutput());
    otsuThresholdImageFilter->SetMaskImage(extractImageFilterMask->GetOutput());
    otsuThresholdImageFilter->SetMaskValue(1);
    otsuThresholdImageFilter->MaskOutputOn();
    otsuThresholdImageFilter->SetInsideValue(0);
    otsuThresholdImageFilter->SetOutsideValue(1);
    otsuThresholdImageFilter->Update();

    tileImageFilter->SetInput(z, otsuThresholdImageFilter->GetOutput());
  }
  TileImageFilterType::LayoutArrayType layout;
  layout[0] = 1;
  layout[1] = 1;
  layout[2] = 0;
  tileImageFilter->SetLayout(layout);
  tileImageFilter->Update();
  output->Graft(tileImageFilter->GetOutput());
}

void otsuMultipleThresholdSliceBySlice(ImageType* input, ImageType* output)
{
  using SliceType =
      itk::Image<ImageType::PixelType, ImageType::ImageDimension - 1>;

  using TileImageFilterType = itk::TileImageFilter<SliceType, ImageType>;
  auto tileImageFilter = TileImageFilterType::New();

  auto region = input->GetBufferedRegion();
  for (auto z = 0; z < region.GetSize()[2]; ++z)
  {
    MITK_INFO << "Progress: " << z;
    auto index = region.GetIndex();
    index[2] = z;
    auto size = region.GetSize();
    size[2] = 0;
    ImageType::RegionType extractionRegion;
    extractionRegion.SetIndex(index);
    extractionRegion.SetSize(size);

    using ExtractImageFilter = itk::ExtractImageFilter<ImageType, SliceType>;
    auto extractImageFilterInput = ExtractImageFilter::New();
    extractImageFilterInput->SetInput(input);
    extractImageFilterInput->SetDirectionCollapseToSubmatrix();
    extractImageFilterInput->SetExtractionRegion(extractionRegion);
    extractImageFilterInput->Update();

    using OtsuMultipleThresholdsFilterType =
        itk::OtsuMultipleThresholdsImageFilter<SliceType, SliceType>;
    auto otsuMultipleThresholdsFilter = OtsuMultipleThresholdsFilterType::New();
    otsuMultipleThresholdsFilter->SetInput(
        extractImageFilterInput->GetOutput());
    otsuMultipleThresholdsFilter->SetNumberOfThresholds(3);
    otsuMultipleThresholdsFilter->Update();

    tileImageFilter->SetInput(z, otsuMultipleThresholdsFilter->GetOutput());
  }
  TileImageFilterType::LayoutArrayType layout;
  layout[0] = 1;
  layout[1] = 1;
  layout[2] = 0;
  tileImageFilter->SetLayout(layout);
  tileImageFilter->Update();
  output->Graft(tileImageFilter->GetOutput());
}

int main(int argc, char* argv[])
{
  auto imageFileName = "../../../Data/CTA 1.0 CE.nrrd";
  auto image = mitk::IOUtil::Load<mitk::Image>(imageFileName);

  auto itkImage =
      mitk::ImageToItkImage<ImageType::PixelType, ImageType::ImageDimension>(
          image);

  auto otsuThresholdImage = ImageType::New();
  otsuThresholdSliceBySlice(itkImage, otsuThresholdImage);
  MITK_INFO << otsuThresholdImage;

  auto otsuThresholdedImage = mitk::Image::New();
  otsuThresholdedImage->InitializeByItk(otsuThresholdImage.GetPointer());
  otsuThresholdedImage->SetVolume(otsuThresholdImage->GetBufferPointer());
  otsuThresholdedImage->SetGeometry(image->GetGeometry());

  mitk::IOUtil::Save(otsuThresholdedImage,
                     "../../../Data/otsuThresholded.nrrd");

  using ConnectedComponentFilterType =
      itk::ConnectedComponentImageFilter<ImageType, ImageType>;
  auto connectedComponentFilter = ConnectedComponentFilterType::New();
  connectedComponentFilter->SetInput(otsuThresholdImage);
  connectedComponentFilter->Update();

  auto connectedComponentImage = mitk::Image::New();
  connectedComponentImage->InitializeByItk(
      connectedComponentFilter->GetOutput());
  connectedComponentImage->SetVolume(
      connectedComponentFilter->GetOutput()->GetBufferPointer());
  connectedComponentImage->SetGeometry(image->GetGeometry());
  mitk::IOUtil::Save(connectedComponentImage,
                     "../../../Data/connectedComponentImage.nrrd");

  using RelabelComponentFilterType =
      itk::RelabelComponentImageFilter<ImageType, ImageType>;
  auto relabelComponentFilter = RelabelComponentFilterType::New();
  relabelComponentFilter->SetInput(connectedComponentFilter->GetOutput());
  relabelComponentFilter->Update();
  using ThresholdImageFilterType = itk::ThresholdImageFilter<ImageType>;
  auto thresholdImageFilter = ThresholdImageFilterType::New();
  thresholdImageFilter->SetInput(relabelComponentFilter->GetOutput());
  thresholdImageFilter->SetLower(1);
  thresholdImageFilter->SetUpper(6);
  thresholdImageFilter->SetOutsideValue(0);
  thresholdImageFilter->Update();

  auto thresholdedImage = mitk::Image::New();
  thresholdedImage->InitializeByItk(thresholdImageFilter->GetOutput());
  thresholdedImage->SetVolume(
      thresholdImageFilter->GetOutput()->GetBufferPointer());
  thresholdedImage->SetGeometry(image->GetGeometry());
  mitk::IOUtil::Save(thresholdedImage, "../../../Data/thresholdedImage.nrrd");

  return 0;
}

int main3(int argc, char* argv[])
{
  auto imageFileName = "../../../Data/thresholdedImage.nrrd";
  auto image = mitk::IOUtil::Load<mitk::Image>(imageFileName);

  using ImageType = itk::Image<short, 3>;
  auto itkImage =
      mitk::ImageToItkImage<ImageType::PixelType, ImageType::ImageDimension>(
          image);

  using BinaryThresholdFilterType =
      itk::BinaryThresholdImageFilter<ImageType, ImageType>;
  auto binaryThresholdFilter = BinaryThresholdFilterType::New();
  binaryThresholdFilter->SetInput(itkImage);
  binaryThresholdFilter->SetLowerThreshold(1);
  binaryThresholdFilter->SetUpperThreshold(1);
  binaryThresholdFilter->SetInsideValue(1);
  binaryThresholdFilter->SetOutsideValue(0);
  binaryThresholdFilter->Update();

  auto binaryThresholdedImage = mitk::Image::New();
  binaryThresholdedImage->InitializeByItk(binaryThresholdFilter->GetOutput());
  binaryThresholdedImage->SetVolume(
      binaryThresholdFilter->GetOutput()->GetBufferPointer());
  binaryThresholdedImage->SetGeometry(image->GetGeometry());

  mitk::IOUtil::Save(binaryThresholdedImage,
                     "../../../Data/binaryThresholded.nrrd");

  //   using ConfidenceCOnnectedImageFilterType =
  //   itk::ConfidenceConnectedImageFilter<
  // 	  ImageType, ImageType>;
  //   auto confidenceConnectedImageFilter =
  //   ConfidenceCOnnectedImageFilterType::New();
  //   confidenceConnectedImageFilter->SetInput(itkImage);
  //   confidenceConnectedImageFilter->SetInitialNeighborhoodRadius(1);
  //   confidenceConnectedImageFilter->SetMultiplier(2);
  //   confidenceConnectedImageFilter->SetNumberOfIterations(5);
  //   confidenceConnectedImageFilter->SetReplaceValue(1);

  return 0;
}