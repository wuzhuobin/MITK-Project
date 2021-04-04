#ifndef IMPLANT_ASSESSMENT_GADGET_H
#define IMPLANT_ASSESSMENT_GADGET_H

// qt
#include <QWidget>

namespace Ui
{
  class ImplantAssessmentGadget;
}

class ImplantAssessmentGadget : public QWidget
{
  Q_OBJECT

public:
  explicit ImplantAssessmentGadget(QWidget *parent = nullptr);
  virtual ~ImplantAssessmentGadget() override;

private:
  Ui::ImplantAssessmentGadget *ui;
};

#endif // IMPLANT_ASSESSMENT_GADGET_H
