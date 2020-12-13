#include "CasePlanningWidget.hpp"
#include "ui_CasePlanningWidget.h"
// qt
CasePlanningWidget::CasePlanningWidget(QWidget *parent)
    : QStackedWidget(parent)
    , mpUi(new Ui::CasePlanningWidget)
{
  this->mpUi->setupUi(this);
}

CasePlanningWidget::~CasePlanningWidget()
{
  delete this->mpUi;
}

void CasePlanningWidget::on_CasePlanningWidget_currentChanged(int index)
{
  Q_UNUSED(index);
}