#include "SCBDentalRegistrationPipeline.h"

// #include "SCBImage.h"
// #include "SCBImageSlice.h"
// #include "SCBAbstractPlanarProp.h"
// #include "vtkPCA_ICP_Transform.h"
// #include "vtkAluminiumMarkerPolyDataExtractionFilter.h"
// #include "SCBScene.h"
// #include "SCBPolyData.h"
// #include "SCBMicronTrackerThread.h"
// #include "SCBMicrontracker.h"
// #include "TrackerRobotRegistration.h"
// //#include "SCBInteractorStyleNavigation.h"
// #include "SCBDentalHub.h"
// #include "SCBDentalMainWindow.h"
// #include "SCBOrthogonalViewer.h"
// #include "URDashBoard.h"
// #include "PoseIndicatorViewer.h"
// #include "SCBPolyDataActor.h"
// #include "vtkLandmarkTransformRMS.h"

// vtk
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>

// qt
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QProgressDialog>

const QString SCBDentalRegistrationPipeline::DIR("MARKERS");
const char* robotArmName = "kqjxb-1";
const char* calibrationToolName = "CalibrationTool2";

SCBDentalRegistrationPipeline::SCBDentalRegistrationPipeline(QWidget* parent) :
    QWidget(parent)
{
  this->setupUi(this);

  this->m_trackerInImageMatrix = vtkMatrix4x4::New();
  this->m_trackerInRobotMatrix = vtkMatrix4x4::New();
  //   this->m_registrationTool = new SCBPolyData;

  QObject::connect(this->pushButtonTrackerInImageRegistration,
                   &QPushButton::clicked,
                   this,
                   &SCBDentalRegistrationPipeline::UpdatePCA_ICP_Transform);

  connect(this,
          SIGNAL(trackerInImageRMSUpdated(double)),
          this->doubleSpinBoxTrackerInImage,
          SLOT(setValue(double)));
  connect(this->pushButtonCalibrateOrientation,
          &QPushButton::clicked,
          this,
          &SCBDentalRegistrationPipeline::slotCalibrateOrientation);
  connect(this->comboBoxToothReference,
          &QComboBox::currentTextChanged,
          this,
          &SCBDentalRegistrationPipeline::slotReferenceChanged);

  //   this->m_trackerThread = SCBMicronTrackerThread::New();
  //   SCBMicronTracker* tracker = this->m_trackerThread->getTracker();

  //   this->m_referenceName = "TeethRef";
  //   this->m_toolName = robotArmName;
  //   this->m_registrationToolName = ":/Tool/Tool/teethRef.vtk";

  //   if (!(tracker->isConnected()))
  //   {
  //     qCritical() << "No Tracker";
  //     return;
  //   }
  //   int setMarkerFolderPathErrorCode = tracker->setMarkerFolderPath(
  //       (QCoreApplication::applicationDirPath() + '/' + DIR)
  //           .toLocal8Bit()
  //           .data());
  //   if (setMarkerFolderPathErrorCode)
  //   {
  //     qCritical() << "Set marker folder path failed. "
  //                 << "Error code is " << setMarkerFolderPathErrorCode;
  //     return;
  //   }

  //   connect(this->m_trackerThread->getTracker(),
  //           &SCBMicronTracker::signalCalibrateSampleNumber,
  //           this,
  //           &SCBDentalRegistrationPipeline::slotUpdateUI);
}

SCBDentalRegistrationPipeline::~SCBDentalRegistrationPipeline()
{
  this->m_trackerInImageMatrix->Delete();
  //   delete this->m_registrationTool;
}

void SCBDentalRegistrationPipeline::UpdatePCA_ICP_Transform()
{
  //   ///////////////////
  //   URDashThread::New()->SetTcpConfig(
  //       0.0014067, -0.0045379, 0.2977823, 1.59028, 0.782185, 1.42545);
  //   ////////////////
  //   this->pushButtonTrackerInImageRegistration->setEnabled(false);
  //   emit SCBDentalHub::self->signalLoading(true);

  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   if (scene->getAllDataByTag("Image").isEmpty() == true)
  //   {
  //     this->pushButtonTrackerInImageRegistration->setEnabled(true);
  //     qCritical() << "No Image";
  //     emit SCBDentalHub::self->signalLoading(false);
  //     return;
  //   }

  //   QString imagePath = scene->getAllDataByTag("Image").first();
  //   SCBImage* image =
  //       qobject_cast<SCBImage*>(scene->getDataByUniqueName(imagePath));

  //   if (!image)
  //   {
  //     this->pushButtonTrackerInImageRegistration->setEnabled(true);
  //     qCritical() << "No Image";
  //     emit SCBDentalHub::self->signalLoading(false);
  //     return;
  //   }

  //   this->m_registrationTool->setAbsolutePath(QStringList()
  //                                             << m_registrationToolName);
  //   this->m_registrationTool->readData();

  //   vtkSmartPointer<vtkAluminiumMarkerPolyDataExtractionFilter>
  //       aluminiumMarkerPolyDataFilter =
  //           vtkSmartPointer<vtkAluminiumMarkerPolyDataExtractionFilter>::New();

  //   aluminiumMarkerPolyDataFilter->SetInputConnection(
  //       image->getTransformOutputPort());
  //   aluminiumMarkerPolyDataFilter->SetTargetVolume(3134);
  //   aluminiumMarkerPolyDataFilter->SetVolumeTolerance(1200);
  //   aluminiumMarkerPolyDataFilter->Update();

  //   emit SCBDentalHub::self->signalSetLoadingValue(93);

  //   vtkSmartPointer<vtkPCA_ICP_Transform> autoICPTransform =
  //       vtkSmartPointer<vtkPCA_ICP_Transform>::New();

  //   autoICPTransform->SetTarget(aluminiumMarkerPolyDataFilter->GetOutput());
  //   autoICPTransform->SetSource(this->m_registrationTool->getTransformPolyData());
  //   autoICPTransform->GetLandmarkTransform()->SetModeToRigidBody();
  //   autoICPTransform->SetMaximumNumberOfIterations(50);
  //   autoICPTransform->Update();
  //   autoICPTransform->GetMatrix(this->m_trackerInImageMatrix);

  //   // this->EnableReference(true);
  //   //  try to find whether it exists.
  //   // int errorCode =
  //   this->m_trackerThread->getRawTransformMatrix("TeethRef",
  //   // vtkSmartPointer<vtkMatrix4x4>::New()); int errorCode =
  //   // this->m_trackerThread->getRawTransformMatrix("R-Dental",
  //   // vtkSmartPointer<vtkMatrix4x4>::New());
  //   this->m_trackerThread->setToolMarker(m_toolName);
  //   this->m_trackerThread->setReferenceMarker(m_referenceName);
  //   QString referenceName = this->m_trackerThread->getReferenceMarker();

  //   int errorCode = m_trackerThread->getTracker()->getRawTransformMatrix(
  //       referenceName.toLocal8Bit().data(),
  //       vtkSmartPointer<vtkMatrix4x4>::New());
  //   switch (errorCode)
  //   {
  //     case -1:
  //       qWarning() << "Try to find " << referenceName
  //                  << ", but it cannot be found. ";
  //       qWarning() << "-1: Wrong ID/Key input. ";
  //       break;
  //     case 1:
  //       qWarning() << "Try to find " << referenceName
  //                  << ", but it cannot be found. ";
  //       qWarning() << "1 : No markers detected.";
  //       break;
  //     case 2:
  //       qWarning() << "Try to find " << referenceName
  //                  << ", but it cannot be found. ";
  //       qWarning() << "2 : Error in get marker to camera Xform.";
  //       break;
  //     case 3:
  //       qWarning() << "Try to find " << referenceName
  //                  << ", but it cannot be found. ";
  //       qWarning() << "3 : Specified tool not found.";
  //       break;
  //     case 0:
  //     default:
  //       break;
  //   }
  //   this->pushButtonTrackerInImageRegistration->setEnabled(true);
  //   emit this->trackerInImageRMSUpdated(autoICPTransform->GetRMS());
  //   emit SCBDentalHub::self->signalLoading(false);
}

void SCBDentalRegistrationPipeline::UpdateTrackerRobotRegistration()
{
#ifdef ROBOT_API_IS_URApi
  // this->pushButtonTrackerInRobotRegistration->setEnabled(false);
  // TrackerRobotRegistration
  // trackerRobotRegistration(SCBInteractorStyleMicronTrackerRobotNavigation2::GetDashBoard(),
  // this->m_trackerThread->getTracker());
  // trackerRobotRegistration.runRegistration(0);
  // trackerRobotRegistration.getTransformMatrix(this->m_trackerInRobotMatrix);
  // emit trackerInRobotRMSUpdated(trackerRobotRegistration.getRMS());
  // this->pushButtonTrackerInRobotRegistration->setEnabled(true);
#endif
  // if (this->pushButtonTrackerInRobotRegistration->isChecked() == true) {

  //	this->pushButtonTrackerInRobotRegistration->setEnabled(true);
  //	//URDashThread::New()->SetTcpConfig(-0.00545, -0.00803,
  // 0.25529, 1.52434, 0.748582, 1.16111);
  //	//URDashThread::New()->SetTcpConfig(0.0026197, 0.0183053,
  // 0.3046826, 1.59028, 0.782185, 1.42545);
  //	URDashThread::New()->SetTcpConfig(0.0014067, -0.0045379,
  // 0.2977823, 1.59028, 0.782185, 1.42545);
  //	//URDashThread::New()->SetTcpConfig(0.0014067, -0.0045379, 0.2977823,
  // 0,0,0);

  //	if (true) {
  //		QTime dieTime = QTime::currentTime().addSecs(1);
  //		while (QTime::currentTime() < dieTime)
  //			QCoreApplication::processEvents(QEventLoop::AllEvents,
  // 16);
  //	}
  //	TrackerRobotRegistration trackerRobotRegistration(URDashThread::New(),
  // this->m_trackerThread->getTracker());
  //	trackerRobotRegistration.runRegistration(0, 1);
  //	trackerRobotRegistration.getTransformMatrix(this->m_trackerInRobotMatrix);
  //	emit trackerInRobotRMSUpdated(trackerRobotRegistration.getRMS());
  //	this->pushButtonTrackerInRobotRegistration->setEnabled(true);
  //}
}

void SCBDentalRegistrationPipeline::slotReferenceChanged(const QString& text)
{
  Q_UNUSED(text)
  //   this->m_trackerThread->setReferenceMarker(text);
  //   if (text.compare("TeethRef") == 0)
  //   {
  //     this->m_registrationToolName = ":/Tool/Tool/teethRef.vtk";
  //     this->m_referenceName = "TeethRef";
  //   }

  //   if (text.compare("R-Dental") == 0)
  //   {
  //     this->m_registrationToolName = ":/Tool/Tool/R-Dental-1.stl";
  //     this->m_referenceName = "R-Dental";
  //   }

  //   if (text.compare("L-Dental") == 0)
  //   {
  //     this->m_registrationToolName = ":/Tool/Tool/L-Dental-1.stl";
  //     this->m_referenceName = "L-Dental";
  //   }
}

void SCBDentalRegistrationPipeline::slotUpdateUI(int val)
{
  Q_UNUSED(val)
  QCoreApplication::processEvents();
}

void SCBDentalRegistrationPipeline::slotCalibrateOrientation() {}

void SCBDentalRegistrationPipeline::slotCalibratePosition()
{
  //   double RMS = 0;
  //   SCBMicronTracker* tracker = this->m_trackerThread->getTracker();
  //   int result = tracker->calibrateGetShiftNew(
  //       calibrationToolName, robotArmName, 200, RMS);

  //   if (result == 0)
  //   {
  //     int saveResult = tracker->saveCalibratedTemplate(robotArmName);
  //     // this->progressBarCalibration->setValue(100);
  //     if (saveResult == 0)
  //     {
  //       // this->doubleSpinBoxCalibrationPositionRMS->setValue(RMS);
  //     }
  //   }
}

void SCBDentalRegistrationPipeline::slotConfirmCalibration() {}
