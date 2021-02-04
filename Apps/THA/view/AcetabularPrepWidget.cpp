#include "AcetabularPrepWidget.h"
#include "ui_AcetabularPrepWidget.h"

// qt
#include <QActionGroup>

AcetabularPrepWidget::AcetabularPrepWidget(QWidget *parent)
    : QStackedWidget(parent)
    , actions(new QActionGroup(this))
    , ui(new Ui::AcetabularPrepWidget)
{
  this->ui->setupUi(this);
  this->actions->addAction(this->ui->action_Pelvis_Checkpoint);
  this->actions->addAction(this->ui->action_Pelvis_Landmark);
  this->actions->addAction(this->ui->action_Pelvis_Registration);
  this->actions->addAction(this->ui->action_RIO_Registratoin);
}

AcetabularPrepWidget::~AcetabularPrepWidget()
{
  delete this->ui;
}

void AcetabularPrepWidget::on_action_Pelvis_Checkpoint_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisCheckpoint);
}

void AcetabularPrepWidget::on_action_Pelvis_Landmark_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisLandmark);
}

void AcetabularPrepWidget::on_action_Pelvis_Registration_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->PelvisRegistration);
}

void AcetabularPrepWidget::on_action_RIO_Registratoin_triggered(bool checked)
{
  Q_UNUSED(checked);
  this->setCurrentWidget(this->ui->RIORegistration);
}

void AcetabularPrepWidget::on_AcetabularPrepWidget_currentChanged(int index)
{
  Q_UNUSED(index);
}