/**
 * @file SCBDentalPlanningPipeline.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef SCB_DENTAL_PLANNING_PIPELINE_H
#define SCB_DENTAL_PLANNING_PIPELINE_H

#include "ui_SCBDentalPlanningPipeline.h"

// qt
#include <QWidget>

namespace mitk
{
class DataNode;
}

class SCBDentalPlanningPipeline : public QWidget,
                                  public Ui::SCBDentalPlanningPipeline
{
  Q_OBJECT

public:
  explicit SCBDentalPlanningPipeline(QWidget* parent = nullptr);

  Q_SIGNAL void signalConfirmPlanning();

private:
  void addAffineBaseDataInteractor3D(mitk::DataNode* node);
  Q_SLOT void on_pushButtonAdvancedOption_clicked();
  Q_SLOT void on_pushButtonAutoReconstruction_clicked();
  Q_SLOT void on_buttonGroupTeethPosition_buttonToggled(QAbstractButton* button,
                                                        bool checked);

  Q_SLOT void on_toolButtonSurgicalGuidePlanningShowVoi_toggled(bool checked);
  Q_SLOT void on_toolButtonSurgicalGuidePlanningSetVoi_toggled(bool checked);
  Q_SLOT void on_toolButtonSurgicalGuidePlanningDoSegmentation_toggled(
      bool checked);
  Q_SLOT void on_toolButtonSurgicalGuidePlanningPaintBrush_toggled(
      bool checked);
  Q_SLOT void on_toolButtonSurgicalGuidePlanningImageToSurface_toggled(
      bool checked);
  Q_SLOT void on_toolButtonSurgicalGuidePlanningAdvance_toggled(bool checked);
  Q_SLOT void on_toolButtonSurgicalGuidePlanningAddAnImplant_clicked(
      bool checked = false);
};

#endif  // SCB_DENTAL_PLANNING_PIPELINE_H
