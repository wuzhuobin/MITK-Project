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

// void ImplantAssessmentGadget::SetPreOpHipLength(int l)
// {
//   if (l == INT_MIN)
//   {
//     this->ui->label_preOpHipLengthValue->setText("N/A");
//     this->ui->label_preOpHipLengthString->setText("");
//     return;
//   }

//   ui->label_preOpHipLengthValue->setText(QString::number(abs(l)));

//   if (l > 0)
//     this->ui->label_preOpHipLengthString->setText(QString::fromLocal8Bit("����"));
//   else if (l < 0)
//     this->ui->label_preOpHipLengthString->setText(QString::fromLocal8Bit("����"));
//   else if (l == 0)
//     this->ui->label_preOpHipLengthString->setText("");
// }

// void ImplantAssessmentGadget::SetPreOpCombinedOffset(int l)
// {
//   if (l == INT_MIN)
//   {
//     this->ui->label_preOpCombinedOffsetValue->setText("N/A");
//     this->ui->label_preOpCombinedOffsetString->setText("");
//     return;
//   }

//   this->ui->label_preOpCombinedOffsetValue->setText(QString::number(abs(l)));

//   if (l > 0)
//     this->ui->label_preOpCombinedOffsetString->setText(QString::fromLocal8Bit("����"));
//   else if (l < 0)
//     this->ui->label_preOpCombinedOffsetString->setText(QString::fromLocal8Bit("����"));
//   else if (l == 0)
//     this->ui->label_preOpCombinedOffsetString->setText("");
// }

// void ImplantAssessmentGadget::SetOppositeHipLength(int l)
// {
//   if (l == INT_MIN)
//   {
//     this->ui->label_oppositeHipLengthValue->setText("N/A");
//     this->ui->label_oppositeHipLengthString->setText("");
//     return;
//   }

//   this->ui->label_oppositeHipLengthValue->setText(QString::number(abs(l)));

//   if (l > 0)
//     this->ui->label_oppositeHipLengthString->setText(QString::fromLocal8Bit("����"));
//   else if (l < 0)
//     this->ui->label_oppositeHipLengthString->setText(QString::fromLocal8Bit("����"));
//   else if (l == 0)
//     this->ui->label_oppositeHipLengthString->setText("");
// }

// void ImplantAssessmentGadget::SetOppositeCombinedOffset(int l)
// {
//   if (l == INT_MIN)
//   {
//     this->ui->label_oppositeCombinedOffsetValue->setText("N/A");
//     this->ui->label_oppositeCombinedOffsetString->setText("");
//     return;
//   }

//   this->ui->label_oppositeCombinedOffsetValue->setText(QString::number(abs(l)));

//   if (l > 0)
//     this->ui->label_oppositeCombinedOffsetString->setText(QString::fromLocal8Bit("����"));
//   else if (l < 0)
//     this->ui->label_oppositeCombinedOffsetString->setText(QString::fromLocal8Bit("����"));
//   else if (l == 0)
//     this->ui->label_oppositeCombinedOffsetString->setText("");
// }