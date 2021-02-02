#include "AcetabularPrepWidget.h"
#include "ui_AcetabularPrepWidget.h"

AcetabularPrepWidget::AcetabularPrepWidget(QWidget *parent)
    : QStackedWidget(parent),
      ui(new Ui::AcetabularPrepWidget)
{
  this->ui->setupUi(this);
}

AcetabularPrepWidget::~AcetabularPrepWidget()
{
  delete this->ui;
}