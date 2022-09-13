/**
 * @file DentalWorkbenchWindowAdvisor.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-09-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DENTAL_WORKBENCH_WINDOW_ADVISOR_H
#define DENTAL_WORKBENCH_WINDOW_ADVISOR_H

// mitk
#include <berryShell.h>
#include <berryWorkbenchWindowAdvisor.h>
class QAbstractButton;
namespace Ui
{
class SCBDentalMainWindow;
}
class DentalWorkbenchWindowAdvisor : public QObject,
                                     public berry::WorkbenchWindowAdvisor
{
  Q_OBJECT
public:
  explicit DentalWorkbenchWindowAdvisor(
      const berry::SmartPointer<berry::IWorkbenchWindowConfigurer>& configurer,
      QObject* parent = nullptr);
  ~DentalWorkbenchWindowAdvisor() override;

  void PreWindowOpen() override;
  void CreateWindowContents(berry::Shell::Pointer shell) override;

private:
  std::unique_ptr<Ui::SCBDentalMainWindow> mControls;
};

#endif  // !DENTAL_WORKBENCH_WINDOW_ADVISOR_H