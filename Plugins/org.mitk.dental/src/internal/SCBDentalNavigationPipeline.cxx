#include "SCBDentalNavigationPipeline.h"
// #include "SCBDentalHub.h"
// #include "SCBDentalMainWindow.h"
// #include "SCBDentalRegistrationPipeline.h"
// #include "PoseIndicatorViewer.h"

// #include "SCBPolyData.h"
// #include "SCBAbstractPlanarProp.h"
// #include "SCBPolyDataActor.h"
// #include "SCBOrthogonalViewer.h"

// #include "TrackerRobotRegistration.h"
// qt
#include <QMessageBox>
#include <QProgressDialog>

// std
#include <iostream>

// #include "URCommon.h"
// #include "URDashBoard.h"

// char* robotArmName = "kqjxb-1";

SCBDentalNavigationPipeline::SCBDentalNavigationPipeline(QWidget* parent) :
    QWidget(parent)
{
  this->setupUi(this);
  // this->m_trackerThread = SCBMicronTrackerThread::New();
  // this->m_robot = URDashThread::New();
  // checktimer.setInterval(100);
  // checktimer.start();
  // m_quitDrill = false;

  // this->m_tool = new SCBPolyData;
  // this->m_tool->setColor(1, 1, 0);
  // this->m_tool->setAbsolutePath(
  //     QStringList()
  //     << ":/TrackerToolRepresentationMeshes/TrackerToolRepresentationMesh/"
  //        "RoboticArmFixtureVisualization.stl");
  // this->m_tool->readData();

  this->pushButtonMoveToTargetImplant->setEnabled(false);
  this->pushButtonStartDrilling->setEnabled(false);
  this->pushButtonNavigationConfirm->setEnabled(false);
  connect(this->pushButtonMoveToTargetImplant,
          SIGNAL(clicked()),
          this,
          SLOT(MoveToImplant()));
  // connect(this->pushButtonResetRoboticArm, SIGNAL(clicked()), this,
  // SLOT(ResetRobot())); connect(this->pushButtonEmergencyStop,
  // SIGNAL(clicked()), this, SLOT(Stop()));
  connect(this->pushButtonCalibratePosition,
          SIGNAL(clicked()),
          this,
          SLOT(CalibrateNewTooltip()));
  connect(this->pushButtonStartDrilling,
          SIGNAL(clicked()),
          this,
          SLOT(DrillMode()));
  connect(this->pushButtonNavigationConfirm,
          SIGNAL(clicked()),
          this,
          SLOT(Finished()));
  connect(&checktimer, SIGNAL(timeout()), this, SLOT(FreedriveCheck()));
  connect(
      this->comboBoxImplant,
      static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::activated),
      [=](const QString& text) { this->setCurrentImplant(text); });
  // connect(SCBScene::getCurrentScene(),
  //         &SCBScene::signalAddedData,
  //         this,
  //         &SCBDentalNavigationPipeline::addImplant);
}

SCBDentalNavigationPipeline::~SCBDentalNavigationPipeline()
{
  // delete this->m_tool;
}

int SCBDentalNavigationPipeline::MoveToImplant()
{
  QMessageBox warning;
  warning.setWindowTitle(tr("Freedrive requested"));
  warning.setInformativeText(
      tr("Please freedrive to patient's mouth and verify the tool."));
  warning.setStandardButtons(QMessageBox::Yes);
  warning.addButton(QMessageBox::Cancel);
  // warning.button(QMessageBox::Yes)->setEnabled(true);
  warning.button(QMessageBox::Yes)->setText(tr("Confirm"));
  // warning.button(QMessageBox::Cancel)->setEnabled(true);
  warning.show();
  int warningresult = warning.exec();
  if (warningresult == QMessageBox::Cancel)
  {
    return 5;
  }
  else if (warningresult != QMessageBox::Yes)
  {
    return 6;
  }

  // SCBScene* scene = SCBScene::getCurrentScene();
  // SCBDentalFixture* implant =
  //     scene->getDataByUniqueName<SCBDentalFixture>(this->m_currentImplant);
  // if (!implant)
  // {
  //   qCritical() << "Cannot find implant whose unique name is" +
  //                      this->m_currentImplant;
  //   return -2;
  // }
  // RoboticOrientationCalculator* calculator = new
  // RoboticOrientationCalculator; QFuture<int> t1; for (int i = 0; i < 5; i++)
  // {
  //   vtkSmartPointer<vtkMatrix4x4> imageInRobotMatrix =
  //       vtkSmartPointer<vtkMatrix4x4>::New();

  //   vtkSmartPointer<vtkMatrix4x4> trackerInRobotMatrix =
  //       vtkSmartPointer<vtkMatrix4x4>::New();

  //   TrackerRobotRegistration trackerRobotRegistration(
  //       URDashThread::New(), this->m_trackerThread->getTracker());
  //   trackerRobotRegistration.runRegistration(0, 1);
  //   trackerRobotRegistration.getTransformMatrix(trackerInRobotMatrix);

  //   ////Set PoseIndicator
  //   SCBDentalHub::self->mainWindow->getPoseIndicatorViewer()->setNailLength(
  //       implant->getDentalFixtureLength());
  //   SCBDentalHub::self->mainWindow->getPoseIndicatorViewer()->setDepth(
  //       implant->getDentalFixtureLength());
  //   SCBDentalHub::self->mainWindow->getPoseIndicatorViewer()->setTargetPose(
  //       implant->getUserMatrix());

  //   // if using reference
  //   if (!m_trackerThread->getReferenceMarker().isEmpty())
  //   {
  //     vtkSmartPointer<vtkMatrix4x4> referenceInTracker =
  //         vtkSmartPointer<vtkMatrix4x4>::New();
  //     int errorCode =
  //         SCBMicronTrackerThread::New()->getTracker()->getRawTransformMatrix(
  //             1, referenceInTracker);
  //     switch (errorCode)
  //     {
  //       case -1:
  //         qCritical() << "Try to find "
  //                     << SCBMicronTrackerThread::New()->getReferenceMarker()
  //                     << ", but it cannot be found. ";
  //         qCritical() << "-1: Wrong ID/Key input. ";
  //         return errorCode;
  //       case 1:
  //         qCritical() << "Try to find "
  //                     << SCBMicronTrackerThread::New()->getReferenceMarker()
  //                     << ", but it cannot be found. ";
  //         qCritical() << "1 : No markers detected.";
  //         return errorCode;
  //       case 2:
  //         qCritical() << "Try to find "
  //                     << SCBMicronTrackerThread::New()->getReferenceMarker()
  //                     << ", but it cannot be found. ";
  //         qCritical() << "2 : Error in get marker to camera Xform.";
  //         return errorCode;
  //       case 3:
  //         qCritical() << "Try to find "
  //                     << SCBMicronTrackerThread::New()->getReferenceMarker()
  //                     << ", but it cannot be found. ";
  //         qCritical() << "3 : Specified tool not found.";
  //         return errorCode;
  //       case 0:
  //       default:
  //         break;
  //     }

  //     // cout << "trackerInRobot" << '\n' << *trackerInRobot;

  //     vtkSmartPointer<vtkTransform> transform =
  //         vtkSmartPointer<vtkTransform>::New();
  //     transform->Identity();
  //     transform->PostMultiply();
  //     transform->Concatenate(
  //         SCBDentalHub::self->mainWindow->getDentalRegistrationPipeline()
  //             ->m_trackerInImageMatrix);
  //     transform->Inverse();
  //     transform->Concatenate(referenceInTracker);
  //     transform->Concatenate(trackerInRobotMatrix);
  //     transform->Inverse();
  //     transform->GetMatrix(imageInRobotMatrix);
  //   }

  //   vtkSmartPointer<vtkTransform> urTransform =
  //       vtkSmartPointer<vtkTransform>::New();
  //   urTransform->Identity();
  //   urTransform->PreMultiply();
  //   urTransform->Concatenate(imageInRobotMatrix);
  //   urTransform->Inverse();
  //   urTransform->Concatenate(implant->getUserMatrix());

  //   ////Get the implant direction (-z-Axis) and compute the vector in image
  //   /// space
  //   double implantDirectionInSTLSpace[4] = {0, 0, -1, 0};
  //   double implantDirectionInImageSpace[4];
  //   vtkSmartPointer<vtkTransform> transform =
  //       vtkSmartPointer<vtkTransform>::New();
  //   transform->SetMatrix(implant->getUserMatrix());
  //   transform->MultiplyPoint(implantDirectionInSTLSpace,
  //                            implantDirectionInImageSpace);

  //   ////Transform implant from image space to robotic space
  //   double implantDirectionInRoboticSpace[4];

  //   vtkSmartPointer<vtkMatrix4x4> _mat =
  //   vtkSmartPointer<vtkMatrix4x4>::New(); _mat->DeepCopy(imageInRobotMatrix);
  //   _mat->Invert();
  //   _mat->MultiplyPoint(implantDirectionInImageSpace,
  //                       implantDirectionInRoboticSpace);

  //   Tool_rt goal;
  //   // goal.X = urTransform->GetMatrix()->GetElement(0, 3) * 0.001 - 0.0055 *
  //   // implantDirectionInRoboticSpace[0]; goal.Y =
  //   // urTransform->GetMatrix()->GetElement(1, 3) * 0.001 - 0.0055 *
  //   // implantDirectionInRoboticSpace[1]; goal.Z =
  //   // urTransform->GetMatrix()->GetElement(2, 3) * 0.001 - 0.0055 *
  //   // implantDirectionInRoboticSpace[2]; goal.Rx =
  //   // implantDirectionInRoboticSpace[0]; goal.Ry =
  //   // implantDirectionInRoboticSpace[1]; goal.Rz =
  //   // implantDirectionInRoboticSpace[2];
  //   double halfImplantLength = implant->getDentalFixtureLength() * 0.0005;
  //   double DesiredPosition[3], DesiredDirection[3];
  //   for (int i = 0; i < 3; i++)
  //   {
  //     DesiredPosition[i] = urTransform->GetMatrix()->GetElement(i, 3) * 0.001
  //     -
  //                          (halfImplantLength + (m_toolTipLength / 1000)) *
  //                              implantDirectionInRoboticSpace[i];
  //     DesiredDirection[i] = implantDirectionInRoboticSpace[i];
  //   }

  //   this->m_robot->SetRobotControlMode(AbstractRobot::RobotWorkMode::Normal);
  //   // Initialization
  //   if (this->m_robot->GetIsInMove())
  //   {
  //     this->m_robot->Brake();
  //     // this->m_robot->QuitFreeDrive();
  //     this->m_robot->SetRequestStop(true);
  //   }
  //   while (this->m_robot->GetIsInMove())
  //   {
  //     QTime dieTime = QTime::currentTime().addMSecs(16);
  //     while (QTime::currentTime() < dieTime)
  //       QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  //   }

  //   Tool_rt mimicpose = this->m_robot->GetCurrentTcp();
  //   double posdirfac[9];
  //   Toolrt2PosDirFac(mimicpose, posdirfac);
  //   for (int i = 0; i < 9; i++) std::cout << posdirfac[i] << " ";

  //   calculator->SetDirectionVector(DesiredDirection);
  //   calculator->SetUpDirection(posdirfac[6], posdirfac[7], posdirfac[8]);
  //   calculator->Update();
  //   double rv[3];
  //   calculator->GetRotationVector(rv);
  //   Tool_rt goalpose;
  //   goalpose.X = DesiredPosition[0];
  //   goalpose.Y = DesiredPosition[1];
  //   goalpose.Z = DesiredPosition[2];
  //   goalpose.Rx = rv[0];
  //   goalpose.Ry = rv[1];
  //   goalpose.Rz = rv[2];
  //   std::cout << "\n";
  //   goalpose.Print();
  //   double res[9];
  //   Toolrt2PosDirFac(goalpose, res);
  //   std::cout << "\n";
  //   for (int i = 0; i < 9; i++) std::cout << res[i] << " ";

  //   const double PI = vtkMath::Pi();

  //   if (i == 0)  // only run at the first time
  //   {
  //     t1 = QtConcurrent::run(
  //         this->m_robot, &AbstractRobot::Move2Target, goalpose, true);
  //   }
  //   else
  //   {
  //     t1 = QtConcurrent::run(
  //         this->m_robot, &AbstractRobot::Move2Target, goalpose, false);
  //   }

  //   // Experiment indicates a wait is needed to give time for the function to
  //   // get proceed signal and return the correct result of isInMove
  //   if (true)
  //   {
  //     QTime dieTime = QTime::currentTime().addMSecs(500);
  //     while (QTime::currentTime() < dieTime)
  //       QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  //   }

  //   while (this->m_robot->GetIsInMove() || t1.isFinished() == false)
  //   {
  //     QTime dieTime = QTime::currentTime().addMSecs(16);
  //     while (QTime::currentTime() < dieTime)
  //       QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  //   }
  // }
  // if (true)
  // {
  //   this->pushButtonStartDrilling->setEnabled(true);
  //   this->m_quitDrill = false;
  // }

  // delete (calculator);
  // return t1.result();
  return 1;
}

void SCBDentalNavigationPipeline::CalibrateNewTooltip()
{
  this->pushButtonMoveToTargetImplant->setEnabled(false);
  this->pushButtonStartDrilling->setEnabled(false);
  this->pushButtonNavigationConfirm->setEnabled(false);
  // todo: add code
  QProgressDialog progress(
      tr("Calibrating Tool Tip..."), QString(), 0, 100, this);
  progress.setWindowModality(Qt::WindowModal);
  progress.setFixedSize(500, 80);
  progress.setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
  progress.show();
  progress.setStyleSheet(
      "QProgressBar {      \
							border: 1px solid #292f5f;\
							color: #fff;\
							font-weight: bold;\
							background-color: #0c172a;\
							text-align: center;\
							}\n \
							QProgressBar::chunk{\
							background-color: #388fcb;\
							width: 3px;\
							margin: 0.5px;\
							}\n \
							QLabel{ color:white; background - color:transparent; }");
  // connect(this->m_trackerThread->getTracker(),
  //         &SCBMicronTracker::signalCalibrateSampleNumber,
  //         &progress,
  //         &QProgressDialog::setValue,
  //         Qt::UniqueConnection);
}

void SCBDentalNavigationPipeline::DrillMode()
{
  using namespace std;
  std::cout << "Start Drill Mode" << endl;

  if (this->pushButtonStartDrilling->isChecked() == true)
  {
    std::cout << "Check if" << endl;

    std::cout << "Drill mode start";
    // this->m_robot->SetRobotControlMode(AbstractRobot::RobotWorkMode::Drill);
    this->pushButtonNavigationConfirm->setEnabled(true);
    this->m_quitDrill = false;
  }
  else
  {
    std::cout << "Check else" << endl;
    this->Finished();
  }

  std::cout << "End Drill Mode" << endl;
}

void SCBDentalNavigationPipeline::FreedriveCheck()
{
  if (this->pushButtonStartDrilling->isEnabled() == false)
  {
    return;
  }
  // if (this->m_robot->GetDIFlag(0) || this->m_quitDrill)
  // {
  //   this->m_robot->SetRobotControlMode(AbstractRobot::RobotWorkMode::Normal);
  //   this->pushButtonStartDrilling->setEnabled(false);
  //   this->pushButtonMoveToTargetImplant->setEnabled(true);
  //   std::cout << "Drill mode end";
  // }
}

void SCBDentalNavigationPipeline::setCurrentImplant(QString index)
{
  this->m_currentImplant = index;
  // SCBScene* scene = SCBScene::getCurrentScene();
  // SCBDentalFixture* implant =
  //     scene->getDataByUniqueName<SCBDentalFixture>(this->m_currentImplant);
  // if (!implant)
  // {
  //   qCritical() << "Cannot find implant whose unique name is" +
  //                      this->m_currentImplant;
  //   return;
  // }
  // cout << "set target pose. \n";
  // cout << *implant->getUserMatrix();
  // SCBDentalHub::self->mainWindow->getPoseIndicatorViewer()->setTargetPose(
  //     implant->getUserMatrix());
}

void SCBDentalNavigationPipeline::Finished()
{
  this->m_quitDrill = true;
}

int SCBDentalNavigationPipeline::ResetRobot()
{
  // this->m_robot->SetRobotControlMode(AbstractRobot::RobotWorkMode::Normal);
  ////Initialization
  // if (this->m_robot->GetIsInMove()) {
  //	this->m_robot->Brake();
  // }
  ////this->m_robot->QuitFreeDrive();
  // this->m_robot->SetRequestStop(true);
  // while (this->m_robot->GetIsInMove()) {
  //	QTime dieTime = QTime::currentTime().addMSecs(16);
  //	while (QTime::currentTime() < dieTime)
  //		QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  // }
  // this->m_robot->SetRequestStop(false);
  // QFuture<int> t1 = QtConcurrent::run(this->m_robot, &AbstractRobot::Reset);

  ////Experiment indicates a wait is needed to give time for the function to get
  /// proceed signal and return the correct result of isInMove
  // if (true) {
  //	QTime dieTime = QTime::currentTime().addMSecs(53);
  //	while (QTime::currentTime() < dieTime)
  //		QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  // }

  // while (this->m_robot->GetIsInMove()) {
  //	QTime dieTime = QTime::currentTime().addMSecs(16);
  //	while (QTime::currentTime() < dieTime)
  //		QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  // }
  // t1.waitForFinished();
  // return t1.result();
  return 0;
}

int SCBDentalNavigationPipeline::Stop()
{
  // this->m_robot->SetRobotControlMode(AbstractRobot::RobotWorkMode::Normal);
  // this->m_robot->QuitFreeDrive();
  // this->m_robot->SetRequestStop(true);
  // QFuture<void> t1 = QtConcurrent::run(this->m_robot, &AbstractRobot::Brake);
  // while (this->m_robot->GetIsInMove())
  // {
  //   QTime dieTime = QTime::currentTime().addMSecs(16);
  //   while (QTime::currentTime() < dieTime)
  //     QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  // }
  return 0;
}

void SCBDentalNavigationPipeline::addImplant(QString uniqueName)
{
  Q_UNUSED(uniqueName);
  // SCBScene* scene = SCBScene::getCurrentScene();
  // SCBDentalFixture* implant =
  //     scene->getDataByUniqueName<SCBDentalFixture>(uniqueName);
  // if (!implant)
  // {
  //   return;
  // }
  // this->comboBoxImplant->addItem(uniqueName);
}

void SCBDentalNavigationPipeline::removeImplant(QString uniqueName)
{
  Q_UNUSED(uniqueName);
  // this->comboBoxImplant->addItem(uniqueName);
  // this->comboBoxImplant->indx
  // this->comboBoxImplant->removeItem();
}

void SCBDentalNavigationPipeline::startTracking()
{
  // connect(this->m_trackerThread,
  //         &SCBMicronTrackerThread::matrixUpdated,
  //         this,
  //         &SCBDentalNavigationPipeline::tracking,
  //         Qt::UniqueConnection);

  // // if (!this->m_trackerThread->getReferenceMarker().isEmpty()) {
  // int errorCode = this->m_trackerThread->getTracker()->getTransformMatrix(
  //     0, 1, vtkSmartPointer<vtkMatrix4x4>::New());
  // switch (errorCode)
  // {
  //   case 1:
  //     qWarning() << "1: Error in get marker to camera Xform.";
  //     break;
  //   case 2:
  //     qWarning() << "2: Both tool and reference are not found.";
  //     break;
  //   case 3:
  //     qWarning() << "3: Reference not found.";
  //     break;
  //   case 4:
  //     qWarning() << "4: Tool not found. ";
  //     break;
  //   default:
  //     break;
  // }

  // this->m_toolProp = new SCBPolyDataActor;
  // this->m_toolProp->setRenderDataSet(m_tool);
  // SCBDentalHub::self->mainWindow->getViewer(3)->AddProp(this->m_toolProp);
}

void SCBDentalNavigationPipeline::stopTracking()
{
  // disconnect(this->m_trackerThread,
  //            &SCBMicronTrackerThread::matrixUpdated,
  //            this,
  //            &SCBDentalNavigationPipeline::tracking);
  // SCBDentalHub::self->mainWindow->getViewer(3)->RemovePropA(this->m_toolProp);
  // delete this->m_toolProp;
}

void SCBDentalNavigationPipeline::tracking(vtkMatrix4x4* matrix)
{
  Q_UNUSED(matrix);
  // vtkTransform* transform = vtkTransform::New();
  // transform->SetMatrix(matrix);
  // transform->Translate(0, 0, m_toolTipLength);
  // transform->Update();
  // transform->PostMultiply();
  // transform->Concatenate(
  //     SCBDentalHub::self->mainWindow->getDentalRegistrationPipeline()
  //         ->m_trackerInImageMatrix);
  // transform->Update();
  // m_tool->getUserMatrix()->DeepCopy(transform->GetMatrix());
  // SCBDentalHub::self->mainWindow->getViewer(0)->SetCursorPosition(
  //     transform->GetMatrix()->GetElement(0, 3),
  //     transform->GetMatrix()->GetElement(1, 3),
  //     transform->GetMatrix()->GetElement(2, 3));
  // SCBDentalHub::self->mainWindow->getPoseIndicatorViewer()->setTrackerPosition(
  //     transform->GetMatrix());
  // // emit ToolMatrix(transform->GetMatrix());
  // transform->Delete();
}

bool RoboticOrientationCalculator::checksame(double* a, double* b)
{
  bool same = true;
  for (int i = 0; i < 3; i++)
  {
    if (a[i] != b[i])
      same = false;
  }
  return same;
}

int RoboticOrientationCalculator::checkorientation(double* a, double* b)
{
  if (a[0] != 0 && b[0] != 0)
  {
    if (a[0] / b[0] < 0)
      return -1;
    else
      return 1;
  }
  else if (a[1] != 0 && b[1] != 0)
  {
    if (a[1] / b[1] < 0)
      return -1;
    else
      return 1;
  }
  else if (a[2] != 0 && b[2] != 0)
  {
    if (a[2] / b[2] < 0)
      return -1;
    else
      return 1;
  }
  // else {
  //	std::cout << "Error determining sign\n";
  // }
  return 1;
}

RoboticOrientationCalculator::RoboticOrientationCalculator()
{
  this->m_direction = new double[3];
  this->m_up_direction = new double[3];
  this->m_output = new double[3];
  this->m_intermediateOutput = new double[4];
  // this->m_transform = vtkTransform::New();
  // this->m_allign = vtkTransform::New();
  // this->m_angular_correction = vtkTransform::New();
  // this->m_rotation_matrix = vtkMatrix3x3::New();
  // this->m_cachematrix = vtkMatrix4x4::New();
  // this->m_orthogonalplane = vtkPlane::New();
}

RoboticOrientationCalculator::~RoboticOrientationCalculator()
{
  delete (this->m_direction);
  delete (this->m_up_direction);
  delete (this->m_output);
  delete (this->m_intermediateOutput);
  // this->m_transform->Delete();
  // this->m_allign->Delete();
  // this->m_angular_correction->Delete();
  // this->m_rotation_matrix->Delete();
  // this->m_cachematrix->Delete();
  // this->m_orthogonalplane->Delete();
}

void RoboticOrientationCalculator::SetUpDirection(double a, double b, double c)
{
  this->m_up_direction[0] = a;
  this->m_up_direction[1] = b;
  this->m_up_direction[2] = c;
}

void RoboticOrientationCalculator::SetUpDirection(double* a)
{
  this->SetUpDirection(a[0], a[1], a[2]);
}

void RoboticOrientationCalculator::SetDirectionVector(double a,
                                                      double b,
                                                      double c)
{
  this->m_direction[0] = a;
  this->m_direction[1] = b;
  this->m_direction[2] = c;
}

void RoboticOrientationCalculator::SetDirectionVector(double* a)
{
  this->SetDirectionVector(a[0], a[1], a[2]);
}

void RoboticOrientationCalculator::Update()
{
  // // The following assumes a standard tcp_configuration
  // double zerozerozero_needle_direction[3] = {
  //     1, 0, 0};  // i.e. When Rx,Ry,Rz are set to 0, the pointing direction
  //     of
  //                // the needle
  // double initial_pointing_direction[3] = {
  //     0,
  //     1,
  //     0};  // i.e. When Rx,Ry,Rz are set to 0, the pointing direction of pin.

  // vtkMath::Normalize(this->m_direction);
  // vtkMath::Normalize(this->m_up_direction);

  // // find (1,0,0)(i,e, starting orientation of pin) to (this->m_direction)
  // // matrix, so to apply it on up_vector to find the rotation
  // this->m_direction
  // // from pin to needle first find the rotation matrix for allignment
  // this->m_rotation_matrix->Identity();
  // // Calculation of pin_to_needle_matrix
  // if (true)
  // {
  //   //
  //   https://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
  //   double a[3] = {1, 0, 0};
  //   double b[3];
  //   for (int i = 0; i < 3; i++) b[i] = this->m_direction[i];
  //   double v[3];
  //   vtkMath::Cross(a, b, v);
  //   double s = vtkMath::Norm(v);
  //   double c = vtkMath::Dot(a, b);
  //   vtkSmartPointer<vtkMatrix3x3> vx = vtkSmartPointer<vtkMatrix3x3>::New();
  //   vx->SetElement(0, 1, -v[2]);
  //   vx->SetElement(0, 2, v[1]);
  //   vx->SetElement(1, 0, v[2]);
  //   vx->SetElement(1, 2, -v[0]);
  //   vx->SetElement(2, 0, -v[1]);
  //   vx->SetElement(2, 1, v[0]);
  //   for (int i = 0; i < 3; i++) vx->SetElement(i, i, 0);
  //   vtkSmartPointer<vtkMatrix3x3> vx2 = vtkSmartPointer<vtkMatrix3x3>::New();
  //   vtkMatrix3x3::Multiply3x3(vx, vx, vx2);
  //   if (c == -1)
  //     std::cout << "error";
  //   double coeff = 1.0 / (1.0 + c);
  //   for (int i = 0; i < 3; i++)
  //   {
  //     for (int j = 0; j < 3; j++)
  //     {
  //       if (i == j)
  //         this->m_rotation_matrix->SetElement(
  //             i, j, 1 + vx->GetElement(i, j) + coeff * vx2->GetElement(i,
  //             j));
  //       else
  //         this->m_rotation_matrix->SetElement(
  //             i, j, vx->GetElement(i, j) + coeff * vx2->GetElement(i, j));
  //     }
  //   }
  // }

  // if (checksame(this->m_direction, this->m_up_direction))
  //   this->m_allign->Identity();
  // this->m_allign->Update();

  // // To make this->m_rotation_matrix become 4x4 matrix
  // this->m_cachematrix->Identity();
  // for (int i = 0; i < 3; i++)
  //   for (int j = 0; j < 3; j++)
  //     this->m_cachematrix->SetElement(
  //         i, j, this->m_rotation_matrix->GetElement(i, j));
  // this->m_allign->SetMatrix(this->m_cachematrix);

  // // For debug only
  // // this->m_rotation_matrix->MultiplyPoint(zerozerozero_needle_direction,
  // // blue_point); std::cout << blue_point[0] << " " << blue_point[1] << " "
  // <<
  // // blue_point[2] << "\n";

  // double present_orientation[3];  // The pointing this->m_direction of the
  // pin. double cache[3];                // To store intermediate answer double
  // target_orientation[3];  // The target pointing this->m_direction of the
  //                                // pin (Normalised)

  // this->m_allign->TransformPoint(initial_pointing_direction,
  //                                present_orientation);
  // // std::cout << present_orientation[0] << " " << present_orientation[1] <<
  // " "
  // // << present_orientation[2] << "\n";

  // this->m_orthogonalplane->SetNormal(m_direction);
  // this->m_orthogonalplane->SetOrigin(0, 0, 0);
  // this->m_orthogonalplane->ProjectVector(this->m_up_direction,
  //                                        target_orientation);

  // vtkMath::Normalize(present_orientation);
  // vtkMath::Normalize(target_orientation);

  // // std::cout << target_orientation[0] << " " << target_orientation[1] << "
  // "
  // // << target_orientation[2] << "\n";

  // double cos = vtkMath::Dot(present_orientation, target_orientation);
  // vtkMath::Cross(present_orientation, target_orientation, cache);
  // double sin = vtkMath::Norm(cache);
  // double angle = std::atan2(sin, cos);
  // angle *= 180 / vtkMath::Pi();
  // if (std::abs(cos) == 1)
  // {
  //   int rotation = checkorientation(present_orientation, target_orientation);
  //   if (rotation == 1)
  //     angle = 0;
  //   else
  //     angle = 180;
  // }
  // int orientation_correction = checkorientation(cache, this->m_direction);
  // angle *= orientation_correction;

  // // std::cout << angle << "!\n";
  // // std::cout << vtkMath::Dot(present_orientation,
  // target_orientation)<<"\n";

  // this->m_angular_correction->Identity();
  // this->m_angular_correction->RotateWXYZ(angle, this->m_direction);
  // this->m_angular_correction->Update();

  // this->m_transform->Identity();
  // this->m_transform->PostMultiply();
  // this->m_transform->Concatenate(this->m_allign);
  // this->m_transform->Concatenate(this->m_angular_correction);
  // this->m_transform->Update();

  // this->m_transform->GetOrientationWXYZ(this->m_intermediateOutput);

  // double length = this->m_intermediateOutput[0] * vtkMath::Pi() / 180.0;
  // for (int i = 1; i < 4; i++) this->m_intermediateOutput[i] *= length;

  // for (int i = 0; i < 3; i++)
  //   this->m_output[i] = this->m_intermediateOutput[i + 1];
}

void RoboticOrientationCalculator::GetRotationVector(double* a)
{
  for (int i = 0; i < 3; i++) a[i] = this->m_output[i];
}

double* RoboticOrientationCalculator::GetRotationVector()
{
  return this->m_output;
}

vtkTransform* RoboticOrientationCalculator::GetTransform()
{
  return this->m_transform;
}
