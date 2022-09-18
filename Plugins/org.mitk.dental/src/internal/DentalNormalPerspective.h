/**
 * @file DentalNormalPerspective.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-09-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DENTAL_NORMAL_PERSPECTIVE_H
#define DENTAL_NORMAL_PERSPECTIVE_H

// qt
#include <QObject>

// mitk
#include <berryIPerspectiveFactory.h>

class DentalNormalPerspective : public QObject,
                                public berry::IPerspectiveFactory
{
  Q_OBJECT
  Q_INTERFACES(berry::IPerspectiveFactory)
public:
  void CreateInitialLayout(berry::IPageLayout::Pointer layout) override;
};

#endif  // DENTAL_NORMAL_PERSPECTIVE_H