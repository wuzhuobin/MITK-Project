/**
 * @file CasePlanningWidget.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CASE_PLANNING_WIDGET_H
#define CASE_PLANNING_WIDGET_H

#include "PointSetDataInteractorScrew.h"

// qt
#include <QStackedWidget>

// mitk
#include <mitkClippingPlaneInteractor3D.h>
#include <mitkPointSetDataInteractor.h>
#include <mitkToolManager.h>

// std
#include <memory>

namespace Ui
{
class CasePlanningWidget;
}
class QButtonGroup;
class QAbstractButton;
class CasePlanningWidget : public QStackedWidget
{
  Q_OBJECT
public:
  explicit CasePlanningWidget(QWidget* parent = nullptr);
  ~CasePlanningWidget() override;
  Q_SLOT void onActionScrewPlanningTriggered(bool checked = false);
  Q_SLOT void onActionPathPlanningTriggered(bool checked = false);
  Q_SLOT void onActionPlatePlanningTriggered(bool checked = false);
  Q_SLOT void onActionIntervalPlanningTriggered(bool checked = false);
  Q_SLOT void onActionLateralPlanningTriggered(bool checked = false);
  Q_SLOT void onActionPosteriorPlanningTriggered(bool checked = false);

private:
  std::unique_ptr<Ui::CasePlanningWidget> mUi;
  QButtonGroup* mButtonGroupScrew;
  PointSetDataInteractorScrew::Pointer mPointSetDataInteractorScrew;
  QButtonGroup* mButtonGroupPath;
  mitk::PointSetDataInteractor::Pointer mPointSetDataInteractor;
  QButtonGroup* mButtonGroupPlate;
  QButtonGroup* mButtonGroupInterval;
  QButtonGroup* mButtonGroupLateral;
  mitk::ClippingPlaneInteractor3D::Pointer mClippingPlaneInteractors[2];
  QButtonGroup* mButtonGroupPosterior;
  mitk::ToolManager::Pointer mToolManager;

  Q_SLOT void on_CasePlanningWidget_currentChanged(int index);
  Q_SLOT void on_pushButtonScrewNew_clicked(bool checked = false);
  Q_SLOT void on_pushButtonScrewConfirm_clicked(bool checked = false);
  Q_SLOT void on_pushButtonPathNew_clicked(bool checked = false);
  Q_SLOT void on_pushButtonPathConfirm_clicked(bool checked = false);
  Q_SLOT void on_pushButtonPlateNew_clicked(bool checked = false);
  Q_SLOT void on_spinBoxPlateSize_valueChanged(int value);
  Q_SLOT void on_pushButtonIntervalNew_clicked(bool checked = false);
  Q_SLOT void on_spinBoxIntervalSize_valueChanged(int value);
  Q_SLOT void on_pushButtonLateralNew_clicked(bool checked = false);
  Q_SLOT void on_radioButtonLateralTranslate_toggled(bool checked);
  Q_SLOT void on_radioButtonLateralRotate_toggled(bool checked);
  Q_SLOT void on_pushButtonPosteriorNew_clicked(bool checked = false);
  Q_SLOT void on_radioButtonPosteriorTranslate_toggled(bool checked);
  Q_SLOT void on_radioButtonPosteriorRotate_toggled(bool checked);
  Q_SLOT void onButtonGroupScrewButtonToggled(QAbstractButton* button,
                                              bool checked = false);
  Q_SLOT void onButtonGroupPathButtonToggled(QAbstractButton* button,
                                             bool checked = false);
  Q_SLOT void onButtonGroupPlateButtonToggled(QAbstractButton* button,
                                              bool checked = false);
  Q_SLOT void onButtonGroupIntervalButtonToggled(QAbstractButton* button,
                                                 bool checked = false);
  Q_SLOT void onButtonGroupLateralButtonToggled(QAbstractButton* button,
                                                bool checked = false);
  Q_SLOT void onButtonGroupPosteriorButtonToggled(QAbstractButton* button,
                                                  bool checked = false);
  Q_SLOT void onPlateDeleteClicked(bool checked);
  Q_SLOT void onPlateHideClicked(bool checked);
  Q_SLOT void onIntervalDeleteClicked(bool checked);
  Q_SLOT void onIntervalHideClicked(bool checked);
  Q_SLOT void onLateralDeleteClicked(bool checked);
  Q_SLOT void onLateralHideClicked(bool checked);
  Q_SLOT void onPosteriorDeleteClicked(bool checked);
  Q_SLOT void onPosteriorHideClicked(bool checked);
};

#endif  //! CASE_PLANNING_WIDGET_H