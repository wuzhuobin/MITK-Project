/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

// mitk
#include <mitkBaseApplication.h>

// ctk
// #include <ctkPluginFrameworkLauncher.h>

// qt
#include <QVariant>

int main(int argc, char** argv)
{
  // Create a QApplication instance first
  mitk::BaseApplication myApp(argc, argv);
  myApp.setApplicationName("DentalApp");
  myApp.setOrganizationName("DKFZ");

  myApp.setProperty(mitk::BaseApplication::PROP_APPLICATION,
                    "org.mitk.dental.dentalapplication");
  myApp.setProperty(mitk::BaseApplication::ARG_CONSOLELOG, true);
  // myApp.setProperty(ctkPluginFrameworkLauncher::PROP_CONSOLE_LOG, true);

  return myApp.run();
}
