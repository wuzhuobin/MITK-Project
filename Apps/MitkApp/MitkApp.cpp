/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/


#include <mitkBaseApplication.h>

#include <QVariant>

int main(int argc, char **argv)
{
  // Create a QApplication instance first
  mitk::BaseApplication myApp(argc, argv);
  myApp.setApplicationName("TestApp");
  myApp.setOrganizationName("DKFZ");

  myApp.setProperty(mitk::BaseApplication::PROP_APPLICATION, "org.mitk.qt.extapplication");
  return myApp.run();
}
