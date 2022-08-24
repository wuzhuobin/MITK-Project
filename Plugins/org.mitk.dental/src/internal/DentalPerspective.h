/**
 * @file DentalPerspective.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-08-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DENTAL_PERSPECTIVE_H
#define DENTAL_PERSPECTIVE_H

// mitk
#include <berryIPerspectiveFactory.h>

// qt
#include <QObject>

class DentalPerspective : public QObject, public berry::IPerspectiveFactory
{
  Q_OBJECT
  Q_INTERFACES(berry::IPerspectiveFactory)

public:
  void CreateInitialLayout(berry::IPageLayout::Pointer layout) override;
};

#endif  // !DENTAL_PERSPECTIVE_H