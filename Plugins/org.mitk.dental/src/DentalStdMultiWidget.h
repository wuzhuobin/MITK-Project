/**
 * @file DentalStdMultiWidget.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-16
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DENTAL_STD_MULTI_WIDGET_H
#define DENTAL_STD_MULTI_WIDGET_H

// mitk
#include <QmitkStdMultiWidget.h>

class DentalStdMultiWidget : public QmitkStdMultiWidget
{
  Q_OBJECT;

public:
  static DentalStdMultiWidget* getInstance();
  explicit DentalStdMultiWidget(QWidget* parent = nullptr,
                                Qt::WindowFlags f = nullptr,
                                const QString& name = "stdmulti");
  void InitializeMultiWidget() override;

private:
  void updateViewMode();
};

#endif  //! DENTAL_STD_MULTI_WIDGET_H