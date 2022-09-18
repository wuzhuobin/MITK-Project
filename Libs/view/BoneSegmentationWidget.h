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
class BoneSegmentationWidgetPrivate;
class BoneSegmentationWidget : public QWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(BoneSegmentationWidget)
public:
  explicit BoneSegmentationWidget(QWidget* parent = nullptr);
  ~BoneSegmentationWidget() override;
  Q_SLOT void showVoi();
  Q_SLOT void setVoi();
  Q_SLOT void doSegmentation();

private:
  std::unique_ptr<Ui::BoneSegmentationWidget> mUi;
  std::unique_ptr<BoneSegmentationWidgetPrivate> d_ptr;

  Q_SLOT void on_toolButtonShowVoi_toggled(bool checked);
  Q_SLOT void on_toolButtonSetVoi_toggled(bool checked);
  Q_SLOT void on_toolButtonUnsharpMask_toggled(bool checked);
  Q_SLOT void on_toolButtonBodyMask_toggled(bool checked);
  Q_SLOT void on_toolButtonOtsuThresholdSliceBySlice_toggled(bool checked);
  Q_SLOT void on_toolButtonThreshold_toggled(bool checked);
  Q_SLOT void on_toolButtonImageToSurface_toggled(bool checked);
  Q_SLOT void on_toolButtonSave_clicked(bool checked = false);
  Q_SLOT void on_toolButtonSaveSegmentation_clicked(bool checked = false);
  Q_SLOT void on_toolButtonRedo_clicked(bool checked = false);
};

#endif  //! BONE_SEGMENTATION_WIDGET_H