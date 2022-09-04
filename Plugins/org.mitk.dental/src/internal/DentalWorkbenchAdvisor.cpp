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

// mitk
#include <mitkLogMacros.h>

const QString DentalWorkbenchAdvisor::GDefafultPerspectiveId =
    "org.mitk.dental.dentalperspective";

void DentalWorkbenchAdvisor::Initialize(
    berry::IWorkbenchConfigurer::Pointer configurer)
{
  MITK_INFO << __func__;
  berry::WorkbenchAdvisor::Initialize(configurer);
  auto* reg = configurer->GetWorkbench()->GetPerspectiveRegistry();

  auto perspectives = reg->GetPerspectives();
  for (auto p : perspectives)
  {
    MITK_INFO << p->GetDescription().toStdString();
    MITK_INFO << p->GetLabel().toStdString();
  }
}

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