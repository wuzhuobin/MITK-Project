#include "CasePlanningWidget.h"
#include "ui_CasePlanningWidget.h"
// qt
#include <QActionGroup>

// mitk
#include <mitkLogMacros.h>

CasePlanningWidget::CasePlanningWidget(QWidget *parent)
    : QStackedWidget(parent)
    , actions(new QActionGroup(this))
    , ui(new Ui::CasePlanningWidget)
{
  this->ui->setupUi(this);
  this->actions->addAction(this->ui->action_Pelvis_CT_Landmark);
  this->actions->addAction(this->ui->action_Implant_Planning);
  this->actions->addAction(this->ui->action_Broach_Tracking);
}

CasePlanningWidget::~CasePlanningWidget()
{
  delete this->ui;
}

void CasePlanningWidget::on_CasePlanningWidget_currentChanged(int index)
{
  Q_UNUSED(index);
}

void CasePlanningWidget::on_action_Pelvis_CT_Landmark_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisCTLandmark);
}

void CasePlanningWidget::on_action_Implant_Planning_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->ImplantPlanning);
}

void CasePlanningWidget::on_action_Broach_Tracking_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->BroachTracking);
}