/**
 * @file DentalApplication.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-08-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "DentalApplication.h"

#include "DentalWorkbenchAdvisor.h"

// mitk
#include <berryPlatformUI.h>

// std
#include <memory>

QVariant DentalApplication::Start(berry::IApplicationContext* context)
{
  Q_UNUSED(context);
  std::unique_ptr<berry::Display> display(berry::PlatformUI::CreateDisplay());

  std::unique_ptr<DentalWorkbenchAdvisor> workbenchAdvisor(
      new DentalWorkbenchAdvisor);

  auto code = berry::PlatformUI::CreateAndRunWorkbench(display.get(),
                                                       workbenchAdvisor.get());

  return code == berry::PlatformUI::RETURN_RESTART ? EXIT_RESTART : EXIT_OK;
}