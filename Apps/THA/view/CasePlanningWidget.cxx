#include "CasePlanningWidget.h"
#include "ui_CasePlanningWidget.h"
// qt

// mitk
#include <mitkLogMacros.h>

CasePlanningWidget::CasePlanningWidget(QWidget *parent)
    : QStackedWidget(parent)
    , ui(new Ui::CasePlanningWidget)
{
  this->ui->setupUi(this);
}

CasePlanningWidget::~CasePlanningWidget()
{
  delete this->ui;
}

void CasePlanningWidget::on_CasePlanningWidget_currentChanged(int index)
{
  Q_UNUSED(index);
}

void CasePlanningWidget::Action_Pelvis_CT_Landmark_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisCTLandmark);
}

void CasePlanningWidget::Action_Implant_Planning_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->ImplantPlanning);
}

void CasePlanningWidget::Action_Broach_Tracking_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->BroachTracking);
}