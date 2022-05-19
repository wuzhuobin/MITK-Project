/**
 * @file CupImpactionParameterGadget.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-05-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CUP_IMPACTION_PARAMETER_GADGET_H
#define CUP_IMPACTION_PARAMETER_GADGET_H

// qt
#include <QWidget>

// std
#include <memory>
namespace Ui
{
class CupImpactionParameterGadget;
}
class CupImpactionParameterGadget : public QWidget
{
  Q_OBJECT
public:
  explicit CupImpactionParameterGadget(
      const QString& acetabularShell = QStringLiteral("acetabular_shell"),
      const QString& impactingAcetabularShell =
          QStringLiteral("impacting_acetabular_shell"),
      const QString& cupCor = QStringLiteral("cup_cor"),
      QWidget* parent = nullptr);
  explicit CupImpactionParameterGadget(QWidget* parent) :
      CupImpactionParameterGadget(QStringLiteral("acetabular_shell"),
                                  QStringLiteral("impacting_acetabular_shell"),
                                  QStringLiteral("cup_cor"),
                                  parent)
  {
  }
  ~CupImpactionParameterGadget() override;

  void observerCupImpaction();
  void updateActualSpinBoxes();
  void updatePlannedSpinBoxes();
  void updateSpinBoxes();

private:
  QString mAcetabularShell;
  QString mImpactingAcetabularShell;
  QString mCupCor;
  std::unique_ptr<Ui::CupImpactionParameterGadget> mUi;
  Q_DISABLE_COPY(CupImpactionParameterGadget)
};

#endif  // !CUP_IMPACTION_PARAMETER_GADGET_H