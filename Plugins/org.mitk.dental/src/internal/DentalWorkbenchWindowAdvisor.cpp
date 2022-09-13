/**
 * @file DentalWorkbenchWindowAdvisor.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-09-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "DentalWorkbenchWindowAdvisor.h"

#include "ui_SCBDentalMainWindow.h"

// mitk
#include <berryIWorkbenchWindow.h>
#include <berryIWorkbenchWindowConfigurer.h>
#include <mitkLogMacros.h>

DentalWorkbenchWindowAdvisor::DentalWorkbenchWindowAdvisor(
    const berry::SmartPointer<berry::IWorkbenchWindowConfigurer>& configurer,
    QObject* parent) :
    QObject(parent),
    berry::WorkbenchWindowAdvisor(configurer),
    mControls(std::make_unique<Ui::SCBDentalMainWindow>())

{
}

DentalWorkbenchWindowAdvisor::~DentalWorkbenchWindowAdvisor() = default;

void DentalWorkbenchWindowAdvisor::PreWindowOpen()
{
  GetWindowConfigurer()->SetInitialSize({1000, 1000});
}

void DentalWorkbenchWindowAdvisor::CreateWindowContents(
    berry::Shell::Pointer shell)
{
  auto* mainWindow = static_cast<QMainWindow*>(shell->GetControl());
  mControls->setupUi(mainWindow);

  berry::WorkbenchWindowAdvisor::CreateWindowContents(shell);
  // GetWindowConfigurer()->CreatePageComposite(mainWindow);
}