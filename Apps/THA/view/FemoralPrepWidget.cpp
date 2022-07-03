#include "FemoralPrepWidget.h"

#include "ui_FemoralPrepWidget.h"

FemoralPrepWidget::FemoralPrepWidget(QWidget* parent) :
    QStackedWidget(parent), mUi(std::make_unique<Ui::FemoralPrepWidget>())
{
  mUi->setupUi(this);

  // connection
  // connect(mUi->checkBoxProximalCheckpoint,
  //         &QCheckBox::toggled,
  //         this,
  //         &FemoralPrepWidget::checkBoxProximalCheckpointToggled);
  // connect(mUi->checkBoxDistalCheckpoint,
  //         &QCheckBox::toggled,
  //         this,
  //         &FemoralPrepWidget::checkBoxDistalCheckpointToggled);
}

FemoralPrepWidget::~FemoralPrepWidget() = default;

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

void FemoralPrepWidget::on_checkBoxProximalCheckpoint_toggled(bool checked)
{
  Q_EMIT checkBoxProximalCheckpointToggled(checked);
  if (checked)
  {
    mUi->pushButtonFemoralLandmarkCapture->setChecked(mProximalCheckpointCaptured);
    mUi->pushButtonFemoralLandmarkClearAll->setChecked(!mProximalCheckpointCaptured);
  }
}

void FemoralPrepWidget::on_checkBoxDistalCheckpoint_toggled(bool checked)
{
  Q_EMIT checkBoxDistalCheckpointToggled(checked);
  if (checked)
  {
    mUi->pushButtonFemoralLandmarkCapture->setChecked(mDistalCheckpointCaptured);
    mUi->pushButtonFemoralLandmarkClearAll->setChecked(!mDistalCheckpointCaptured);
  }
}

void FemoralPrepWidget::on_pushButtonFemoralLandmarkCapture_toggled(bool checked)
{
  if (checked)
  {
    if(mUi->checkBoxProximalCheckpoint->isChecked())
    {
      mProximalCheckpointCaptured = true;
    }
    else if(mUi->checkBoxDistalCheckpoint->isChecked())
    {
      mDistalCheckpointCaptured = true;
    }
  }
}

void FemoralPrepWidget::on_pushButtonFemoralLandmarkClearAll_toggled(bool checked)
{
  if (checked)
  {
    if(mUi->checkBoxProximalCheckpoint->isChecked())
    {
      mProximalCheckpointCaptured = false;
    }
    else if(mUi->checkBoxDistalCheckpoint->isChecked())
    {
      mDistalCheckpointCaptured = false;
    }
  }
}