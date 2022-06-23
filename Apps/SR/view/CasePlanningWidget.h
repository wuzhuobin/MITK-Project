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

// qt
#include <QStackedWidget>

// mitk
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
  mitk::PointSetDataInteractor::Pointer mPointSetDataInteractorScrew;
  QButtonGroup* mButtonGroupPath;
  mitk::PointSetDataInteractor::Pointer mPointSetDataInteractor;
  QButtonGroup* mButtonGroupPlate;
  QButtonGroup* mButtonGroupInterval;
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
  Q_SLOT void onButtonGroupScrewButtonToggled(QAbstractButton* button,
                                              bool checked = false);
  Q_SLOT void onButtonGroupPathButtonToggled(QAbstractButton* button,
                                             bool checked = false);
  Q_SLOT void onButtonGroupPlateButtonToggled(QAbstractButton* button,
                                              bool checked = false);
  Q_SLOT void onButtonGroupIntervalButtonToggled(QAbstractButton* button,
                                                 bool checked = false);
};

#endif  //! CASE_PLANNING_WIDGET_H