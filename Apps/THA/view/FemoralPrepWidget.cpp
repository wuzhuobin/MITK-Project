#include "FemoralPrepWidget.h"

#include "ui_FemoralPrepWidget.h"

FemoralPrepWidget::FemoralPrepWidget(QWidget* parent) :
    QStackedWidget(parent), mUi(std::make_unique<Ui::FemoralPrepWidget>())
{
  mUi->setupUi(this);
}

void FemoralPrepWidget::action_Femoral_Landmark_triggered(bool checked)
{
  Q_UNUSED(checked);

  this->setCurrentWidget(mUi->FemoralLandmark);
}

void FemoralPrepWidget::action_Femoral_Checkpoint_triggered(bool checked)
{
  Q_UNUSED(checked);

  this->setCurrentWidget(mUi->FemoralCheckpoint);
}

void FemoralPrepWidget::action_Femoral_Registration_triggered(bool checked)
{
  Q_UNUSED(checked);

  this->setCurrentWidget(mUi->FemoralRegistration);
}

void FemoralPrepWidget::action_Neck_Resection_Guide_triggered(bool checked)
{
  Q_UNUSED(checked);

  this->setCurrentWidget(mUi->NeckResectionGuide);
}

void FemoralPrepWidget::action_Broach_Tracking_triggered(bool checked)
{
  Q_UNUSED(checked);

  this->setCurrentWidget(mUi->BroachTracking);
}