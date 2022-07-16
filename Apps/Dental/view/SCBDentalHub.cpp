// me
#include "SCBDentalHub.h"

// #include "SCBWelcomeWizard.h"

// #include "SCBScene.h"
// #include "SCBImage.h"
// #include "SCBImageSlice.h"
// #include "SCBOrthogonalViewer.h"
// #include "SCBVolume.h"
// #include "SCBImplant.h"
// #include "SCBPolyDataActor.h"
// #include "SCBStylesSwitchOrthogonalViewer.h"
// #include "SCBWidgetCollection.h"
// #include "SCBPolyDataSource.h"
// #include "PoseIndicatorViewer.h"
// #include "SCBBiopsyData.h"

#include "SCBDentalMainWindow.h"
#include "SCBDentalNavigationPipeline.h"
#include "SCBDentalPlanningPipeline.h"
#include "SCBDentalRegistrationPipeline.h"
#include "SCBDentalValidationPipeline.h"
// #include "SCBPatientInfo.h"

// vtk
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataArray.h>
#include <vtkImageAccumulate.h>
#include <vtkImageBimodalAnalysis.h>
#include <vtkImageData.h>
#include <vtkMatrix3x3.h>
#include <vtkMatrix4x4.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>

// qt
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QProgressDialog>

SCBDentalHub* SCBDentalHub::self = nullptr;
SCBDentalHub::SCBDentalHub(QObject* parent) : QObject(parent)
{
  self = this;
  this->mainWindow = new SCBDentalMainWindow;
  this->mainWindow->show();
  // process render event to make sure the QVTKWidget can be exposed.
  QCoreApplication::processEvents();

  //   SCBScene* scene = SCBScene::getCurrentScene();

  // connection
  QObject::connect(this->mainWindow,
                   &SCBDentalMainWindow::signalImportImages,
                   this,
                   &SCBDentalHub::slotInitializationImages);
  QObject::connect(this->mainWindow,
                   &SCBDentalMainWindow::signalImportLabelImages,
                   this,
                   &SCBDentalHub::slotInitializationLabeledImages);
  QObject::connect(this->mainWindow,
                   &SCBDentalMainWindow::signalImportedProject,
                   this,
                   &SCBDentalHub::slotImport);
  QObject::connect(this->mainWindow,
                   &SCBDentalMainWindow::signalCommitedProject,
                   this,
                   &SCBDentalHub::slotCommit);
  //   QObject::connect(this->mainWindow->action_Exit,
  //                    &QAction::triggered,
  //                    this,
  //                    &SCBDentalHub::slotExit);
  QObject::connect(this,
                   SIGNAL(signalInitializationParanomicView()),
                   this,
                   SLOT(slotInitializationParanomicView()));

  // interactorstyle
  for (int i = 0; i < NUM_OF_ORTHOGONAL_VIEWER; ++i)
  {
    // this->styles[i] = SCBStylesSwitchOrthogonalViewer::New();
    // this->styles[i]->SetViewer(this->mainWindow->getViewer(i));
    // this->styles[i]->SetDefaultRenderer(
    //     this->mainWindow->getViewer(i)->GetRenderers()[0]);
    // this->mainWindow->getViewer(i)->GetInteractor()->SetInteractorStyle(
    //     this->styles[i]);
  }

  //   QObject::connect(this->mainWindow->action_Window_Level_Mode,
  //                    &QAction::toggled,
  //                    this,
  //                    &SCBDentalHub::slotInteractorStyleWindowLevel);
  //   QObject::connect(this->mainWindow->action_Navigation_Mode,
  //                    &QAction::toggled,
  //                    this,
  //                    &SCBDentalHub::slotInteractorStyleNavigation);

  for (int i = 0; i < NUM_OF_ORTHOGONAL_VIEWER; ++i)
  {
    // this->widgets[i] = SCBWidgetCollection::New();
    // this->widgets[i]->SetViewer(this->mainWindow->getViewer(i));
    // this->widgets[i]->SetDefaultRenderer(
    //     this->mainWindow->getViewer(i)->GetRenderers()[0]);
    // this->widgets[i]->SetInteractor(
    //     this->mainWindow->getViewer(i)->GetInteractor());
  }

  QObject::connect(this,
                   &SCBDentalHub::signalLoading,
                   this->mainWindow,
                   &SCBDentalMainWindow::slotLoadingDialog);
  QObject::connect(this,
                   &SCBDentalHub::signalSetLoadingValue,
                   this->mainWindow,
                   &SCBDentalMainWindow::slotSetLoadingDialogValue);

  // emit signalInitializationParanomicView();
}

SCBDentalHub::~SCBDentalHub()
{
  delete this->mainWindow;

  for (int i = 0; i < NUM_OF_ORTHOGONAL_VIEWER; ++i)
  {
    // this->widgets[i]->SetInteractor(nullptr);
    // this->widgets[i]->Delete();
  }

  for (int i = 0; i < NUM_OF_ORTHOGONAL_VIEWER; ++i)
  {
    // this->styles[i]->SetInteractor(nullptr);
    // this->styles[i]->Delete();
  }
}

void SCBDentalHub::slotInitializationImport()
{
  //   this->mainWindow->getViewer(0)->GetCursorActor()->VisibilityOn();
  //   this->mainWindow->getViewer(1)->GetCursorActor()->VisibilityOn();
  //   this->mainWindow->getViewer(2)->GetCursorActor()->VisibilityOn();
  //   this->mainWindow->getViewer(3)->GetCursorActor()->VisibilityOn();

  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   QStringList props = scene->getAllDataByClassName("SCBAbstractProp");

  //   for (QStringList::const_iterator cit = props.cbegin(); cit !=
  //   props.cend();
  //        ++cit)
  //   {
  //     SCBAbstractProp* prop =
  //         static_cast<SCBAbstractProp*>(scene->getDataByUniqueName(*cit));
  //     long long id = prop->getViewerID();
  //     int layer = prop->getLayer();
  //     if (id > -1 && id < NUM_OF_ORTHOGONAL_VIEWER)
  //     {
  //       this->mainWindow->getViewer(id)->AddProp(
  //           prop, this->mainWindow->getViewer(id)->GetRenderers()[layer]);
  //     }
  //   }

  //   for (int i = 0; i < 3; ++i)
  //   {
  //     switch (i)
  //     {
  //       case 0:
  //         this->mainWindow->getViewer(i)->SetOrientationToAxial();
  //         break;
  //       case 1:
  //         this->mainWindow->getViewer(i)->SetOrientationToCoronal();
  //         break;
  //       case 2:
  //         this->mainWindow->getViewer(i)->SetOrientationToSagital();
  //         break;
  //       default:
  //         break;
  //     }
  //   }

  //   this->slotInitializationMainWindow();

  //   this->mainWindow->action_Navigation_Mode->trigger();
  //   this->styles[0]->GetNavigation()->CentralizeCursorPosition();
  //   this->styles[1]->GetNavigation()->CentralizeCursorPosition();
  //   this->styles[2]->GetNavigation()->CentralizeCursorPosition();
  //   this->styles[3]->GetNavigation()->CentralizeCursorPosition();

  //   this->mainWindow->getViewer(0)->ResetCamera(0);
  //   this->mainWindow->getViewer(0)->ResetCameraClippingRange(0);
  //   this->mainWindow->getViewer(1)->ResetCamera(0);
  //   this->mainWindow->getViewer(1)->ResetCameraClippingRange(0);
  //   this->mainWindow->getViewer(2)->ResetCamera(0);
  //   this->mainWindow->getViewer(2)->ResetCameraClippingRange(0);
  //   this->mainWindow->getViewer(3)->ResetCamera(0);
  //   this->mainWindow->getViewer(3)->ResetCameraClippingRange(0);
  //   SCBAbstractViewer::RenderAllViewers();
}

void SCBDentalHub::slotInitializationMainWindow()
{
  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   SCBPolyData* data = scene->getDataByUniqueName<SCBPolyData>(
  //       SCBPlanarSeedWidget::SCB_PLANAR_SEED_WIDGET_DATA);
  //   if (!data || !data->getPolyData() || !data->getPolyData()->GetPoints())
  //   {
  //     return;
  //   }
  //   vtkPoints* points = data->getPolyData()->GetPoints();
}

void SCBDentalHub::slotCommit(QString path)
{
  //   emit signalLoading(true);
  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   scene->setSaveToXMLString(false);
  //   scene->setURL(path);
  //   scene->commit(QFileInfo(path).baseName());
  //   emit signalLoading(false);
}

void SCBDentalHub::slotImport(QString path, bool flag)
{
  emit signalLoading(true);
  if (flag)
  {
    slotClean();
  }

  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   scene->setURL(path);
  //   scene->setLoadFromXMLString(false);
  //   scene->import(flag);

  slotInitializationImport();
  emit signalInitializationParanomicView();
  emit signalLoading(false);
}

void SCBDentalHub::slotImportString(QString XML)
{
  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   scene->setSceneXMLString(XML);
  //   scene->setLoadFromXMLString(true);
  //   scene->import();

  //   slotInitializationImport();
}

void SCBDentalHub::slotClean()
{
  //   this->mainWindow->getViewer(0)->RemoveAllProp(
  //       this->mainWindow->getViewer(0)->GetRenderers()[0]);
  //   this->mainWindow->getViewer(0)->RemoveAllProp(
  //       this->mainWindow->getViewer(0)->GetRenderers()[1]);
  //   this->mainWindow->getViewer(0)->GetCursorActor()->VisibilityOff();
  //   this->mainWindow->getViewer(1)->RemoveAllProp(
  //       this->mainWindow->getViewer(1)->GetRenderers()[0]);
  //   this->mainWindow->getViewer(1)->RemoveAllProp(
  //       this->mainWindow->getViewer(1)->GetRenderers()[1]);
  //   this->mainWindow->getViewer(1)->GetCursorActor()->VisibilityOff();
  //   this->mainWindow->getViewer(2)->RemoveAllProp(
  //       this->mainWindow->getViewer(2)->GetRenderers()[0]);
  //   this->mainWindow->getViewer(2)->RemoveAllProp(
  //       this->mainWindow->getViewer(2)->GetRenderers()[1]);
  //   this->mainWindow->getViewer(2)->GetCursorActor()->VisibilityOff();
  //   this->mainWindow->getViewer(3)->RemoveAllProp(
  //       this->mainWindow->getViewer(3)->GetRenderers()[0]);
  //   this->mainWindow->getViewer(3)->GetCursorActor()->VisibilityOff();
  //   SCBAbstractViewer::RenderAllViewers();

  //   SCBScene::getCurrentScene()->clear();
}

void SCBDentalHub::slotExit()
{
  this->mainWindow->close();
}

void SCBDentalHub::slotInteractorStyleWindowLevel(bool flag)
{
  if (!flag)
  {
    return;
  }
  for (int i = 0; i < NUM_OF_ORTHOGONAL_VIEWER; i++)
  {
    // this->styles[i]->SetInteractorStyleToWindowLevel();
  }
}

void SCBDentalHub::slotInteractorStyleNavigation(bool flag)
{
  if (!flag)
  {
    return;
  }
  for (int i = 0; i < NUM_OF_ORTHOGONAL_VIEWER; i++)
  {
    // this->styles[i]->SetInteractorStyleToNavigation();
  }
}

void SCBDentalHub::slotInitializationImages(QStringList imagePaths)
{
  emit signalLoading(true);
  //   SCBScene* scene = SCBScene::getCurrentScene();
  emit signalSetLoadingValue(69);
  emit signalInitializationParanomicView();
  //   for (QStringList::const_iterator cit = imagePaths.cbegin();
  //        cit != imagePaths.cend();
  //        ++cit)
  //   {
  //     SCBImage* image = scene->addNewDataByClass<SCBImage>();
  //     image->setAbsolutePath(cit->split(";"));
  //     image->readData();
  //     image->setRelativePath(QStringList() << image->getUniqueName() +
  //     ".nii.gz");

  //     for (int i = 0; i < 3; ++i)
  //     {
  //       SCBImageSlice* imageSlice = new SCBImageSlice;
  //       imageSlice->setRenderDataSet(image);
  //       scene->addData(imageSlice);
  //       this->mainWindow->getViewer(i)->AddProp(imageSlice);
  //       switch (i)
  //       {
  //         case 0:
  //           this->mainWindow->getViewer(i)->SetOrientationToAxial();
  //           break;
  //         case 1:
  //           this->mainWindow->getViewer(i)->SetOrientationToCoronal();
  //           break;
  //         case 2:
  //           this->mainWindow->getViewer(i)->SetOrientationToSagital();
  //           break;
  //         default:
  //           break;
  //       }
  //     }

  //     ////////////Initialize Image Window Level////////////
  //     vtkImageAccumulate* accumulate = vtkImageAccumulate::New();
  //     vtkImageBimodalAnalysis* bimodal = vtkImageBimodalAnalysis::New();
  //     bool needAdHoc = 0;
  //     int window = 0;
  //     int level = 0;

  //     if (image->getImageData()->GetNumberOfScalarComponents() >= 3)
  //     {
  //       needAdHoc = 1;
  //     }
  //     else if (image->getImageData()->GetScalarType() == VTK_INT ||
  //              image->getImageData()->GetScalarType() == VTK_SHORT ||
  //              image->getImageData()->GetScalarType() == VTK_CHAR ||
  //              image->getImageData()->GetScalarType() == VTK_SIGNED_CHAR ||
  //              image->getImageData()->GetScalarType() == VTK_UNSIGNED_CHAR ||
  //              image->getImageData()->GetScalarType() == VTK_UNSIGNED_SHORT
  //              || image->getImageData()->GetScalarType() == VTK_UNSIGNED_INT)
  //     {
  //       // Data type is VTK_INT or similar, so calculate window/level
  //       // check the scalar type, bimodal analysis only works on int

  //       // Setup filter to work with signed 16-bit integer.
  //       int extent[6] = {0, 65535, 0, 0, 0, 0};
  //       accumulate->SetComponentExtent(extent);
  //       double origin[3] = {-32768, 0, 0};
  //       accumulate->SetComponentOrigin(origin);
  //       accumulate->SetInputData(image->getImageData());
  //       bimodal->SetInputConnection(accumulate->GetOutputPort());
  //       bimodal->Update();
  //       // Workaround for image data where all accumulate samples fall
  //       // within the same histogram bin
  //       if (bimodal->GetWindow() == 0.0 && bimodal->GetLevel() == 0.0)
  //       {
  //         needAdHoc = 1;
  //       }
  //       else
  //       {
  //         window = bimodal->GetWindow();
  //         level = bimodal->GetLevel();
  //         // lower = bimodal->GetThreshold();
  //         // upper = bimodal->GetMax();
  //       }
  //     }
  //     else if (image->getImageData()->GetScalarType() == VTK_FLOAT ||
  //              image->getImageData()->GetScalarType() == VTK_DOUBLE ||
  //              image->getImageData()->GetScalarType() == VTK_LONG ||
  //              image->getImageData()->GetScalarType() == VTK_UNSIGNED_LONG)
  //     {
  //       // If scalar range is expected to be less conventional, then scale
  //       the
  //       // bins the image accumulate algorithm
  //       double range[2];
  //       // this->GetDisplayScalarRange(range);
  //       image->getImageData()
  //           ->GetPointData()
  //           ->GetArray("ImageScalars")
  //           ->GetRange(range);
  //       qDebug() << range[0];
  //       qDebug() << range[1];

  //       long minInt = trunc(range[0]) - 1;
  //       long maxInt = trunc(range[1]) + 1;

  //       accumulate->SetInputData(image->getImageData());
  //       int extent[6] = {0, 999, 0, 0, 0, 0};
  //       accumulate->SetComponentExtent(extent);
  //       double origin[3] = {static_cast<double>(minInt), 0.0, 0.0};
  //       accumulate->SetComponentOrigin(origin);
  //       double spacing[3] = {(maxInt - minInt) / 1000.0, 1.0, 1.0};
  //       accumulate->SetComponentSpacing(spacing);

  //       bimodal->SetInputConnection(accumulate->GetOutputPort());
  //       bimodal->Update();

  //       // The bimodal analysis assumes that the bin indices correspond
  //       directly
  //       // to voxel intensity values, need to convert back to intensity space
  //       window = bimodal->GetWindow() * spacing[0];
  //       level = minInt + (bimodal->GetLevel() - minInt) * spacing[0];
  //       // lower = minInt + (bimodal->GetThreshold() - minInt) * spacing[0];
  //       // upper = minInt + (bimodal->GetMax() - minInt) * spacing[0];
  //     }
  //     // else
  //     //{
  //     //	// If unhandled type, estimate with ad hoc method
  //     //	needAdHoc = 1;
  //     // }

  //     if (needAdHoc)
  //     {
  //       qDebug()
  //           << ("CalculateScalarAutoLevels: image data scalar type is not "
  //               "integer,"
  //               " doing ad hoc calc of window/level.");
  //       double range[2];
  //       image->getImageData()
  //           ->GetPointData()
  //           ->GetArray("ImageScalars")
  //           ->GetRange(range);
  //       // this->GetDisplayScalarRange(range);

  //       double min = range[0];
  //       double max = range[1];

  //       window = max - min;
  //       level = 0.5 * (max + min);
  //       // lower = level;
  //       // upper = range[1];
  //     }

  //     qDebug() << "window" << window;
  //     qDebug() << "level" << level;
  //     image->setWindow(window);
  //     image->setLevel(level);

  //     bimodal->Delete();
  //     accumulate->Delete();

  //     ////////////Create Volume Rendering////////////
  //     SCBVolume* volume = new SCBVolume;
  //     volume->setRenderDataSet(image);
  //     // volume->setPresetToCBCTDental();
  //     scene->addData(volume);
  //     this->mainWindow->getViewer(3)->AddProp(volume);
  //     this->mainWindow->getViewer(3)->SetOrientationToAxial();
  //   }
  //   emit signalSetLoadingValue(87);

  //   this->mainWindow->action_Navigation_Mode->trigger();
  //   this->styles[0]->GetNavigation()->CentralizeCursorPosition();
  //   this->styles[1]->GetNavigation()->CentralizeCursorPosition();
  //   this->styles[2]->GetNavigation()->CentralizeCursorPosition();
  //   // this->mainWindow->action_Dental_Planning->trigger();
  //   // this->styles[3]->GetNavigation()->CentralizeCursorPosition();

  //   this->mainWindow->getViewer(0)->ResetCamera(0);
  //   this->mainWindow->getViewer(0)->ResetCameraClippingRange(0);
  //   this->mainWindow->getViewer(0)->GetCursorActor()->VisibilityOn();
  //   this->mainWindow->getViewer(1)->ResetCamera(0);
  //   this->mainWindow->getViewer(1)->ResetCameraClippingRange(0);
  //   this->mainWindow->getViewer(1)->GetCursorActor()->VisibilityOn();
  //   this->mainWindow->getViewer(2)->ResetCamera(0);
  //   this->mainWindow->getViewer(2)->ResetCameraClippingRange(0);
  //   this->mainWindow->getViewer(2)->GetCursorActor()->VisibilityOn();
  //   this->mainWindow->getViewer(3)->ResetCamera(0);
  //   this->mainWindow->getViewer(3)->ResetCameraClippingRange(0);
  //   this->mainWindow->getViewer(3)->GetCursorActor()->VisibilityOn();
  //   SCBAbstractViewer::RenderAllViewers();

  emit signalLoading(false);
}

void SCBDentalHub::slotInitializationLabeledImages(QStringList imagePaths,
                                                   QString label)
{
  //   SCBScene* scene = SCBScene::getCurrentScene();

  //   for (QStringList::const_iterator cit = imagePaths.cbegin();
  //        cit != imagePaths.cend();
  //        ++cit)
  //   {
  //     SCBImage* image = scene->addNewDataByClass<SCBImage>();
  //     image->setAbsolutePath(cit->split(";"));
  //     image->readData();
  //     image->setRelativePath(QStringList() << image->getUniqueName() +
  //     ".nii.gz"); image->setAlias(label);

  //     for (int i = 0; i < 3; ++i)
  //     {
  //       SCBImageSlice* imageSlice = new SCBImageSlice;
  //       imageSlice->setRenderDataSet(image);
  //       scene->addData(imageSlice);
  //       this->mainWindow->getViewer(i)->AddProp(imageSlice);
  //       switch (i)
  //       {
  //         case 0:
  //           this->mainWindow->getViewer(i)->SetOrientationToAxial();
  //           break;
  //         case 1:
  //           this->mainWindow->getViewer(i)->SetOrientationToCoronal();
  //           break;
  //         case 2:
  //           this->mainWindow->getViewer(i)->SetOrientationToSagital();
  //           break;
  //         default:
  //           break;
  //       }
  //     }

  //     ////////////Initialize Image Window Level////////////
  //     vtkImageAccumulate* accumulate = vtkImageAccumulate::New();
  //     vtkImageBimodalAnalysis* bimodal = vtkImageBimodalAnalysis::New();
  //     bool needAdHoc = 0;
  //     int window = 0;
  //     int level = 0;

  //     if (image->getImageData()->GetNumberOfScalarComponents() >= 3)
  //     {
  //       needAdHoc = 1;
  //     }
  //     else if (image->getImageData()->GetScalarType() == VTK_INT ||
  //              image->getImageData()->GetScalarType() == VTK_SHORT ||
  //              image->getImageData()->GetScalarType() == VTK_CHAR ||
  //              image->getImageData()->GetScalarType() == VTK_SIGNED_CHAR ||
  //              image->getImageData()->GetScalarType() == VTK_UNSIGNED_CHAR ||
  //              image->getImageData()->GetScalarType() == VTK_UNSIGNED_SHORT
  //              || image->getImageData()->GetScalarType() == VTK_UNSIGNED_INT)
  //     {
  //       // Data type is VTK_INT or similar, so calculate window/level
  //       // check the scalar type, bimodal analysis only works on int

  //       // Setup filter to work with signed 16-bit integer.
  //       int extent[6] = {0, 65535, 0, 0, 0, 0};
  //       accumulate->SetComponentExtent(extent);
  //       double origin[3] = {-32768, 0, 0};
  //       accumulate->SetComponentOrigin(origin);
  //       accumulate->SetInputData(image->getImageData());
  //       bimodal->SetInputConnection(accumulate->GetOutputPort());
  //       bimodal->Update();
  //       // Workaround for image data where all accumulate samples fall
  //       // within the same histogram bin
  //       if (bimodal->GetWindow() == 0.0 && bimodal->GetLevel() == 0.0)
  //       {
  //         needAdHoc = 1;
  //       }
  //       else
  //       {
  //         window = bimodal->GetWindow();
  //         level = bimodal->GetLevel();
  //         // lower = bimodal->GetThreshold();
  //         // upper = bimodal->GetMax();
  //       }
  //     }
  //     else if (image->getImageData()->GetScalarType() == VTK_FLOAT ||
  //              image->getImageData()->GetScalarType() == VTK_DOUBLE ||
  //              image->getImageData()->GetScalarType() == VTK_LONG ||
  //              image->getImageData()->GetScalarType() == VTK_UNSIGNED_LONG)
  //     {
  //       //// If scalar range is expected to be less conventional, then scale
  //       the
  //       ///bins / the image accumulate algorithm
  //       // double range[2] = {0, 0};
  //       ////image->getImageData()->GetPointData()->GetScalars()->GetRange(range);

  //       // long minInt = trunc(range[0]) - 1;
  //       // long maxInt = trunc(range[1]) + 1;

  //       // accumulate->SetInputData(image->getImageData());
  //       // int extent[6] = { 0, 999, 0, 0, 0, 0 };
  //       // accumulate->SetComponentExtent(extent);
  //       // double origin[3] = { static_cast<double>(minInt), 0.0, 0.0 };
  //       // accumulate->SetComponentOrigin(origin);
  //       // double spacing[3] = { (maxInt - minInt) / 1000.0, 1.0, 1.0 };
  //       // accumulate->SetComponentSpacing(spacing);

  //       // bimodal->SetInputConnection(accumulate->GetOutputPort());
  //       // bimodal->Update();

  //       //// The bimodal analysis assumes that the bin indices correspond
  //       directly
  //       ///to / voxel intensity values, need to convert back to intensity
  //       space
  //       // window = bimodal->GetWindow() * spacing[0];
  //       // level = minInt + (bimodal->GetLevel() - minInt) * spacing[0];
  //       ////lower = minInt + (bimodal->GetThreshold() - minInt) * spacing[0];
  //       ////upper = minInt + (bimodal->GetMax() - minInt) * spacing[0];
  //     }
  //     // else
  //     //{
  //     //	// If unhandled type, estimate with ad hoc method
  //     //	needAdHoc = 1;
  //     // }

  //     if (needAdHoc)
  //     {
  //       qDebug()
  //           << ("CalculateScalarAutoLevels: image data scalar type is not "
  //               "integer,"
  //               " doing ad hoc calc of window/level.");
  //       double range[2];
  //       image->getImageData()->GetPointData()->GetScalars()->GetRange(range);

  //       double min = range[0];
  //       double max = range[1];

  //       window = max - min;
  //       level = 0.5 * (max + min);
  //       // lower = level;
  //       // upper = range[1];
  //     }

  //     image->setWindow(window);
  //     image->setLevel(level);

  //     bimodal->Delete();
  //     accumulate->Delete();

  //     // Initialize Volume////
  //     SCBVolume* volume = new SCBVolume;
  //     volume->setRenderDataSet(image);
  //     // volume->setPresetToCBCTDental();
  //     scene->addData(volume);
  //     this->mainWindow->getViewer(3)->AddProp(volume);
  //     this->mainWindow->getViewer(3)->SetOrientationToAxial();
  //   }

  //   this->mainWindow->action_Navigation_Mode->trigger();
  //   this->styles[0]->GetNavigation()->CentralizeCursorPosition();
  //   this->styles[1]->GetNavigation()->CentralizeCursorPosition();
  //   this->styles[2]->GetNavigation()->CentralizeCursorPosition();
  //   // this->mainWindow->action_Dental_Planning->trigger();
  //   // this->styles[3]->GetNavigation()->CentralizeCursorPosition();

  //   this->mainWindow->getViewer(0)->ResetCamera(0);
  //   this->mainWindow->getViewer(0)->ResetCameraClippingRange(0);
  //   this->mainWindow->getViewer(0)->GetCursorActor()->VisibilityOn();
  //   this->mainWindow->getViewer(1)->ResetCamera(0);
  //   this->mainWindow->getViewer(1)->ResetCameraClippingRange(0);
  //   this->mainWindow->getViewer(1)->GetCursorActor()->VisibilityOn();
  //   this->mainWindow->getViewer(2)->ResetCamera(0);
  //   this->mainWindow->getViewer(2)->ResetCameraClippingRange(0);
  //   this->mainWindow->getViewer(2)->GetCursorActor()->VisibilityOn();
  //   this->mainWindow->getViewer(3)->ResetCamera(0);
  //   this->mainWindow->getViewer(3)->ResetCameraClippingRange(0);
  //   this->mainWindow->getViewer(3)->GetCursorActor()->VisibilityOn();
  //   SCBAbstractViewer::RenderAllViewers();
}

void SCBDentalHub::slotInitializationParanomicView()
{
  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   SCBImage* image = new SCBImage;
  //   image->setAbsolutePath(
  //       QStringList() << QString()
  //                            .append(QCoreApplication::applicationDirPath())
  //                            .append("/HardCode/HardCodeParanomicView.nii.gz"));

  //   image->readData();
  //   // SCBVolume* volume = new SCBVolume;
  //   // volume->setRenderDataSet(image);
  //   // scene->addData(volume);
  //   // this->mainWindow->getViewer(3)->AddProp(volume);
  //   // this->mainWindow->getViewer(3)->SetOrientationToCoronal();
  //   // double center[3];
  //   itk::Image<short, 3>::Pointer img = itk::Image<short, 3>::New();
  //   itk::Size<3> size;
  //   image->getITKImageData<short>(img);

  //   //////////////////////////////////////////////////////////////////////////////////////////
  //   // Diret copy from above initialization
  //   ////////////Initialize Image Window Level////////////
  //   vtkImageAccumulate* accumulate = vtkImageAccumulate::New();
  //   vtkImageBimodalAnalysis* bimodal = vtkImageBimodalAnalysis::New();
  //   bool needAdHoc = 0;
  //   int window = 0;
  //   int level = 0;

  //   if (image->getImageData()->GetNumberOfScalarComponents() >= 3)
  //   {
  //     needAdHoc = 1;
  //   }
  //   else if (image->getImageData()->GetScalarType() == VTK_INT ||
  //            image->getImageData()->GetScalarType() == VTK_SHORT ||
  //            image->getImageData()->GetScalarType() == VTK_CHAR ||
  //            image->getImageData()->GetScalarType() == VTK_SIGNED_CHAR ||
  //            image->getImageData()->GetScalarType() == VTK_UNSIGNED_CHAR ||
  //            image->getImageData()->GetScalarType() == VTK_UNSIGNED_SHORT ||
  //            image->getImageData()->GetScalarType() == VTK_UNSIGNED_INT)
  //   {
  //     // Data type is VTK_INT or similar, so calculate window/level
  //     // check the scalar type, bimodal analysis only works on int

  //     // Setup filter to work with signed 16-bit integer.
  //     int extent[6] = {0, 65535, 0, 0, 0, 0};
  //     accumulate->SetComponentExtent(extent);
  //     double origin[3] = {-32768, 0, 0};
  //     accumulate->SetComponentOrigin(origin);
  //     accumulate->SetInputData(image->getImageData());
  //     bimodal->SetInputConnection(accumulate->GetOutputPort());
  //     bimodal->Update();
  //     // Workaround for image data where all accumulate samples fall
  //     // within the same histogram bin
  //     if (bimodal->GetWindow() == 0.0 && bimodal->GetLevel() == 0.0)
  //     {
  //       needAdHoc = 1;
  //     }
  //     else
  //     {
  //       window = bimodal->GetWindow();
  //       level = bimodal->GetLevel();
  //       // lower = bimodal->GetThreshold();
  //       // upper = bimodal->GetMax();
  //     }
  //   }
  //   else if (image->getImageData()->GetScalarType() == VTK_FLOAT ||
  //            image->getImageData()->GetScalarType() == VTK_DOUBLE ||
  //            image->getImageData()->GetScalarType() == VTK_LONG ||
  //            image->getImageData()->GetScalarType() == VTK_UNSIGNED_LONG)
  //   {
  //     // If scalar range is expected to be less conventional, then scale the
  //     bins
  //     // the image accumulate algorithm
  //     double range[2];
  //     image->getImageData()->GetPointData()->GetScalars()->GetRange(range);

  //     long minInt = trunc(range[0]) - 1;
  //     long maxInt = trunc(range[1]) + 1;

  //     accumulate->SetInputData(image->getImageData());
  //     int extent[6] = {0, 999, 0, 0, 0, 0};
  //     accumulate->SetComponentExtent(extent);
  //     double origin[3] = {static_cast<double>(minInt), 0.0, 0.0};
  //     accumulate->SetComponentOrigin(origin);
  //     double spacing[3] = {(maxInt - minInt) / 1000.0, 1.0, 1.0};
  //     accumulate->SetComponentSpacing(spacing);

  //     bimodal->SetInputConnection(accumulate->GetOutputPort());
  //     bimodal->Update();

  //     // The bimodal analysis assumes that the bin indices correspond
  //     directly to
  //     // voxel intensity values, need to convert back to intensity space
  //     window = bimodal->GetWindow() * spacing[0];
  //     level = minInt + (bimodal->GetLevel() - minInt) * spacing[0];
  //     // lower = minInt + (bimodal->GetThreshold() - minInt) * spacing[0];
  //     // upper = minInt + (bimodal->GetMax() - minInt) * spacing[0];
  //   }
  //   // else
  //   //{
  //   //	// If unhandled type, estimate with ad hoc method
  //   //	needAdHoc = 1;
  //   // }

  //   if (needAdHoc)
  //   {
  //     qDebug()
  //         << ("CalculateScalarAutoLevels: image data scalar type is not
  //         integer,"
  //             " doing ad hoc calc of window/level.");
  //     double range[2];
  //     image->getImageData()->GetPointData()->GetScalars()->GetRange(range);

  //     double min = range[0];
  //     double max = range[1];

  //     window = max - min;
  //     level = 0.5 * (max + min);
  //     // lower = level;
  //     // upper = range[1];
  //   }

  //   image->setWindow(window);
  //   image->setLevel(level);

  //   bimodal->Delete();
  //   accumulate->Delete();
  //   ///////////////////////////////////////////////////////////////////////////////////////////

  //   double center[3];
  //   for (int i = 0; i < 3; i++)
  //   {
  //     center[i] =
  //         img->GetOrigin()[i] +
  //         (img->GetBufferedRegion().GetSize()[i] / 2) * img->GetSpacing()[i];
  //   }
  //   double rangez =
  //       (img->GetBufferedRegion().GetSize()[2]) * img->GetSpacing()[2];
  //   // qDebug() << center;
  //   for (int i = 0; i < 10; i++)
  //   {
  //     // todo: add delete to the locally created thing
  //     SCBImageSlice* imageSlice = new SCBImageSlice;
  //     imageSlice->setRenderDataSet(image);
  //     imageSlice->setPlanarNormal(0, 0, -1);
  //     imageSlice->setPlanarOrigin(
  //         center[0], center[1], center[2] + 0.1 * rangez * (-1 + 2 * i
  //         / 10.0));
  //     // imageSlice->propMatrixUpdate();
  //     scene->addData(imageSlice);
  //     this->mainWindow->panoramicSagitalViewers[i]->SetCursorDesyncFlag(true);
  //     this->mainWindow->panoramicSagitalViewers[i]->SetCursorPosition(
  //         center[0], center[1], center[2] + 0.1 * rangez * (-1 + 2 * i
  //         / 10.0));
  //     this->mainWindow->panoramicSagitalViewers[i]->AddProp(imageSlice);
  //     this->mainWindow->panoramicSagitalViewers[i]->ResetCamera(0);
  //     this->mainWindow->panoramicSagitalViewers[i]->ResetCameraClippingRange(0);
  //     this->mainWindow->panoramicSagitalViewers[i]->GetActiveCamera()->Zoom(
  //         2);  // todo: non-hardcoded zoom value
  //     this->mainWindow->panoramicSagitalViewers[i]->UpdateAxes(false);
  //   }
  //   double bd[6];
  //   image->getImageData()->GetBounds(bd);
  //   SCBImageSlice* aimageSlice = new SCBImageSlice;
  //   aimageSlice->setRenderDataSet(image);
  //   // aimageSlice->setDisplayRegion(bd);
  //   aimageSlice->setPlanarNormal(1, 0, 0);
  //   aimageSlice->setPlanarOrigin(center[0], center[1], center[2]);
  //   vtkImageResliceMapper::SafeDownCast(aimageSlice->getImageActor()->GetMapper())
  //       ->SetSlabThickness(20);
  //   vtkImageResliceMapper::SafeDownCast(aimageSlice->getImageActor()->GetMapper())
  //       ->SetSlabTypeToMean();

  //   scene->addData(aimageSlice);
  //   this->mainWindow->panoramicViewer->SetCursorDesyncFlag(true);
  //   this->mainWindow->panoramicViewer->SetCursorPosition(
  //       center[0], center[1], center[2]);
  //   this->mainWindow->panoramicViewer->AddProp(aimageSlice);
  //   this->mainWindow->panoramicViewer->GetActiveCamera()
  //       ->UseHorizontalViewAngleOn();
  //   this->mainWindow->panoramicViewer->ResetCamera(0);
  //   this->mainWindow->panoramicViewer->ResetCameraClippingRange(0);
  //   // this->mainWindow->panoramicViewer->SetAxialViewPlaneNormal(0,1,0);
  //   this->mainWindow->panoramicViewer->SetOrientationToSagital();
  //   this->mainWindow->panoramicViewer->GetActiveCamera()->SetViewUp(0, 1, 0);

  //   SCBAbstractViewer::RenderAllViewers();
}