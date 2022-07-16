/**
 * @file SRStdMultiWidget.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-16
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SR_STD_MULTI_WIDGET_H
#define SR_STD_MULTI_WIDGET_H

// mitk
#include <QmitkStdMultiWidget.h>
class GroupBoxGadget;
class SRStdMultiWidget : public QmitkStdMultiWidget
{
  Q_OBJECT;

public:
  static SRStdMultiWidget* getInstance();
  explicit SRStdMultiWidget(QWidget* parent = nullptr,
                            Qt::WindowFlags f = nullptr,
                            const QString& name = "stdmulti");
  void InitializeMultiWidget() override;

  void setTransformTarget(const QString& transformTarget = QString());

  void enableGroupBox(bool flag);

  void enableDisplay(bool flag);

private:
  GroupBoxGadget* mGroupBoxGadget[4];
  bool scrollEnabled = true;
  QString mTransformTarget;
  void updateViewMode();
};

#endif  //! SR_STD_MULTI_WIDGET_H