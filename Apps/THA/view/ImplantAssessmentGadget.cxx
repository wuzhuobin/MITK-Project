#include "ImplantAssessmentGadget.h"
#include "ui_ImplantAssessmentGadget.h"

ImplantAssessmentGadget::ImplantAssessmentGadget(QWidget *parent) : QWidget(parent),
  ui(new Ui::ImplantAssessmentGadget)
{
  this->ui->setupUi(this);
}

ImplantAssessmentGadget::~ImplantAssessmentGadget()
{
  delete this->ui;
}
