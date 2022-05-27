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
#include <itkMaskImageFilter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>
#include <itkOtsuThresholdImageFilter.h>
#include <itkRelabelComponentImageFilter.h>
#include <itkThresholdImageFilter.h>

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

int main1(int argc, char* argv[])
{
  auto imageFileName = "../../../Data/CTA 1.0 CE.nrrd";
  auto image = mitk::IOUtil::Load<mitk::Image>(imageFileName);

  using ImageType = itk::Image<short, 3>;
  auto itkImage =
      mitk::ImageToItkImage<ImageType::PixelType, ImageType::ImageDimension>(
          image);

  using OtsuMultipleThresholdsFilterType =
      itk::OtsuMultipleThresholdsImageFilter<ImageType, ImageType>;
  auto otsuMultipleThresholdsFilter = OtsuMultipleThresholdsFilterType::New();
  otsuMultipleThresholdsFilter->SetInput(itkImage);
  otsuMultipleThresholdsFilter->SetNumberOfThresholds(2);
  otsuMultipleThresholdsFilter->Update();

  auto otsuMultipleThresholdsImage = mitk::Image::New();
  otsuMultipleThresholdsImage->InitializeByItk(
      otsuMultipleThresholdsFilter->GetOutput());
  otsuMultipleThresholdsImage->SetVolume(
      otsuMultipleThresholdsFilter->GetOutput()->GetBufferPointer());
  otsuMultipleThresholdsImage->SetGeometry(image->GetGeometry());

  mitk::IOUtil::Save(otsuMultipleThresholdsImage,
                     "../../../Data/otsuMultipleThresholdsImage.nrrd");

  using BinaryThresholdFilterType =
      itk::BinaryThresholdImageFilter<ImageType, ImageType>;
  auto binaryThresholdFilter = BinaryThresholdFilterType::New();
  binaryThresholdFilter->SetInput(otsuMultipleThresholdsFilter->GetOutput());
  binaryThresholdFilter->SetLowerThreshold(2);
  binaryThresholdFilter->SetUpperThreshold(2);
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

  //   using MaskImageFilter = itk::MaskImageFilter<ImageType, ImageType,
  //   ImageType>; auto maskImageFilter = MaskImageFilter::New();
  //   maskImageFilter->SetInput(itkImage);
  //   maskImageFilter->SetMaskImage(binaryThresholdFilter->GetOutput());
  //   maskImageFilter->SetOutsideValue(0);
  //   maskImageFilter->Update();

  //   auto maskedImage = mitk::Image::New();
  //   maskedImage->InitializeByItk(maskImageFilter->GetOutput());
  //   maskedImage->SetVolume(maskImageFilter->GetOutput()->GetBufferPointer());
  //   maskedImage->SetGeometry(image->GetGeometry());
  //   mitk::IOUtil::Save(maskedImage, "../../../Data/maskedImage.nrrd");

  using OtsuThresholdImageFilterType =
      itk::OtsuThresholdImageFilter<ImageType, ImageType, ImageType>;
  auto otsuThresholdImageFilter = OtsuThresholdImageFilterType::New();
  otsuThresholdImageFilter->SetInput(itkImage);
  otsuThresholdImageFilter->SetMaskImage(binaryThresholdFilter->GetOutput());
  otsuThresholdImageFilter->SetMaskValue(1);
  otsuThresholdImageFilter->MaskOutputOn();
  otsuThresholdImageFilter->SetInsideValue(0);
  otsuThresholdImageFilter->SetOutsideValue(1);
  otsuThresholdImageFilter->Update();

  //   otsuMultipleThresholdsFilter->SetInput(maskImageFilter->GetOutput());
  //   otsuMultipleThresholdsFilter->SetNumberOfThresholds(2);
  //   otsuMultipleThresholdsFilter->Update();

  //   otsuMultipleThresholdsImage->InitializeByItk(
  //       otsuMultipleThresholdsFilter->GetOutput());
  //   otsuMultipleThresholdsImage->SetVolume(
  //       otsuMultipleThresholdsFilter->GetOutput()->GetBufferPointer());
  //   otsuMultipleThresholdsImage->SetGeometry(image->GetGeometry());

  //   mitk::IOUtil::Save(otsuMultipleThresholdsImage,
  //                      "../../../Data/otsuMultipleThresholdsImage.nrrd");

  auto otsuThresholdedImage = mitk::Image::New();
  otsuThresholdedImage->InitializeByItk(otsuThresholdImageFilter->GetOutput());
  otsuThresholdedImage->SetVolume(
      otsuThresholdImageFilter->GetOutput()->GetBufferPointer());
  otsuThresholdedImage->SetGeometry(image->GetGeometry());

  mitk::IOUtil::Save(otsuThresholdedImage,
                     "../../../Data/otsuThresholded.nrrd");

  using ConnectedComponentFilterType =
      itk::ConnectedComponentImageFilter<ImageType, ImageType>;
  auto connectedComponentFilter = ConnectedComponentFilterType::New();
  connectedComponentFilter->SetInput(otsuThresholdImageFilter->GetOutput());
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

int main(int argc, char* argv[])
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