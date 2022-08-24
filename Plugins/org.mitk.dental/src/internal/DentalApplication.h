/**
 * @file DentalApplication.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-08-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DENTAL_APPLICATION_H
#define DENTAL_APPLICATION_H

// mitk
#include <berryIApplication.h>

class DentalApplication : public QObject, public berry::IApplication
{
  Q_OBJECT
  Q_INTERFACES(berry::IApplication)
public:
  QVariant Start(berry::IApplicationContext* context) override;
  void Stop() override {}
};

#endif  // !DENTAL_APPLICATION_H