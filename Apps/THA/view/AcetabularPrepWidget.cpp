#include "AcetabularPrepWidget.h"
#include "ui_AcetabularPrepWidget.h"

// qt

AcetabularPrepWidget::AcetabularPrepWidget(QWidget *parent)
    : QStackedWidget(parent)
    , ui(new Ui::AcetabularPrepWidget)
{
  this->ui->setupUi(this);
}

AcetabularPrepWidget::~AcetabularPrepWidget()
{
  delete this->ui;
}

void AcetabularPrepWidget::Action_Pelvis_Checkpoint_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisCheckpoint);
}

void AcetabularPrepWidget::Action_Pelvis_Landmark_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisLandmark);
}

void AcetabularPrepWidget::Action_Pelvis_Registration_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisRegistration);
}

void AcetabularPrepWidget::Action_RIO_Registratoin_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->RIORegistration);
}

void AcetabularPrepWidget::on_AcetabularPrepWidget_currentChanged(int index)
{
  Q_UNUSED(index);
}