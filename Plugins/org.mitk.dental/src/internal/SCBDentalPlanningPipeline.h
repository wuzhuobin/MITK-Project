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

class SCBDentalPlanningPipeline : public QWidget,
                                  public Ui::SCBDentalPlanningPipeline
{
  Q_OBJECT

public:
  explicit SCBDentalPlanningPipeline(QWidget* parent = nullptr);

  Q_SIGNAL void signalConfirmPlanning();

private:
  Q_SLOT void on_pushButtonAdvancedOption_clicked();
  Q_SLOT void on_pushButtonAutoReconstruction_clicked();
  Q_SLOT void on_tableWidgetTeeth_cellClicked(int row, int column);
  Q_SLOT void on_buttonGroupTeethPosition_buttonToggled(QAbstractButton* button,
                                                        bool checked);
  Q_SLOT void addImplant(QString uniqueName);
  Q_SLOT void removeImplant(QString uniqueName);
};

#endif  // SCB_DENTAL_PLANNING_PIPELINE_H