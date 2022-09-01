/**
 * @file DentalWorkbenchAdvisor.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-09-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "DentalWorkbenchAdvisor.h"

#include "DentalWorkbenchWindowAdvisor.h"

const QString DentalWorkbenchAdvisor::GDefafultPerspectiveId =
    "org.mitk.dental.dentalperspective";
berry::WorkbenchWindowAdvisor*
DentalWorkbenchAdvisor::CreateWorkbenchWindowAdvisor(
    berry::IWorkbenchWindowConfigurer::Pointer configurer)
{
  return new DentalWorkbenchWindowAdvisor(configurer);
}

QString DentalWorkbenchAdvisor::GetInitialWindowPerspectiveId()
{
  return GDefafultPerspectiveId;
}