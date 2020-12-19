#include "CasePlanningWidget.h"
#include "ui_CasePlanningWidget.h"
// qt
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