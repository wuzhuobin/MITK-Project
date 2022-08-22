#include "SCBDentalClosingDialog.h"

SCBDentalClosingDialog::SCBDentalClosingDialog(QWidget* parent) :
    QDialog(parent)
{
  this->setupUi(this);
  // this->robot = URDashThread::New();
  // this->setWindowFlags(Qt::WindowStaysOnTopHint);

  // const double PI = vtkMath::Pi();
  // Joints_DataMap tmpjnt1, tmpjnt2, tmpjnt3;
  // tmpjnt1.Copy(270.74 * PI / 180.0, 18.32* PI / 180.0, -150.04* PI / 180.0,
  // -15.98* PI / 180.0, -88.53* PI / 180.0, 355.44* PI / 180.0);
  // tmpjnt2.Copy(182.30* PI / 180.0, 9.35* PI / 180.0, -84.79* PI / 180.0,
  // -193.50* PI / 180.0, -88.00* PI / 180.0, 9.84* PI / 180.0);
  // tmpjnt3.Copy(8.79* PI / 180.0, -79.13* PI / 180.0, -128.78* PI / 180.0,
  // -15.63* PI / 180.0, 77.04* PI / 180.0, 13.62* PI / 180.0);
  // m_ref_jnt.push_back(tmpjnt1);
  // m_ref_jnt.push_back(tmpjnt2);
  // m_ref_jnt.push_back(tmpjnt3);

  // connect(this->pushButtonCancel2, SIGNAL(clicked()), this,
  // SLOT(pushbtn_cancel2_pressed())); connect(this->pushButtonCancel,
  // SIGNAL(clicked()), this, SLOT(pushbtn_cancel_pressed()));
  // connect(this->pushButtonCollapse, SIGNAL(clicked()), this,
  // SLOT(pushbtn_collapse_pressed()));
  // connect(this->pushButtonContinueValidation, SIGNAL(clicked()), this,
  // SLOT(pushbtn_continue_validation_pressed()));
  // connect(this->pushButtonEndThisSurgery, SIGNAL(clicked()), this,
  // SLOT(pushbtn_end_surgery_pressed())); connect(this->pushButtonShutDown,
  // SIGNAL(clicked()), this, SLOT(pushbtn_shutdown_pressed()));
  // connect(this->pushButtonStartNewSurgery, SIGNAL(clicked()), this,
  // SLOT(pushbtn_start_surgery_pressed())); this->hide();
}

SCBDentalClosingDialog::~SCBDentalClosingDialog() {}

void SCBDentalClosingDialog::pushbtn_end_surgery_pressed()
{
  this->stackedWidget->setCurrentIndex(1);
}

void SCBDentalClosingDialog::pushbtn_continue_validation_pressed()
{
  // do nothing
}

void SCBDentalClosingDialog::pushbtn_start_surgery_pressed()
{
  // todo:
}

void SCBDentalClosingDialog::pushbtn_cancel_pressed()
{
  this->hide();
}

void SCBDentalClosingDialog::pushbtn_shutdown_pressed()
{
}

void SCBDentalClosingDialog::pushbtn_collapse_pressed()
{
  // this->robot->SetRobotControlMode(AbstractRobot::RobotWorkMode::Normal);

  // if (this->robot->GetIsInMove()) {
  // 	this->robot->Brake();
  // }

  // this->robot->SetRequestStop(true);
  // while (this->robot->GetIsInMove()) {
  // 	QTime dieTime = QTime::currentTime().addMSecs(16);
  // 	while (QTime::currentTime() < dieTime)
  // 		QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  // }

  // if (this->robot)
  // {
  // 	if (this->pushButtonCollapse->isChecked() == false)
  // 	{
  // 		//Then it means a stop
  // 		if (this->robot->GetIsInMove()) {
  // 			this->robot->Brake();
  // 		}

  // 		this->robot->SetRequestStop(true);
  // 		while (this->robot->GetIsInMove()) {
  // 			QTime dieTime = QTime::currentTime().addMSecs(16);
  // 			while (QTime::currentTime() < dieTime)
  // 				QCoreApplication::processEvents(QEventLoop::AllEvents,
  // 16);
  // 		}
  // 	}
  // 	else
  // 	{
  // 		//then it mwans a move
  // 		//Routine ATC validation
  // 		if (this->robot->GetIsInMove()) {
  // 			//Actually this should not be happening
  // 			qDebug() << "Multiple move detected. Please press stop button
  // to quit all move and retry.";
  // this->robot->SetRequestStop(true);
  // 			//return; //suppose request stop is on, the other process
  // should be quiting move.
  // 		}
  // 		this->robot->SetRequestStop(false);
  // 		qDebug() << "Warning: No collision check will be carried out on
  // collapse."; 		while (this->robot->GetIsInMove()) { 			QTime
  // dieTime =
  // QTime::currentTime().addMSecs(16); 			while (QTime::currentTime() <
  // dieTime) 				QCoreApplication::processEvents(QEventLoop::AllEvents, 16);
  // 		}
  // 		std::vector<Joints_DataMap> jntlist;
  // 		jntlist.push_back(m_ref_jnt.at(1));
  // 		jntlist.push_back(m_ref_jnt.at(0));

  // 		QFuture<int> t1 = QtConcurrent::run(this->robot,
  // &AbstractRobot::SeriesMoveJ, jntlist); 		QTime dieTime =
  // QTime::currentTime().addMSecs(100); 		while (QTime::currentTime() <
  // dieTime) 			QCoreApplication::processEvents(QEventLoop::AllEvents, 16);

  // 		while (t1.isFinished() == false) {
  // 			QTime dieTime = QTime::currentTime().addMSecs(16);
  // 			while (QTime::currentTime() < dieTime)
  // 				QCoreApplication::processEvents(QEventLoop::AllEvents,
  // 16);
  // 		}
  // 		jntlist.clear();
  // 		jntlist.shrink_to_fit();
  // 	}

  // }
  // else
  // {
  // 	this->pushButtonCollapse->setChecked(false);
  // }
}

void SCBDentalClosingDialog::pushbtn_cancel2_pressed()
{
  this->stackedWidget->setCurrentIndex(0);
}
