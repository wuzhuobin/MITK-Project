/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include "org_mitk_dental_Activator.h"

#include "DentalApplication.h"
#include "DentalPerspective.h"
#include "MitkView.h"

namespace mitk
{
void org_mitk_dental_Activator::start(ctkPluginContext* context)
{
  BERRY_REGISTER_EXTENSION_CLASS(MitkView, context)
  BERRY_REGISTER_EXTENSION_CLASS(DentalApplication, context)
  BERRY_REGISTER_EXTENSION_CLASS(DentalPerspective, context)
}

void org_mitk_dental_Activator::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}
}  // namespace mitk
