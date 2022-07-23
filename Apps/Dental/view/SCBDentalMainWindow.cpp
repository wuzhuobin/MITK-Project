#include "SCBDentalMainWindow.h"

#include "ui_SCBDentalMainWindow.h"
// me
// #include "PoseIndicatorViewer.h"
#include "IOController.h"
#include "SCBDentalClosingDialog.h"
#include "SCBDentalNavigationPipeline.h"
#include "SCBDentalPlanningPipeline.h"
#include "SCBDentalRegistrationPipeline.h"
#include "SCBDentalValidationPipeline.h"
// #include "SCBDocumentViewer.h"
// #include "SCBNonPlanarViewer.h"
// #include "SCBOrthogonalViewer.h"
// #include "SCBPlanarViewer.h"
// #include "SCBViewer.h"
// #include "SCBWelcomeWizard.h"
// #include "TrackerVolumeViewer.h"
// #include "URDashBoard.h"
// #include "ui_SCBOrthogonalViewer.h"

// vtk
#include <vtkRenderer.h>

// qt
#include <QActionGroup>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsBlurEffect>
#include <QGraphicsView>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>

static const struct SCBDentalMainWindowInit
{
  SCBDentalMainWindowInit() { Q_INIT_RESOURCE(Dental); }
} G_INIT;

SCBDentalMainWindow::SCBDentalMainWindow(QWidget* parent) :
    mUi(std::make_unique<Ui::SCBDentalMainWindow>()), QMainWindow(parent)
{
  mUi->setupUi(this);

  connect(IOController::getInstance(),
          &IOController::sceneLoaded,
          this,
          &SCBDentalMainWindow::onSceneLoaded);
  // viewers
  for (int i = 0; i < 3; ++i)
  {
    // this->planarViewers << new SCBPlanarViewer(this);
    // this->planarViewers.last()->GetUi()->pushBtnAxial->setVisible(false);
    // this->planarViewers.last()->GetUi()->pushBtnCoronal->setVisible(false);
    // this->planarViewers.last()->GetUi()->pushBtnSagital->setVisible(false);
    // this->planarViewers.last()->GetUi()->pushBtnMaximize->setVisible(false);
    // this->planarViewers.last()->GetUi()->pushBtnNormal->setVisible(false);
    // this->planarViewers.last()->GetUi()->pushBtnDropDownMenu->setVisible(false);
    // this->planarViewers.last()->SetEnableCornerAnnotation(false);
    // this->planarViewers.last()->GetCursorActor()->VisibilityOff();
    // this->planarViewers.last()->SetMaxNoOfPeelings(50);
    // this->planarViewers.last()->SetEnableDepthPeeling(true);
  }
  //   this->nonPlanarViewer = new SCBNonPlanarViewer(this);
  //   this->nonPlanarViewer->GetUi()->pushBtnAxial->setVisible(false);
  //   this->nonPlanarViewer->GetUi()->pushBtnCoronal->setVisible(false);
  //   this->nonPlanarViewer->GetUi()->pushBtnSagital->setVisible(false);
  //   this->nonPlanarViewer->GetUi()->pushBtnMaximize->setVisible(false);
  //   this->nonPlanarViewer->GetUi()->pushBtnNormal->setVisible(false);
  //   this->nonPlanarViewer->GetUi()->pushBtnDropDownMenu->setVisible(false);
  //   this->nonPlanarViewer->SetEnableCornerAnnotation(false);
  //   this->nonPlanarViewer->GetCursorActor()->VisibilityOff();
  //   this->nonPlanarViewer->SetMaxNoOfPeelings(50);
  //   this->nonPlanarViewer->SetEnableDepthPeeling(true);

  //   this->panoramicViewer = new SCBOrthogonalViewer(this);
  //   this->panoramicViewer->GetUi()->pushBtnAxial->setVisible(false);
  //   this->panoramicViewer->GetUi()->pushBtnCoronal->setVisible(false);
  //   this->panoramicViewer->GetUi()->pushBtnSagital->setVisible(false);
  //   this->panoramicViewer->GetUi()->pushBtnMaximize->setVisible(false);
  //   this->panoramicViewer->GetUi()->pushBtnNormal->setVisible(false);
  //   this->panoramicViewer->GetUi()->pushBtnDropDownMenu->setVisible(false);
  //   this->panoramicViewer->SetEnableCornerAnnotation(false);
  //   this->panoramicViewer->SetCursorDesyncFlag(true);
  for (int i = 0; i < 10; ++i)
  {
    // this->panoramicSagitalViewers << new SCBPlanarViewer(this);
    // this->panoramicSagitalViewers.last()->GetUi()->pushBtnAxial->setVisible(
    //     false);
    // this->panoramicSagitalViewers.last()->GetUi()->pushBtnCoronal->setVisible(
    //     false);
    // this->panoramicSagitalViewers.last()->GetUi()->pushBtnSagital->setVisible(
    //     false);
    // this->panoramicSagitalViewers.last()->GetUi()->pushBtnMaximize->setVisible(
    //     false);
    // this->panoramicSagitalViewers.last()->GetUi()->pushBtnNormal->setVisible(
    //     false);
    // this->panoramicSagitalViewers.last()
    //     ->GetUi()
    //     ->pushBtnDropDownMenu->setVisible(false);
    // this->panoramicSagitalViewers.last()->SetEnableCornerAnnotation(false);
    // this->panoramicSagitalViewers.last()->GetCursorActor()->VisibilityOff();
    // this->panoramicSagitalViewers.last()->SetMaxNoOfPeelings(50);
    // this->panoramicSagitalViewers.last()->SetEnableDepthPeeling(true);
    // this->panoramicSagitalViewers.last()->SetCursorDesyncFlag(true);
    // this->panoramicSagitalViewers.last()->UpdateAxes(false);
  }

  //   this->poseIndicatorViewer = new PoseIndicatorViewer(20, 2, 2, 6, this);

  // home button
  QMenu* menu = new QMenu(this);
  menu->addAction(mUi->action_Exit);
  mUi->pushButtonHome->setMenu(menu);

  // remove status bar
  mUi->dockWidgetMenuBar->setTitleBarWidget(new QWidget(this));
  mUi->dockWidgetFunctionPipeline->setTitleBarWidget(new QWidget(this));
  mUi->dockWidgetToolBar->setTitleBarWidget(new QWidget(this));
  // layout
  this->splitDockWidget(
      mUi->dockWidgetToolBar, mUi->dockWidgetFunctionPipeline, Qt::Horizontal);

  // adding pipeline widget (Neuro)
  this->DentalPlanningPipeline = new SCBDentalPlanningPipeline(this);
  mUi->stackedWidgetFunctionPipeline->addWidget(this->DentalPlanningPipeline);

  this->DentalRegistrationPipeline = new SCBDentalRegistrationPipeline(this);
  mUi->stackedWidgetFunctionPipeline->addWidget(
      this->DentalRegistrationPipeline);

  this->DentalNavigatePipeline = new SCBDentalNavigationPipeline(this);
  mUi->stackedWidgetFunctionPipeline->addWidget(this->DentalNavigatePipeline);

  this->DentalValidatePipeline = new SCBDentalValidationPipeline(this);
  mUi->stackedWidgetFunctionPipeline->addWidget(this->DentalValidatePipeline);
  // connect(this->DentalValidatePipeline->loadPushButton, SIGNAL(clicked()),
  // this, SLOT(slotWelcomeWizard()));

  closingdialog = new SCBDentalClosingDialog(this);
  mUi->stackedWidgetFunctionPipeline->setCurrentWidget(
      this->DentalPlanningPipeline);

  // this->poseIndicatorViewer->hide();
  // this->DentalNavigatePipeline->stackedWidget->addWidget(this->poseIndicatorViewer);
  // set progressDialog
  this->m_progressDialog = new QProgressDialog;
  this->m_progressDialog->setWindowModality(Qt::WindowModal);
  this->m_progressDialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
  this->m_progressDialog->setFixedSize(500, 80);
  this->m_progressDialog->setLabelText(tr("Loading..."));
  this->m_progressDialog->setCancelButton(0);
  this->m_progressDialog->setMinimumDuration(1000);
  this->m_progressDialog->setStyleSheet(
      "QWidget{background-color:#151d31}\
											   QProgressBar {                   \
											   border: 1px solid #292f5f;       \
							                   color: #fff;                     \
							                   font-weight: bold;              \
							                   background-color: #0c172a;      \
							                   text-align: center;}\n          \
							                   QProgressBar::chunk{            \
							                   background-color: #388fcb;      \
							                   width: 3px;                     \
							                   margin: 0.5px;}\n               \
							                   QLabel{ color:white; background - color:transparent; border:none}");
  this->m_progressDialog->close();
  QCoreApplication::processEvents(QEventLoop::AllEvents);

  // interactorstyles
  auto* stylesGroup = new QActionGroup(this);
  stylesGroup->addAction(mUi->action_Navigation_Mode);
  stylesGroup->addAction(mUi->action_Window_Level_Mode);
  stylesGroup->setExclusive(true);

  //   this->robot = URDashThread::New();
  //   this->welcomeWizard = new SCBWelcomeWizard();

  // connection
  // menu file
  connect(mUi->pushButtonWindowLevel,
          &QPushButton::clicked,
          mUi->action_Window_Level_Mode,
          &QAction::setChecked);
  connect(mUi->pushButtonNavigate,
          &QPushButton::clicked,
          mUi->action_Navigation_Mode,
          &QAction::setChecked);

  connect(mUi->pushButtonReset, &QPushButton::clicked, [this]() {
    // this->planarViewers[0]->ResetCamera(0);
    // this->planarViewers[0]->ResetCameraClippingRange(0);
    // this->planarViewers[0]->Render();
    // this->planarViewers[1]->ResetCamera(0);
    // this->planarViewers[1]->ResetCameraClippingRange(0);
    // this->planarViewers[1]->Render();
    // this->planarViewers[2]->ResetCamera(0);
    // this->planarViewers[2]->ResetCameraClippingRange(0);
    // this->planarViewers[2]->Render();
    // this->nonPlanarViewer->ResetCamera(0);
    // this->nonPlanarViewer->ResetCameraClippingRange(0);
    // this->nonPlanarViewer->Render();
  });

  connect(this->DentalPlanningPipeline->pushButtonPlanningConfirm,
          &QPushButton::clicked,
          this,
          &SCBDentalMainWindow::slotPlanningPipelineConfirm);
  connect(this->DentalRegistrationPipeline->pushButtonRegistrationConfirm,
          &QPushButton::clicked,
          this,
          &SCBDentalMainWindow::slotRegistrationPipelineConfirm);
  connect(this->DentalNavigatePipeline->pushButtonNavigationConfirm,
          &QPushButton::clicked,
          this,
          &SCBDentalMainWindow::slotNavigationPipelineConfirm);
  connect(this->DentalValidatePipeline->pushButtonValidationConfirm,
          &QPushButton::clicked,
          this,
          &SCBDentalMainWindow::slotValidationPipelineConfirm);
  connect(this->closingdialog->pushButtonContinueValidation,
          &QPushButton::clicked,
          this,
          &SCBDentalMainWindow::slotNavigationPipelineToValidation);

  // std::function<void(bool)> a = std::bind(&QAction::setCheckable,
  // mUi->action_Window_Level_Mode, std::placeholders::_1);
  // a(mUi->action_Window_Level_Mode);
  //  for recent images used

  //   const double PI = vtkMath::Pi();
  //   Joints_DataMap tmpjnt1, tmpjnt2, tmpjnt3;
  //   tmpjnt1.Copy(270.74 * PI / 180.0,
  //                18.32 * PI / 180.0,
  //                -150.04 * PI / 180.0,
  //                -15.98 * PI / 180.0,
  //                -88.53 * PI / 180.0,
  //                355.44 * PI / 180.0);
  //   tmpjnt2.Copy(182.30 * PI / 180.0,
  //                9.35 * PI / 180.0,
  //                -84.79 * PI / 180.0,
  //                -193.50 * PI / 180.0,
  //                -88.00 * PI / 180.0,
  //                9.84 * PI / 180.0);
  //   tmpjnt3.Copy(8.79 * PI / 180.0,
  //                -79.13 * PI / 180.0,
  //                -128.78 * PI / 180.0,
  //                -15.63 * PI / 180.0,
  //                77.04 * PI / 180.0,
  //                13.62 * PI / 180.0);
  //   m_ref_jnt.push_back(tmpjnt1);
  //   m_ref_jnt.push_back(tmpjnt2);
  //   m_ref_jnt.push_back(tmpjnt3);
}

SCBDentalMainWindow::~SCBDentalMainWindow()
{
  delete this->m_progressDialog;
  delete closingdialog;
  //   delete this->welcomeWizard;
}

void SCBDentalMainWindow::test()
{
  auto fileName = QApplication::applicationDirPath() + "/Dental.mitk";
  IOController::getInstance()->loadScene(fileName);
}

void SCBDentalMainWindow::onSceneLoaded()
{
  mUi->multiWidget->InitializeMultiWidget();
}

void SCBDentalMainWindow::setPatientNameIDLabel(QString name, int id, int age)
{
  QString nameID = name;

  if (age > 0)
  {
    nameID.append(" (");
    nameID.append(QString::number(age));
    nameID.append(")");
  }

  if (id > 0)
  {
    nameID.append(" ID: ");
    this->patientID = id;
    nameID.append(QString::number(id));
  }

  mUi->labelPatientNameID->setText(nameID);
}

void SCBDentalMainWindow::slotPlanningPipelineConfirm()
{
  // QString project = QFileDialog::getSaveFileName(this, tr("Commit XML"),
  //												"",
  // tr("XML files
  //(*.xml)")); if (!project.isEmpty()) { 	emit
  // signalCommitedProject(project);
  //	//return;
  // }
  currentProjectXml = this->getCurrentProjectDirectory() + "\\" +
                      QDateTime::currentDateTime().toString("yyyyMMdd") + "_" +
                      QString::number(patientID) + ".xml";
  emit signalCommitedProject(currentProjectXml);
  mUi->toolButtonRegistration->setChecked(true);
}

void SCBDentalMainWindow::slotRegistrationPipelineConfirm()
{
  mUi->toolButtonNavigation->setChecked(true);
}

void SCBDentalMainWindow::slotNavigationPipelineConfirm()
{
  //   MessageBOX ask if collapse robot
  ///////////////////////////////////////
  mUi->toolButtonValidation->setChecked(true);
  closingdialog->pushButtonContinueValidation->setHidden(false);
  closingdialog->show();
}

void SCBDentalMainWindow::slotValidationPipelineConfirm()
{
  if (!(currentProjectXml.isEmpty()))
  {
    emit signalCommitedProject(currentProjectXml);
  }
  // MessageBOX ask if collapse robot
  /////////////////////////////////////////
  closingdialog->pushButtonContinueValidation->setHidden(true);
  closingdialog->show();
}

void SCBDentalMainWindow::slotNavigationPipelineToValidation()
{
  closingdialog->hide();
  mUi->toolButtonValidation->setChecked(true);
}

void SCBDentalMainWindow::on_toolButtonPlanning_toggled(bool checked)
{
  if (checked)
  {
    mUi->stackedWidgetFunctionPipeline->setCurrentWidget(
        this->DentalPlanningPipeline);
  }
}
void SCBDentalMainWindow::on_toolButtonRegistration_toggled(bool checked)
{
  if (checked)
  {
    mUi->stackedWidgetFunctionPipeline->setCurrentWidget(
        this->DentalRegistrationPipeline);
    mUi->tabWidgetCentral->setCurrentIndex(
        2);  // change central widget to registration
  }
}

void SCBDentalMainWindow::on_toolButtonNavigation_toggled(bool checked)
{
  //   if (checked)
  //   {
  //     mUi->stackedWidgetFunctionPipeline->setCurrentWidget(
  //         this->DentalNavigatePipeline);
  //     if (SCBMicronTrackerThread::New()->getTracker()->isConnected())
  //     {
  //       this->DentalNavigatePipeline->startTracking();
  //     }
  //   }
  //   if (!checked)
  //   {
  //     if (SCBMicronTrackerThread::New()->getTracker()->isConnected())
  //     {
  //       this->DentalNavigatePipeline->stopTracking();
  //     }
  //   }
}
void SCBDentalMainWindow::on_toolButtonValidation_toggled(bool checked)
{
  if (checked)
  {
    mUi->stackedWidgetFunctionPipeline->setCurrentWidget(
        this->DentalValidatePipeline);
  }
}
void SCBDentalMainWindow::on_pushButtonHelp_clicked()
{
  //   SCBDocumentViewer viewer;
  //   viewer.showDocument(":/UserManual/UserManual");
  //   viewer.exec();
}

void SCBDentalMainWindow::on_pushButtonAbout_clicked()
{
  //中文：
  //牙科種植手术导航定位系统

  //软件名称：SK-DENT-IMP
  //发布版本：0.2.4a

  //英文
  // Sucabot Dental Implant Navigation System

  // Software Name: SK-DENT-IMP
  // Release Version: 0.2.4a

  QString softwareName = "Sucabot Dental Implant Navigation System";

  QString text;
  text += "";
  text += "\n\n";
  text += "Software Name: SK-DENT-IMP";
  text += "\n";
  text += "Release Version: 0.2.4a";

  QPixmap logoPixmap(":HyperBlue/hyperblue/icon/Dental-icon.png");
  logoPixmap = logoPixmap.scaledToHeight(100);

  QMessageBox msg;
  msg.setWindowTitle(softwareName);
  msg.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  msg.setIconPixmap(logoPixmap);
  msg.setStyleSheet(
      "QPushButton{background-color: #398fcc;color: rgb(255, 255, 255);}"
      "QPushButton:hover{ background-color:#2fa2f4; color:  white; }"
      "QPushButton:pressed{ background-color: #223555;} \n "
      "QLabel{color:white;} \n QMessageBox{border: 1px; border-color: "
      "#292f5f;background-color: #292f5f; color: rgb(255,255,255);};");
  msg.setText(text);
  msg.exec();
}

void SCBDentalMainWindow::on_tabWidgetCentral_currentChanged(int index)
{
  //   QSize central = this->tabWidgetCentral->size();
  //   // the margin is using hard code value in designer.
  //   // hard code in Designer.
  //   for (QList<SCBPlanarViewer*>::const_iterator cit =
  //   planarViewers.cbegin();
  //        cit != planarViewers.cend();
  //        ++cit)
  //   {
  //     (*cit)->hide();
  //   }
  //   nonPlanarViewer->hide();
  //   for (QList<SCBPlanarViewer*>::const_iterator cit =
  //            panoramicSagitalViewers.cbegin();
  //        cit != panoramicSagitalViewers.cend();
  //        ++cit)
  //   {
  //     (*cit)->hide();
  //   }
  //   panoramicViewer->hide();
  //   poseIndicatorViewer->hide();

  //   switch (index)
  //   {
  //     case 0:
  //       this->gridLayoutNormal->addWidget(this->nonPlanarViewer, 0, 0);
  //       this->nonPlanarViewer->setFixedSize((central.width() - 6 - 20) / 3.0,
  //                                           (central.height() - 6 - 40)
  //                                           / 2.0);
  //       this->nonPlanarViewer->show();
  //       this->gridLayoutNormal->addWidget(this->panoramicViewer, 0, 1, 1, 2);
  //       this->panoramicViewer->setFixedSize((central.width() - 6 - 20) / 3.0
  //       * 2,
  //                                           (central.height() - 6 - 40)
  //                                           / 2.0);
  //       this->panoramicViewer->show();
  //       this->gridLayoutNormal->addWidget(this->planarViewers[0], 1, 0);
  //       this->planarViewers[0]->setFixedSize((central.width() - 8 - 20)
  //       / 3.0,
  //                                            (central.height() - 6 - 40)
  //                                            / 2.0);
  //       this->planarViewers[0]->show();
  //       this->gridLayoutNormal->addWidget(this->planarViewers[1], 1, 1);
  //       this->planarViewers[1]->setFixedSize((central.width() - 8 - 20)
  //       / 3.0,
  //                                            (central.height() - 6 - 40)
  //                                            / 2.0);
  //       this->planarViewers[1]->show();
  //       this->gridLayoutNormal->addWidget(this->planarViewers[2], 1, 2);
  //       this->planarViewers[2]->setFixedSize((central.width() - 8 - 20)
  //       / 3.0,
  //                                            (central.height() - 6 - 40)
  //                                            / 2.0);
  //       this->planarViewers[2]->show();
  //       break;
  //     case 1:
  //       this->horizontalLayoutMultiViewer->addWidget(this->nonPlanarViewer);
  //       this->nonPlanarViewer->setFixedSize((central.width() - 6 - 20) / 3.0,
  //                                           (central.height() - 8 - 40)
  //                                           / 3.0);
  //       this->nonPlanarViewer->show();
  //       this->horizontalLayoutMultiViewer->addWidget(this->panoramicViewer);
  //       this->panoramicViewer->setFixedSize((central.width() - 6 - 20) / 3.0
  //       * 2,
  //                                           (central.height() - 8 - 40)
  //                                           / 3.0);
  //       this->panoramicViewer->show();
  //       for (int i = 0; i < 5; i++)
  //       {
  //         for (int j = 0; j < 2; j++)
  //         {
  //           this->gridLayoutMultiViewer->addWidget(
  //               this->panoramicSagitalViewers[j * 5 + i], j, i);
  //           this->panoramicSagitalViewers[j * 5 + i]->setFixedSize(
  //               (central.width() - 12 - 20) / 5.0,
  //               (central.height() - 8 - 40) / 3.0);
  //           this->panoramicSagitalViewers[j * 5 + i]->show();
  //         }
  //       }
  //       break;
  //     case 2:
  //       this->horizontalLayoutRegistration->insertWidget(0,
  //       this->verticalLayoutRegistration->addWidget(this->planarViewers[0]);
  //       this->planarViewers[0]->setFixedSize((central.width() - 6 - 20)
  //       / 3.0,
  //                                            (central.height() - 8 - 40)
  //                                            / 3.0);
  //       this->planarViewers[0]->show();
  //       this->verticalLayoutRegistration->addWidget(this->planarViewers[1]);
  //       this->planarViewers[1]->setFixedSize((central.width() - 6 - 20)
  //       / 3.0,
  //                                            (central.height() - 8 - 40)
  //                                            / 3.0);
  //       this->planarViewers[1]->show();
  //       this->verticalLayoutRegistration->addWidget(this->planarViewers[2]);
  //       this->planarViewers[2]->setFixedSize((central.width() - 6 - 20)
  //       / 3.0,
  //                                            (central.height() - 8 - 40)
  //                                            / 3.0);
  //       this->planarViewers[2]->show();
  //       break;
  //     case 3:
  //       this->gridLayoutSurgical->addWidget(this->nonPlanarViewer, 0, 0);
  //       this->nonPlanarViewer->setFixedSize((central.width() - 8 - 20) / 3.0,
  //                                           (central.height() - 6 - 40)
  //                                           / 2.0);
  //       this->nonPlanarViewer->show();
  //       this->gridLayoutSurgical->addWidget(this->poseIndicatorViewer, 0, 1);
  //       this->poseIndicatorViewer->setFixedSize(
  //           (central.width() - 8 - 20) / 3.0, (central.height() - 6 - 40)
  //           / 2.0);
  //       this->poseIndicatorViewer->show();
  //       this->gridLayoutSurgical->addWidget(this->planarViewers[0], 1, 0);
  //       this->planarViewers[0]->setFixedSize((central.width() - 8 - 20)
  //       / 3.0,
  //                                            (central.height() - 6 - 40)
  //                                            / 2.0);
  //       this->planarViewers[0]->show();
  //       this->gridLayoutSurgical->addWidget(this->planarViewers[1], 1, 1);
  //       this->planarViewers[1]->setFixedSize((central.width() - 8 - 20)
  //       / 3.0,
  //                                            (central.height() - 6 - 40)
  //                                            / 2.0);
  //       this->planarViewers[1]->show();
  //       this->gridLayoutSurgical->addWidget(this->planarViewers[2], 1, 2);
  //       this->planarViewers[2]->setFixedSize((central.width() - 8 - 20)
  //       / 3.0,
  //                                            (central.height() - 6 - 40)
  //                                            / 2.0);
  //       this->planarViewers[2]->show();
  //       break;
  //     default:
  //       break;
  //   }
}

void SCBDentalMainWindow::on_pushButtonOperation_clicked()
{
  //   mUi->pushButtonCollapse->setChecked(false);
  //   this->robot->SetRobotControlMode(
  //       AbstractRobot::RobotWorkMode::OperativeProcedure);
  //   if (this->robot->GetIsInMove())
  //   {
  //     this->robot->Brake();
  //   }

  //   this->robot->SetRequestStop(true);
  //   while (this->robot->GetIsInMove())
  //   {
  //     QTime dieTime = QTime::currentTime().addMSecs(16);
  //     while (QTime::currentTime() < dieTime)
  //       QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  //   }

  //   if (this->robot)
  //   {
  //     if (mUi->pushButtonOperation->isChecked() == false)
  //     {
  //       // Then it means a stop
  //       if (this->robot->GetIsInMove())
  //       {
  //         this->robot->Brake();
  //       }

  //       this->robot->SetRequestStop(true);
  //       while (this->robot->GetIsInMove())
  //       {
  //         QTime dieTime = QTime::currentTime().addMSecs(16);
  //         while (QTime::currentTime() < dieTime)
  //           QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  //       }
  //     }
  //     else
  //     {
  //       if (this->robot->GetIsInMove())
  //       {
  //         // Actually this should not be happening
  //         qDebug() << "Multiple move detected. Please press stop button to
  //         quit "
  //                     "all move and retry.";
  //         this->robot->SetRequestStop(true);
  //         // return; //suppose request stop is on, the other process should
  //         be
  //         // quiting move.
  //       }
  //       this->robot->SetRequestStop(false);
  //       qDebug() << "Warning: No collision check will be carried out on
  //       extend.";

  //       QFuture<int> t1 = QtConcurrent::run(this->robot,
  //                                           &AbstractRobot::Extend,
  //                                           5 * vtkMath::Pi() / 180.0,
  //                                           5 * vtkMath::Pi() / 180.0);

  //       QTime dieTime = QTime::currentTime().addMSecs(100);
  //       while (QTime::currentTime() < dieTime)
  //         QCoreApplication::processEvents(QEventLoop::AllEvents, 16);

  //       while (t1.isFinished() == false)
  //       {
  //         QTime dieTime = QTime::currentTime().addMSecs(16);
  //         while (QTime::currentTime() < dieTime)
  //           QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  //       }
  //     }
  //   }
  //   else
  //   {
  //     mUi->pushButtonOperation->setChecked(false);
  //   }
}

void SCBDentalMainWindow::on_pushButtonCollapse_clicked()
{
  //   this->robot->SetRobotControlMode(
  //       AbstractRobot::RobotWorkMode::OperativeProcedure);
  //   mUi->pushButtonOperation->setChecked(false);

  //   if (this->robot->GetIsInMove())
  //   {
  //     this->robot->Brake();
  //   }

  //   this->robot->SetRequestStop(true);
  //   while (this->robot->GetIsInMove())
  //   {
  //     QTime dieTime = QTime::currentTime().addMSecs(16);
  //     while (QTime::currentTime() < dieTime)
  //       QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  //   }

  //   if (this->robot)
  //   {
  //     if (mUi->pushButtonCollapse->isChecked() == false)
  //     {
  //       // Then it means a stop
  //       if (this->robot->GetIsInMove())
  //       {
  //         this->robot->Brake();
  //       }

  //       this->robot->SetRequestStop(true);
  //       while (this->robot->GetIsInMove())
  //       {
  //         QTime dieTime = QTime::currentTime().addMSecs(16);
  //         while (QTime::currentTime() < dieTime)
  //           QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  //       }
  //     }
  //     else
  //     {
  //       // then it mwans a move
  //       // Routine ATC validation
  //       if (this->robot->GetIsInMove())
  //       {
  //         // Actually this should not be happening
  //         qDebug() << "Multiple move detected. Please press stop button to
  //         quit "
  //                     "all move and retry.";
  //         this->robot->SetRequestStop(true);
  //         // return; //suppose request stop is on, the other process should
  //         be
  //         // quiting move.
  //       }
  //       this->robot->SetRequestStop(false);
  //       qDebug()
  //           << "Warning: No collision check will be carried out on
  //           collapse.";
  //       while (this->robot->GetIsInMove())
  //       {
  //         QTime dieTime = QTime::currentTime().addMSecs(16);
  //         while (QTime::currentTime() < dieTime)
  //           QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  //       }

  //       QFuture<int> t1 = QtConcurrent::run(this->robot,
  //                                           &AbstractRobot::Collapse,
  //                                           5 * vtkMath::Pi() / 180.0,
  //                                           5 * vtkMath::Pi() / 180.0);

  //       QTime dieTime = QTime::currentTime().addMSecs(100);
  //       while (QTime::currentTime() < dieTime)
  //         QCoreApplication::processEvents(QEventLoop::AllEvents, 16);

  //       while (t1.isFinished() == false)
  //       {
  //         QTime dieTime = QTime::currentTime().addMSecs(16);
  //         while (QTime::currentTime() < dieTime)
  //           QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  //       }
  //     }
  //   }
  //   else
  //   {
  //     mUi->pushButtonCollapse->setChecked(false);
  //   }
}

// void SCBDentalMainWindow::on_pushButtonFreeDrive_clicked()
//{
//	mUi->pushButtonCollapse->setChecked(false);
//	mUi->pushButtonOperation->setChecked(false);
//
//	if (this->robot->GetIsInMove()) {
//		this->robot->Brake();
//	}
//	this->robot->SetRequestStop(true);
//
//	while (this->robot->GetIsInMove()) {
//		QTime dieTime = QTime::currentTime().addMSecs(16);
//		while (QTime::currentTime() < dieTime)
//			QCoreApplication::processEvents(QEventLoop::AllEvents,
// 16);
//	}
//
//	if (this->robot)
//	{
//		if (mUi->pushButtonFreeDrive->isChecked()== false)
//		{
////Then it means a stop
//			if (this->robot->GetIsInMove()) {
//				this->robot->Brake();
//			}
//			this->robot->SetRequestStop(true);
//
//			while (this->robot->GetIsInMove()) {
//				QTime dieTime =
// QTime::currentTime().addMSecs(16); 				while
// (QTime::currentTime() < dieTime)
//					QCoreApplication::processEvents(QEventLoop::AllEvents,
// 16);
//			}
//		}
//		else
//		{
//			if (this->robot->GetIsInMove()) {
//				//Actually this should not be happening
//				qDebug() << "Multiple move detected. Please
// press stop button to quit all move and retry.";
// this->robot->SetRequestStop(true);
//				//return; //suppose request stop is on, the
// other process should be quiting move.
//			}
//
//		}
//
//	}
//	else
//	{
//		mUi->pushButtonFreeDrive->setChecked(false);
//	}
//}

void SCBDentalMainWindow::closeEvent(QCloseEvent* closeEvent)
{
  mUi->action_Exit->triggered();
  QMainWindow::closeEvent(closeEvent);
  //   ReSetWindows(EWX_SHUTDOWN, true);
}

void SCBDentalMainWindow::slotLoadingDialog(bool isLoading)
{
  if (isLoading)
  {
    // this->m_progressDialog = new QProgressDialog;
    // this->m_progressDialog->setWindowModality(Qt::WindowModal);
    // this->m_progressDialog->setWindowFlags(Qt::FramelessWindowHint |
    // Qt::Dialog); this->m_progressDialog->setFixedSize(500,80);
    // this->m_progressDialog->setMinimum(0);
    // this->m_progressDialog->setMaximum(100);
    // this->m_progressDialog->setValue(50);
    // this->m_progressDialog->setLabelText(tr("Loading..."));
    // this->m_progressDialog->setCancelButton(0);
    // this->m_progressDialog->setMinimumDuration(1000);
    // this->m_progressDialog->setStyleSheet("QWidget{background-color:#151d31}\
		//									   QProgressBar
    //{
    //\
		//									   border:
    // 1px solid #292f5f;
    //\
		//					                   color: #fff; \
		//					                   font-weight: bold; \
		//					                   background-color:
    //#0c172a;
    //\
		//					                   text-align:
    // center;}\n
    //\
		//					                   QProgressBar::chunk{
    //\
		//					                   background-color:
    //#388fcb;
    //\
		//					                   width: 3px; \
		//					                   margin: 0.5px;}\n \
		//					                   QLabel{ color:white;
    // background
    //- color:transparent; border:none}");
    this->m_progressDialog->setMinimum(0);
    this->m_progressDialog->setMaximum(100);
    this->m_progressDialog->setValue(50);
    this->m_progressDialog->show();
    QCoreApplication::processEvents(QEventLoop::AllEvents);
  }
  if (!isLoading)
  {
    this->m_progressDialog->setValue(100);
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    this->m_progressDialog->close();
    // delete this->m_progressDialog;
  }
}

void SCBDentalMainWindow::slotSetLoadingDialogValue(int val)
{
  this->m_progressDialog->setValue(val);
  QCoreApplication::processEvents(QEventLoop::AllEvents);
}

QString SCBDentalMainWindow::getCurrentProjectDirectory()
{
  int i = 1;
  while (true)
  {
    QString tempDirectory = PROJECT_DIR + "\\" + QString::number(patientID) +
                            "_" + QString::number(i);
    if (!QDir(tempDirectory).exists())
    {
      QDir(".").mkdir(tempDirectory);
      this->currentProjectDirectory = tempDirectory;
      return this->currentProjectDirectory;
    }
    i++;
  }
}