#include "ImplantAssessmentGadget.h"
#include "ui_ImplantAssessmentGadget.h"

ImplantAssessmentGadget::ImplantAssessmentGadget(QWidget *parent) : QWidget(parent),
  mUi(new Ui::ImplantAssessmentGadget)
{
  this->mUi->setupUi(this);
}

ImplantAssessmentGadget::~ImplantAssessmentGadget()
{
  delete this->mUi;
}
