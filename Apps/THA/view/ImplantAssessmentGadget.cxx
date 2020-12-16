#include "ImplantAssessmentGadget.h"
#include "ui_ImplantAssessmentGadget.h"

ImplantAssessmentGadget::ImplantAssessmentGadget(QWidget *parent) : QWidget(parent),
  ui(new Ui::ImplantAssessmentGadget)
{
  this->ui->setupUi(this);

  // this->SetPreOpHipLength(INT_MIN);
  // this->SetPreOpCombinedOffset(INT_MIN);
}

ImplantAssessmentGadget::~ImplantAssessmentGadget()
{
  delete this->ui;
}

void ImplantAssessmentGadget::SetSide(QString side)
{
  QString tmp;
  tmp.append(QString::fromLocal8Bit("术前规划"));
  tmp.append("(");
  tmp.append(side);
  tmp.append(")");

  ui->label->setText(tmp);
}

void ImplantAssessmentGadget::SetPreOpHipLength(int l)
{
  if (l == INT_MIN)
  {
    this->ui->label_preOpHipLengthValue->setText("N/A");
    this->ui->label_preOpHipLengthString->setText("");
    return;
  }

  ui->label_preOpHipLengthValue->setText(QString::number(abs(l)));

  if (l > 0)
    this->ui->label_preOpHipLengthString->setText(QString::fromLocal8Bit("增长"));
  else if (l < 0)
    this->ui->label_preOpHipLengthString->setText(QString::fromLocal8Bit("缩短"));
  else if (l == 0)
    this->ui->label_preOpHipLengthString->setText("");
}

void ImplantAssessmentGadget::SetPreOpCombinedOffset(int l)
{
  if (l == INT_MIN)
  {
    this->ui->label_preOpCombinedOffsetValue->setText("N/A");
    this->ui->label_preOpCombinedOffsetString->setText("");
    return;
  }

  this->ui->label_preOpCombinedOffsetValue->setText(QString::number(abs(l)));

  if (l > 0)
    this->ui->label_preOpCombinedOffsetString->setText(QString::fromLocal8Bit("增加"));
  else if (l < 0)
    this->ui->label_preOpCombinedOffsetString->setText(QString::fromLocal8Bit("减少"));
  else if (l == 0)
    this->ui->label_preOpCombinedOffsetString->setText("");
}

void ImplantAssessmentGadget::SetOppositeHipLength(int l)
{
  if (l == INT_MIN)
  {
    this->ui->label_oppositeHipLengthValue->setText("N/A");
    this->ui->label_oppositeHipLengthString->setText("");
    return;
  }

  this->ui->label_oppositeHipLengthValue->setText(QString::number(abs(l)));

  if (l > 0)
    this->ui->label_oppositeHipLengthString->setText(QString::fromLocal8Bit("增长"));
  else if (l < 0)
    this->ui->label_oppositeHipLengthString->setText(QString::fromLocal8Bit("缩短"));
  else if (l == 0)
    this->ui->label_oppositeHipLengthString->setText("");
}

void ImplantAssessmentGadget::SetOppositeCombinedOffset(int l)
{
  if (l == INT_MIN)
  {
    this->ui->label_oppositeCombinedOffsetValue->setText("N/A");
    this->ui->label_oppositeCombinedOffsetString->setText("");
    return;
  }

  this->ui->label_oppositeCombinedOffsetValue->setText(QString::number(abs(l)));

  if (l > 0)
    this->ui->label_oppositeCombinedOffsetString->setText(QString::fromLocal8Bit("增加"));
  else if (l < 0)
    this->ui->label_oppositeCombinedOffsetString->setText(QString::fromLocal8Bit("减少"));
  else if (l == 0)
    this->ui->label_oppositeCombinedOffsetString->setText("");
}