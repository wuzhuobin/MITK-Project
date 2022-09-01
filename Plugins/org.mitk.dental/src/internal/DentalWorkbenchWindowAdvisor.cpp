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

DentalWorkbenchWindowAdvisor::DentalWorkbenchWindowAdvisor(
    const berry::SmartPointer<berry::IWorkbenchWindowConfigurer>& configurer) :
    berry::WorkbenchWindowAdvisor(configurer)
{
}