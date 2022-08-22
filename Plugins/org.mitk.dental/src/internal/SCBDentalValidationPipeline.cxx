#include "SCBDentalValidationPipeline.h"

#include <math.h>

// #include "SCBDentalHub.h"
// #include "SCBScene.h"
// #include "SCBImage.h"
// #include "SCBImageSlice.h"
// #include "SCBVolume.h"
// #include "SCBDentalMainWindow.h"
// #include "SCBPlanarViewer.h"
// #include "SCBNonPlanarViewer.h"
// #include "SCBDentalFixture.h"

// itk
#include <itkBinaryThresholdImageFilter.h>
#include <itkCenteredTransformInitializer.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkLabelImageToShapeLabelMapFilter.h>
#include <itkLabelMap.h>
#include <itkLabelSelectionLabelMapFilter.h>
#include <itkMaskImageFilter.h>
#include <itkMatrix.h>
#include <itkMergeLabelMapFilter.h>
#include <itkOtsuMultipleThresholdsImageFilter.h>
#include <itkResampleImageFilter.h>
#include <itkShapeLabelObject.h>
#include <itkSubtractImageFilter.h>

// vtk
#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

// #include "ImageRegistration.h"

// qr
#include <QComboBox>

#define PI 3.14159265

SCBDentalValidationPipeline::SCBDentalValidationPipeline(QWidget* parent) :
    QWidget(parent)
{
  this->setupUi(this);

  this->manualRegFrame->hide();

  //// connect signal slots
  connect(this->manualRegPushButton,
          SIGNAL(clicked()),
          this,
          SLOT(manualRegistrationShow()));
  connect(this->rotateXSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(rotateXSliderValueChanged(int)));
  connect(this->rotateYSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(rotateYSliderValueChanged(int)));
  connect(this->rotateZSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(rotateZSliderValueChanged(int)));
  connect(this->translateXSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(translateXSliderValueChanged(int)));
  connect(this->translateYSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(translateYSliderValueChanged(int)));
  connect(this->translateZSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(translateZSliderValueChanged(int)));
  connect(this->rotateXSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(rotateXSpinBoxValueChanged(double)));
  connect(this->rotateYSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(rotateYSpinBoxValueChanged(double)));
  connect(this->rotateZSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(rotateZSpinBoxValueChanged(double)));
  connect(this->translateXSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(translateXSpinBoxValueChanged(double)));
  connect(this->translateYSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(translateYSpinBoxValueChanged(double)));
  connect(this->translateZSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(translateZSpinBoxValueChanged(double)));
  connect(this->resetPushButton,
          SIGNAL(clicked()),
          this,
          SLOT(resetRegistration()));
  connect(this->autoValidationPushButton,
          SIGNAL(clicked()),
          this,
          SLOT(autoValidationRun()));
  connect(this->manualValidationPushButton,
          SIGNAL(clicked()),
          this,
          SLOT(manualValidationRun()));
  //   connect(SCBScene::getCurrentScene(),
  //           SIGNAL(signalAddedData(QString)),
  //           this,
  //           SLOT(sceneDataAdded(QString)));

  // preOp postOp itk images
  //   m_preOpItkImage = ImageType::New();
  //   m_postOpItkImage = ImageType::New();
  //   m_postOpItkImageReg = ImageType::New();
  //   m_postOpIinitialUserMatrix = vtkMatrix4x4::New();
  //   m_postOpInititalItkRegTransform =
  //   ImageRegistration::TransformType::New();

  m_needleVol = 100;
}

SCBDentalValidationPipeline::~SCBDentalValidationPipeline()
{
  // m_postOpIinitialUserMatrix->Delete();

  this->clearTable();
}

void SCBDentalValidationPipeline::sceneDataAdded(QString name)
{
  Q_UNUSED(name);
  //   m_preOpImage =
  //   SCBScene::getCurrentScene()->getDataByAlias<SCBImage>("PreOp");
  //   m_postOpImage =
  //       SCBScene::getCurrentScene()->getDataByAlias<SCBImage>("PostOp");

  //   // std::cout << "scene data added" << std::endl;

  //   if (m_preOpImage == nullptr || m_postOpImage == nullptr)
  //     return;

  //   // std::cout << "registration initialization" << std::endl;

  //   m_preOpImage->getITKImageData<short>(m_preOpItkImage);
  //   m_postOpImage->getITKImageData<short>(m_postOpItkImage);

  //   // initial registration
  //   typedef
  //   itk::CenteredTransformInitializer<ImageRegistration::TransformType,
  //                                             ImageRegistration::ImageType,
  //                                             ImageRegistration::ImageType>
  //       InitializerType;  // initializer
  //   InitializerType::Pointer initializer = InitializerType::New();

  //   initializer->SetTransform(m_postOpInititalItkRegTransform);
  //   initializer->SetFixedImage(m_preOpItkImage);
  //   initializer->SetMovingImage(m_postOpItkImage);
  //   initializer->GeometryOn();
  //   initializer->InitializeTransform();

  //   // std::cout << "itk initial transformation" << std::endl;
  //   m_postOpInititalItkRegTransform->Print(std::cout);

  //   for (int i = 0; i < 3; i++)
  //   {
  //     for (int j = 0; j < 3; j++)
  //     {
  //       // m_postOpImage->getUserMatrix()->SetElement(i, j,
  //       // transform->GetMatrix()[i][j]);
  //       m_postOpImage->getUserMatrix()->SetElement(
  //           i,
  //           j,
  //           m_postOpInititalItkRegTransform->GetMatrix().GetInverse()[i][j]);
  //     }
  //   }

  //   m_postOpImage->getUserMatrix()->SetElement(
  //       0, 3, -m_postOpInititalItkRegTransform->GetTranslation()[0]);
  //   m_postOpImage->getUserMatrix()->SetElement(
  //       1, 3, -m_postOpInititalItkRegTransform->GetTranslation()[1]);
  //   m_postOpImage->getUserMatrix()->SetElement(
  //       2, 3, -m_postOpInititalItkRegTransform->GetTranslation()[2]);

  //   m_postOpIinitialUserMatrix->DeepCopy(m_postOpImage->getUserMatrix());

  //   // update ui 6 dof widget
  //   this->itkTransformToWidget(m_postOpInititalItkRegTransform);

  //   // std::cout << "user matrix after initialize" << std::endl;
  //   m_postOpImage->getUserMatrix()->Print(std::cout);

  //   for (int i = 0; i < 4; i++)
  //   {
  //     SCBDentalHub::self->mainWindow->getViewer(i)->Render();
  //   }
}

void SCBDentalValidationPipeline::clearTable()
{
  for (int i = 0; i < this->tableWidget->rowCount(); i++)
  {
    for (int j = 0; j < this->tableWidget->columnCount(); j++)
    {
      delete this->tableWidget->item(i, j);
    }
  }

  // clean up needle property collections
  while (!m_plannedNeedlePropertyCollections.empty())
  {
    delete m_plannedNeedlePropertyCollections.back();
    m_plannedNeedlePropertyCollections.pop_back();
  }

  while (!m_plantedNeedlePropertyCollections.empty())
  {
    delete m_plantedNeedlePropertyCollections.back();
    m_plantedNeedlePropertyCollections.pop_back();
  }
}

void SCBDentalValidationPipeline::manualRegistrationShow()
{
  if (this->manualRegFrame->isHidden())
    this->manualRegFrame->show();
  else
    this->manualRegFrame->hide();
}

void SCBDentalValidationPipeline::UpdateUserMatrixFromTransformWidget()
{
  //   SCBImage* imagePost =
  //       SCBScene::getCurrentScene()->getDataByAlias<SCBImage>("PostOp");
  //   if (imagePost == nullptr)
  //     return;

  //   itk::Image<short, 3>::Pointer postOpItkImage = itk::Image<short,
  //   3>::New(); imagePost->getITKImageData<short>(postOpItkImage);
  //   // postOpItkImage->Print(std::cout);

  //   double worldBounds[6];
  //   imagePost->getWorldBounds(worldBounds);

  //   vtkSmartPointer<vtkTransform> transform =
  //       vtkSmartPointer<vtkTransform>::New();
  //   transform->PostMultiply();
  //   transform->Translate(
  //       -(worldBounds[1] + worldBounds[0]) / 2 -
  //       this->translateXSpinBox->value(),
  //       -(worldBounds[3] + worldBounds[2]) / 2 -
  //       this->translateYSpinBox->value(),
  //       -(worldBounds[5] + worldBounds[4]) / 2 -
  //           this->translateZSpinBox->value());

  //   transform->RotateX(this->rotateXSpinBox->value());
  //   transform->RotateY(this->rotateYSpinBox->value());
  //   transform->RotateZ(this->rotateZSpinBox->value());
  //   transform->Translate(
  //       (worldBounds[1] + worldBounds[0]) / 2 +
  //       this->translateXSpinBox->value(), (worldBounds[3] + worldBounds[2]) /
  //       2 + this->translateYSpinBox->value(), (worldBounds[5] +
  //       worldBounds[4]) / 2 + this->translateZSpinBox->value());
  //   transform->Translate(-this->translateXSpinBox->value(),
  //                        -this->translateYSpinBox->value(),
  //                        -this->translateZSpinBox->value());

  //   imagePost->getUserMatrix()->DeepCopy(transform->GetMatrix());

  //   std::cout
  //       << "********************Widget To User Matrix Result****************"
  //       << std::endl;
  //   imagePost->getUserMatrix()->Print(std::cout);

  //   for (int i = 0; i < 4; i++)
  //   {
  //     SCBDentalHub::self->mainWindow->getViewer(i)->Render();
  //   }
}

void SCBDentalValidationPipeline::itkTransformToWidget(
    itk::VersorRigid3DTransform<double>::Pointer transform)
{
  // userMatrix->DeepCopy(m_postOpImage->getUserMatrix());

  // std::cout << "itk transform converting to 6 dof" << std::endl;
  // transform->Print(std::cout);

  double sixDof[6] = {0, 0, 0, 0, 0, 0};

  // Solving X1Y2Z3 of Tait-Bryan angles in
  // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
  if (transform->GetMatrix()[0][2] == 1 || transform->GetMatrix()[0][2] == -1)
  {
    // std::cerr << "singular set" << std::endl;
    sixDof[1] = asin(transform->GetMatrix()[0][2]);
    sixDof[0] = 0;  // set x rotation to be 0
    sixDof[2] =
        atan(transform->GetMatrix()[1][2] / transform->GetMatrix()[1][1]);
  }
  else
  {
    sixDof[1] = asin(transform->GetMatrix()[0][2]);
    sixDof[2] =
        atan(-transform->GetMatrix()[0][1] / transform->GetMatrix()[0][0]);
    sixDof[0] =
        atan(-transform->GetMatrix()[1][2] / transform->GetMatrix()[2][2]);
  }

  for (int i = 0; i < 3; i++)
  {
    sixDof[i] = sixDof[i] * 180.0 / PI;
  }

  // translation
  sixDof[3] = transform->GetTranslation()[0];
  sixDof[4] = transform->GetTranslation()[1];
  sixDof[5] = transform->GetTranslation()[2];

  // std::cout << "itk transform to 6 DoF result" << std::endl;
  // std::cout << sixDof[0] << ","
  //	<< sixDof[1] << ","
  //	<< sixDof[2] << ","
  //	<< sixDof[3] << ","
  //	<< sixDof[4] << ","
  //	<< sixDof[5] << std::endl;

  // angle direction correction for RAI system
  sixDof[0] = -sixDof[0];
  sixDof[1] = -sixDof[1];
  sixDof[2] = -sixDof[2];

  // disconnect spinbox before set value, necessary for update ui from user
  // matrix
  disconnect(this->rotateXSlider,
             SIGNAL(valueChanged(int)),
             this,
             SLOT(rotateXSliderValueChanged(int)));
  disconnect(this->rotateYSlider,
             SIGNAL(valueChanged(int)),
             this,
             SLOT(rotateYSliderValueChanged(int)));
  disconnect(this->rotateZSlider,
             SIGNAL(valueChanged(int)),
             this,
             SLOT(rotateZSliderValueChanged(int)));
  disconnect(this->translateXSlider,
             SIGNAL(valueChanged(int)),
             this,
             SLOT(translateXSliderValueChanged(int)));
  disconnect(this->translateYSlider,
             SIGNAL(valueChanged(int)),
             this,
             SLOT(translateYSliderValueChanged(int)));
  disconnect(this->translateZSlider,
             SIGNAL(valueChanged(int)),
             this,
             SLOT(translateZSliderValueChanged(int)));

  disconnect(this->rotateXSpinBox,
             SIGNAL(valueChanged(double)),
             this,
             SLOT(rotateXSpinBoxValueChanged(double)));
  disconnect(this->rotateYSpinBox,
             SIGNAL(valueChanged(double)),
             this,
             SLOT(rotateYSpinBoxValueChanged(double)));
  disconnect(this->rotateZSpinBox,
             SIGNAL(valueChanged(double)),
             this,
             SLOT(rotateZSpinBoxValueChanged(double)));
  disconnect(this->translateXSpinBox,
             SIGNAL(valueChanged(double)),
             this,
             SLOT(translateXSpinBoxValueChanged(double)));
  disconnect(this->translateYSpinBox,
             SIGNAL(valueChanged(double)),
             this,
             SLOT(translateYSpinBoxValueChanged(double)));
  disconnect(this->translateZSpinBox,
             SIGNAL(valueChanged(double)),
             this,
             SLOT(translateZSpinBoxValueChanged(double)));

  this->rotateXSlider->setValue(sixDof[0]);
  this->rotateYSlider->setValue(sixDof[1]);
  this->rotateZSlider->setValue(sixDof[2]);
  this->translateXSlider->setValue(sixDof[3]);
  this->translateYSlider->setValue(sixDof[4]);
  this->translateZSlider->setValue(sixDof[5]);

  this->rotateXSpinBox->setValue(sixDof[0]);
  this->rotateYSpinBox->setValue(sixDof[1]);
  this->rotateZSpinBox->setValue(sixDof[2]);
  this->translateXSpinBox->setValue(sixDof[3]);
  this->translateYSpinBox->setValue(sixDof[4]);
  this->translateZSpinBox->setValue(sixDof[5]);

  // reconnect spinbox before set value
  connect(this->rotateXSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(rotateXSliderValueChanged(int)));
  connect(this->rotateYSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(rotateYSliderValueChanged(int)));
  connect(this->rotateZSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(rotateZSliderValueChanged(int)));
  connect(this->translateXSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(translateXSliderValueChanged(int)));
  connect(this->translateYSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(translateYSliderValueChanged(int)));
  connect(this->translateZSlider,
          SIGNAL(valueChanged(int)),
          this,
          SLOT(translateZSliderValueChanged(int)));

  connect(this->rotateXSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(rotateXSpinBoxValueChanged(double)));
  connect(this->rotateYSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(rotateYSpinBoxValueChanged(double)));
  connect(this->rotateZSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(rotateZSpinBoxValueChanged(double)));
  connect(this->translateXSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(translateXSpinBoxValueChanged(double)));
  connect(this->translateYSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(translateYSpinBoxValueChanged(double)));
  connect(this->translateZSpinBox,
          SIGNAL(valueChanged(double)),
          this,
          SLOT(translateZSpinBoxValueChanged(double)));
}

void SCBDentalValidationPipeline::rotateXSliderValueChanged(int value)
{
  this->rotateXSpinBox->setValue(value);
}

void SCBDentalValidationPipeline::rotateYSliderValueChanged(int value)
{
  this->rotateYSpinBox->setValue(value);
}

void SCBDentalValidationPipeline::rotateZSliderValueChanged(int value)
{
  this->rotateZSpinBox->setValue(value);
}

void SCBDentalValidationPipeline::translateXSliderValueChanged(int value)
{
  this->translateXSpinBox->setValue(value);
}

void SCBDentalValidationPipeline::translateYSliderValueChanged(int value)
{
  this->translateYSpinBox->setValue(value);
}

void SCBDentalValidationPipeline::translateZSliderValueChanged(int value)
{
  this->translateZSpinBox->setValue(value);
}

void SCBDentalValidationPipeline::rotateXSpinBoxValueChanged(double value)
{
  UpdateUserMatrixFromTransformWidget();
  this->rotateXSlider->setValue(value);
  this->clearTable();
}

void SCBDentalValidationPipeline::rotateYSpinBoxValueChanged(double value)
{
  UpdateUserMatrixFromTransformWidget();
  this->rotateYSlider->setValue(value);
  this->clearTable();
}

void SCBDentalValidationPipeline::rotateZSpinBoxValueChanged(double value)
{
  UpdateUserMatrixFromTransformWidget();
  this->rotateZSlider->setValue(value);
  this->clearTable();
}

void SCBDentalValidationPipeline::translateXSpinBoxValueChanged(double value)
{
  UpdateUserMatrixFromTransformWidget();
  this->translateXSlider->setValue(value);
  this->clearTable();
}

void SCBDentalValidationPipeline::translateYSpinBoxValueChanged(double value)
{
  UpdateUserMatrixFromTransformWidget();
  this->translateYSlider->setValue(value);
  this->clearTable();
}

void SCBDentalValidationPipeline::translateZSpinBoxValueChanged(double value)
{
  UpdateUserMatrixFromTransformWidget();
  this->translateZSlider->setValue(value);
  this->clearTable();
}

void SCBDentalValidationPipeline::resetRegistration()
{
  //   m_postOpImage->getUserMatrix()->DeepCopy(m_postOpIinitialUserMatrix);

  //   // update ui 6 dof widget
  //   this->itkTransformToWidget(m_postOpInititalItkRegTransform);

  //   for (int i = 0; i < 4; i++)
  //   {
  //     SCBDentalHub::self->mainWindow->getViewer(i)->Render();
  //   }
}

void SCBDentalValidationPipeline::autoValidationRun()
{
  //   emit SCBDentalHub::self->signalLoading(true);
  //   this->clearTable();
  //   this->registration(true);
  //   this->validation();
  //   emit SCBDentalHub::self->signalLoading(false);
}

void SCBDentalValidationPipeline::manualValidationRun()
{
  //   emit SCBDentalHub::self->signalLoading(true);
  //   this->clearTable();
  //   this->registration(false);
  //   this->validation();
  //   emit SCBDentalHub::self->signalLoading(false);
}

void SCBDentalValidationPipeline::registration(bool autoReg)
{
  Q_UNUSED(autoReg);
  //   std::cout
  //       <<
  //       "**************************registration*****************************"
  //       << std::endl;

  //   m_preOpImage =
  //   SCBScene::getCurrentScene()->getDataByAlias<SCBImage>("PreOp");
  //   m_postOpImage =
  //       SCBScene::getCurrentScene()->getDataByAlias<SCBImage>("PostOp");

  //   if (m_preOpImage == nullptr || m_postOpImage == nullptr)
  //     return;

  //   m_preOpImage->getITKImageData<short>(m_preOpItkImage);
  //   m_postOpImage->getITKImageData<short>(m_postOpItkImage);

  //   itk::Matrix<double, 3, 3> regMatrix;
  //   for (int i = 0; i < 3; i++)
  //   {
  //     for (int j = 0; j < 3; j++)
  //     {
  //       regMatrix(i, j) = m_postOpImage->getUserMatrix()->Element[i][j];
  //     }
  //   }

  //   itk::Matrix<double, 3, 3> regMatrixInverse;
  //   regMatrixInverse = regMatrix.GetInverse();

  //   double worldBounds[6];
  //   m_postOpImage->getWorldBounds(worldBounds);

  //   if (autoReg)
  //   {
  //     std::cout << "perform auto validation" << std::endl;

  //     ImageRegistration imgReg;
  //     imgReg.SetInitialMatrix(regMatrixInverse);
  //     imgReg.SetInitialTranslation(this->translateXSpinBox->value(),
  //                                  this->translateYSpinBox->value(),
  //                                  this->translateZSpinBox->value());
  //     imgReg.SetInitialCenter((worldBounds[1] + worldBounds[0]) / 2,
  //                             (worldBounds[3] + worldBounds[2]) / 2,
  //                             (worldBounds[5] + worldBounds[4]) / 2);
  //     imgReg.SetFixedImage(m_preOpItkImage);
  //     imgReg.SetMovingImage(m_postOpItkImage);
  //     imgReg.Update();

  //     imgReg.GetOutputTransform()->Print(std::cout);

  //     // compute 6Dof widget values from the output transform
  //     for (int i = 0; i < 3; i++)
  //     {
  //       for (int j = 0; j < 3; j++)
  //       {
  //         m_postOpImage->getUserMatrix()->SetElement(
  //             i, j, imgReg.GetOutputTransform()->GetMatrix()[i][j]);
  //       }
  //     }

  //     m_postOpImage->getUserMatrix()->SetElement(
  //         0, 3, imgReg.GetOutputTransform()->GetOffset()[0]);
  //     m_postOpImage->getUserMatrix()->SetElement(
  //         1, 3, imgReg.GetOutputTransform()->GetOffset()[1]);
  //     m_postOpImage->getUserMatrix()->SetElement(
  //         2, 3, imgReg.GetOutputTransform()->GetOffset()[2]);

  //     m_postOpImage->getUserMatrix()->Invert();

  //     this->itkTransformToWidget(imgReg.GetOutputTransform());

  //     for (int i = 0; i < 4; i++)
  //     {
  //       SCBDentalHub::self->mainWindow->getViewer(i)->Render();
  //     }

  //     regMatrixInverse = imgReg.GetOutputTransform()->GetMatrix();
  //   }
  //   else
  //   {
  //     std::cout << "perform manual validation" << std::endl;
  //   }

  //   // get the image by applying the transform to the fixed image using
  //   resampling typedef itk::ResampleImageFilter<itk::Image<short, 3>,
  //   itk::Image<short, 3>>
  //       ResampleFilterType;
  //   typedef itk::VersorRigid3DTransform<double> TransformType;

  //   ResampleFilterType::Pointer resampler = ResampleFilterType::New();
  //   TransformType::Pointer transform = TransformType::New();

  //   // transform
  //   // std::cout << "registration 3x3 matrix" << std::endl;
  //   // std::cout << regMatrixInverse << std::endl;

  //   transform->SetMatrix(regMatrixInverse);

  //   // std::cout << "registration translation" << std::endl;
  //   itk::Vector<double, 3> regTranslation;
  //   regTranslation.SetElement(0, this->translateXSpinBox->value());
  //   regTranslation.SetElement(1, this->translateYSpinBox->value());
  //   regTranslation.SetElement(2, this->translateZSpinBox->value());

  //   // std::cout << regTranslation[0] << ","
  //   //	<< regTranslation[1] << ","
  //   //	<< regTranslation[2] << std::endl;
  //   transform->SetTranslation(regTranslation);

  //   // std::cout << "registration center" << std::endl;
  //   itk::Vector<double, 3> regCenter;
  //   regCenter.SetElement(0, (worldBounds[1] + worldBounds[0]) / 2);
  //   regCenter.SetElement(1, (worldBounds[3] + worldBounds[2]) / 2);
  //   regCenter.SetElement(2, (worldBounds[5] + worldBounds[4]) / 2);
  //   // std::cout << regCenter[0] << ","
  //   //	<< regCenter[1] << ","
  //   //	<< regCenter[2] << std::endl;
  //   transform->SetCenter(regCenter);

  //   std::cout << "transform for resampling" << std::endl;
  //   transform->Print(std::cout);

  //   // resampler->SetTransform(finalTransform);
  //   resampler->SetTransform(transform);
  //   resampler->SetInput(m_postOpItkImage);

  //   // set the parameters from the fixed image
  //   resampler->SetSize(m_preOpItkImage->GetLargestPossibleRegion().GetSize());
  //   resampler->SetOutputOrigin(m_preOpItkImage->GetOrigin());
  //   resampler->SetOutputSpacing(m_preOpItkImage->GetSpacing());
  //   resampler->SetOutputDirection(m_preOpItkImage->GetDirection());
  //   resampler->SetDefaultPixelValue(0);
  //   resampler->Update();

  //   m_postOpItkImageReg->Graft(resampler->GetOutput());
}

void SCBDentalValidationPipeline::validation()
{
  //   std::cout
  //       <<
  //       "**************************validation*****************************"
  //       << std::endl;

  //   // test if there are any planned implants
  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   QStringList implants = scene->getAllDataByClassName("SCBDentalFixture");
  //   if (implants.size() < 1)
  //   {
  //     return;
  //   }

  //   double plannedImplantStlVector[4] = {0, 0, 1, 0};
  //   unsigned int plannedImplantCount = 0;

  //   // std::cout << "plannedImplant: ";
  //   for (QStringList::const_iterator cit = implants.cbegin();
  //        cit != implants.cend();
  //        ++cit)
  //   {
  //     SCBDentalFixture* implant =
  //         scene->getDataByUniqueName<SCBDentalFixture>(*cit);

  //     // save the result in needle property
  //     NeedleProperty* needleProperty = new NeedleProperty;
  //     m_plannedNeedlePropertyCollections.push_back(needleProperty);

  //     needleProperty->label = plannedImplantCount + 1;

  //     double* plannedImplantVector =
  //         implant->getUserMatrix()->MultiplyDoublePoint(plannedImplantStlVector);
  //     for (int i = 0; i < 3; i++)
  //     {
  //       needleProperty->majorPrincipalAxes[i] = plannedImplantVector[i];
  //       needleProperty->centroid[i] = implant->getUserMatrix()->GetElement(i,
  //       3);
  //     }

  //     // std::cout << "needle: " << needleProperty->label << std::endl;
  //     // std::cout << "direction: " << needleProperty->majorPrincipalAxes[0]
  //     <<
  //     // "," << needleProperty->majorPrincipalAxes[1] << "," <<
  //     // needleProperty->majorPrincipalAxes[2] << std::endl; std::cout <<
  //     // "centroid: " << needleProperty->centroid[0] << "," <<
  //     // needleProperty->centroid[1] << "," << needleProperty->centroid[2] <<
  //     // std::endl;

  //     plannedImplantCount++;
  //   }

  //   typedef itk::ShapeLabelObject<unsigned short, 3> ShapeLabelObjectType;
  //   typedef itk::LabelMap<ShapeLabelObjectType> ShapeLabelMapType;
  //   typedef itk::LabelImageToShapeLabelMapFilter<LabelImageType,
  //                                                ShapeLabelMapType>
  //       LabelImageToShapeLabelMapFilterType;
  //   typedef itk::MergeLabelMapFilter<ShapeLabelMapType>
  //       MergeShapeLabelMapFilterType;

  //   itk::SubtractImageFilter<ImageType>::Pointer subtractFilter =
  //       itk::SubtractImageFilter<ImageType>::New();
  //   subtractFilter->SetInput1(m_postOpItkImageReg);
  //   subtractFilter->SetInput2(m_preOpItkImage);
  //   subtractFilter->Update();

  //   std::cout << "running first otsu..." << std::endl;
  //   itk::OtsuMultipleThresholdsImageFilter<ImageType,
  //                                          LabelImageType>::Pointer
  //                                          otsuFilter =
  //       itk::OtsuMultipleThresholdsImageFilter<ImageType,
  //       LabelImageType>::New();
  //   otsuFilter->SetInput(subtractFilter->GetOutput());
  //   otsuFilter->SetNumberOfThresholds(4);
  //   otsuFilter->Update();

  //   itk::BinaryThresholdImageFilter<LabelImageType,
  //                                   LabelImageType>::Pointer thresholdFilter
  //                                   =
  //       itk::BinaryThresholdImageFilter<LabelImageType,
  //       LabelImageType>::New();
  //   thresholdFilter->SetInput(otsuFilter->GetOutput());
  //   thresholdFilter->SetLowerThreshold(4);
  //   thresholdFilter->SetUpperThreshold(4);
  //   thresholdFilter->SetInsideValue(1);
  //   thresholdFilter->SetOutsideValue(0);
  //   thresholdFilter->Update();

  //   itk::MaskImageFilter<ImageType, LabelImageType, ImageType>::Pointer
  //       maskFilter =
  //           itk::MaskImageFilter<ImageType, LabelImageType,
  //           ImageType>::New();
  //   maskFilter->SetInput1(subtractFilter->GetOutput());
  //   maskFilter->SetInput2(thresholdFilter->GetOutput());
  //   maskFilter->Update();

  //   std::cout << "running second otsu..." << std::endl;
  //   itk::OtsuMultipleThresholdsImageFilter<ImageType,
  //                                          LabelImageType>::Pointer
  //                                          otsuFilter2 =
  //       itk::OtsuMultipleThresholdsImageFilter<ImageType,
  //       LabelImageType>::New();
  //   otsuFilter2->SetInput(maskFilter->GetOutput());
  //   otsuFilter2->SetNumberOfThresholds(3);
  //   otsuFilter2->Update();

  //   itk::BinaryThresholdImageFilter<LabelImageType,
  //                                   LabelImageType>::Pointer thresholdFilter2
  //                                   =
  //       itk::BinaryThresholdImageFilter<LabelImageType,
  //       LabelImageType>::New();
  //   thresholdFilter2->SetInput(otsuFilter2->GetOutput());
  //   thresholdFilter2->SetLowerThreshold(3);
  //   thresholdFilter2->SetUpperThreshold(3);
  //   thresholdFilter2->SetInsideValue(1);
  //   thresholdFilter2->SetOutsideValue(0);
  //   thresholdFilter2->Update();

  //   // itk::ImageFileWriter<itk::Image<LabelImageType::PixelType,
  //   3>>::Pointer
  //   // writer = itk::ImageFileWriter<itk::Image<LabelImageType::PixelType,
  //   // 3>>::New();

  //   //
  //   writer->SetFileName("D:/projects/SucabotProject/build/ImageData/1/nii/registered.nii.gz");
  //   // writer->SetInput(thresholdFilter2->GetOutput());
  //   // writer->Write();
  //   // std::cout << "regstered image saved" << std::endl;

  //   // extract connected components with volume thresholding
  //   typedef itk::ConnectedComponentImageFilter<LabelImageType,
  //   LabelImageType>
  //       ConnectedComponentImageFilterType;

  //   ConnectedComponentImageFilterType::Pointer ccFilter =
  //       ConnectedComponentImageFilterType::New();
  //   ccFilter->SetInput(thresholdFilter2->GetOutput());
  //   ccFilter->Update();

  //   LabelImageToShapeLabelMapFilterType::Pointer
  //   labelImageToShapeLabelMapFilter =
  //       LabelImageToShapeLabelMapFilterType::New();
  //   labelImageToShapeLabelMapFilter->SetInput(ccFilter->GetOutput());
  //   labelImageToShapeLabelMapFilter->SetComputeOrientedBoundingBox(true);
  //   labelImageToShapeLabelMapFilter->Update();

  //   MergeShapeLabelMapFilterType::Pointer mergeMapFilter =
  //       MergeShapeLabelMapFilterType::New();
  //   mergeMapFilter->SetMethod(
  //       MergeShapeLabelMapFilterType::PACK);  // relabel all the label
  //       objects by
  //                                             // order of processing

  //   int selectedCount = 0;
  //   double dirVec[3] = {1, 1, 1};
  //   for (int i = 0;
  //        i <
  //        labelImageToShapeLabelMapFilter->GetOutput()->GetNumberOfLabelObjects();
  //        i++)
  //   {
  //     double labelVol = labelImageToShapeLabelMapFilter->GetOutput()
  //                           ->GetNthLabelObject(i)
  //                           ->GetPhysicalSize();
  //     if (labelVol >= m_needleVol * 0.3 && labelVol <= m_needleVol * 2.3)
  //     {
  //       typedef itk::LabelSelectionLabelMapFilter<ShapeLabelMapType>
  //       SelectorType; SelectorType::Pointer selector = SelectorType::New();
  //       selector->SetInput(labelImageToShapeLabelMapFilter->GetOutput());
  //       selector->SetLabel(i + 1);  // label 0 is a special number that
  //       ignores in
  //                                   // the filter, take care on the loop
  //       selector->Update();

  //       //
  //       labelImageToShapeLabelMapFilter->GetOutput()->GetNthLabelObject(i)->Print(std::cout);

  //       // save the result in needle property
  //       NeedleProperty* needleProperty = new NeedleProperty;
  //       m_plantedNeedlePropertyCollections.push_back(needleProperty);

  //       needleProperty->label = selectedCount + 1;

  //       // major principal axes, note that this is the 3rd row of PCA matrix
  //       double needleNorm[3];

  //       for (int j = 0; j < 3; j++)
  //       {
  //         needleNorm[j] = labelImageToShapeLabelMapFilter->GetOutput()
  //                             ->GetNthLabelObject(i)
  //                             ->GetPrincipalAxes()(2, j);
  //         needleProperty->majorPrincipalAxes[j] = needleNorm[j];
  //       }

  //       // centroid of the label
  //       double centroid[3];
  //       for (int j = 0; j < 3; j++)
  //       {
  //         centroid[j] = labelImageToShapeLabelMapFilter->GetOutput()
  //                           ->GetNthLabelObject(i)
  //                           ->GetCentroid()[j] *
  //                       dirVec[j];
  //         needleProperty->centroid[j] = centroid[j];
  //       }

  //       // needle length is obtained by z length of oriented bounding box
  //       // (equivalent to direction of major pricipal axes)
  //       double needleLen = labelImageToShapeLabelMapFilter->GetOutput()
  //                              ->GetNthLabelObject(i)
  //                              ->GetOrientedBoundingBoxSize()[2];
  //       needleProperty->needleLength = needleLen;

  //       double start[3];
  //       double end[3];
  //       for (int j = 0; j < 3; j++)
  //       {
  //         start[j] = centroid[j] - 0.5 * needleLen * needleNorm[j] *
  //         dirVec[j]; end[j] = centroid[j] + 0.5 * needleLen * needleNorm[j] *
  //         dirVec[j];

  //         needleProperty->start[j] = start[j];
  //         needleProperty->end[j] = end[j];
  //       }

  //       mergeMapFilter->SetInput(selectedCount, selector->GetOutput());

  //       // std::cout << "needle: " << needleProperty->label << std::endl;
  //       // std::cout << "direction: " <<
  //       needleProperty->majorPrincipalAxes[0] <<
  //       // "," << needleProperty->majorPrincipalAxes[1] << "," <<
  //       // needleProperty->majorPrincipalAxes[2]<< std::endl; std::cout <<
  //       // "centroid: " << needleProperty->centroid[0] << "," <<
  //       // needleProperty->centroid[1] << "," << needleProperty->centroid[2]
  //       <<
  //       // std::endl;

  //       selectedCount++;
  //     }
  //   }

  //   // distance threshold to pair up planned and planted needles
  //   double distanceThreshold = 10;

  //   // add planned implant to validation table
  //   for (int i = 0; i < m_plannedNeedlePropertyCollections.size(); i++)
  //   {
  //     if (this->tableWidget->columnCount() <
  //         m_plannedNeedlePropertyCollections.size())
  //     {
  //       this->tableWidget->insertColumn(i);
  //     }

  //     this->tableWidget->setItem(0, i, new QTableWidgetItem());
  //     this->tableWidget->item(0, i)->setText(QString("implant") +
  //                                            QString::number(i));

  //     // find nearest planted implant
  //     double minDistance = VTK_DOUBLE_MAX;
  //     unsigned int label = 0;
  //     double angle = 0;
  //     for (int j = 0; j < m_plantedNeedlePropertyCollections.size(); j++)
  //     {
  //       double squaredDistance = vtkMath::Distance2BetweenPoints(
  //           m_plannedNeedlePropertyCollections.at(i)->centroid,
  //           m_plantedNeedlePropertyCollections.at(j)->centroid);
  //       double distance = sqrt(squaredDistance);
  //       if (distance < minDistance)
  //       {
  //         minDistance = distance;
  //         label = m_plantedNeedlePropertyCollections.at(j)->label;

  //         // calculate angle between the planned and planted implant
  //         double crossProduct[3];
  //         vtkMath::Cross(
  //             m_plannedNeedlePropertyCollections.at(i)->majorPrincipalAxes,
  //             m_plantedNeedlePropertyCollections.at(j)->majorPrincipalAxes,
  //             crossProduct);
  //         double dotProduct = vtkMath::Dot(
  //             m_plannedNeedlePropertyCollections.at(i)->majorPrincipalAxes,
  //             m_plantedNeedlePropertyCollections.at(j)->majorPrincipalAxes);
  //         angle = atan(vtkMath::Norm(crossProduct) / dotProduct) * 180 / PI;
  //       }
  //     }

  //     if (minDistance > distanceThreshold)
  //     {
  //       break;
  //     }
  //     else
  //     {
  //       this->tableWidget->setItem(1, i, new QTableWidgetItem());
  //       this->tableWidget->item(1, i)->setText(QString("Implant") +
  //                                              QString::number(label));
  //       this->tableWidget->setItem(2, i, new QTableWidgetItem());
  //       this->tableWidget->item(2, i)->setText(QString::number(minDistance));
  //       this->tableWidget->setItem(3, i, new QTableWidgetItem());
  //       this->tableWidget->item(3, i)->setText(QString::number(angle));
  //     }
  //   }
}
