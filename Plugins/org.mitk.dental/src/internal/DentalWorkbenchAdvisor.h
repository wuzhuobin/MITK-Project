/**
 * @file DentalWorkbenchAdvisor.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-09-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DENTAL_WORKBENCH_ADVISOR
#define DENTAL_WORKBENCH_ADVISOR

// mitk
#include <berryQtWorkbenchAdvisor.h>

class DentalWorkbenchAdvisor : public berry::QtWorkbenchAdvisor
{
public:
  static const QString GDefafultPerspectiveId;

  void Initialize(berry::IWorkbenchConfigurer::Pointer configurer) override;
  berry::WorkbenchWindowAdvisor* CreateWorkbenchWindowAdvisor(
      berry::IWorkbenchWindowConfigurer::Pointer configurer) override;

  QString GetInitialWindowPerspectiveId() override;
};

#endif  // !DENTAL_WORKBENCH_ADVISOR