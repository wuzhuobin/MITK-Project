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
}

AcetabularPrepWidget::~AcetabularPrepWidget()
{
  delete this->ui;
}