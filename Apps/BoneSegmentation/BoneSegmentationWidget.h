/**
 * @file BoneSegmentationWidget.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-31
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef BONE_SEGMENTATION_WIDGET_H
#define BONE_SEGMENTATION_WIDGET_H

// qt
#include <QWidget>

// std
#include <memory>

namespace Ui
{
class BoneSegmentationWidget;
}
class BoneSegmentationWidget : public QWidget
{
  Q_OBJECT
public:
  explicit BoneSegmentationWidget(QWidget* parent = nullptr);
  ~BoneSegmentationWidget() override;

private:
  std::unique_ptr<Ui::BoneSegmentationWidget> mUi;

  void on_toolButtonShowVoi_toggled(bool checked);
};

#endif  //! BONE_SEGMENTATION_WIDGET_H