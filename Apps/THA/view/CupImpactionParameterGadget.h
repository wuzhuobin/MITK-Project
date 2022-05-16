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
  explicit CupImpactionParameterGadget(QWidget* parent = nullptr);
  ~CupImpactionParameterGadget() override;

private:
  std::unique_ptr<Ui::CupImpactionParameterGadget> mUi;
  Q_DISABLE_COPY(CupImpactionParameterGadget)
};

#endif  // !CUP_IMPACTION_PARAMETER_GADGET_H