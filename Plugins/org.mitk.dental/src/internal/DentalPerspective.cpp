/**
 * @file DentalPerspective.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-08-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "DentalPerspective.h"

// mitk
#include <berryIViewLayout.h>

void DentalPerspective::CreateInitialLayout(berry::IPageLayout::Pointer layout)
{
  layout->SetEditorAreaVisible(false);
}