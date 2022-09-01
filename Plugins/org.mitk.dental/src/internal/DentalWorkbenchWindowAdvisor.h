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

#include <berryWorkbenchWindowAdvisor.h>

class DentalWorkbenchWindowAdvisor : public berry::WorkbenchWindowAdvisor
{
public:
  explicit DentalWorkbenchWindowAdvisor(
      const berry::SmartPointer<berry::IWorkbenchWindowConfigurer>& configurer);
};

#endif  // !DENTAL_WORKBENCH_WINDOW_ADVISOR_H