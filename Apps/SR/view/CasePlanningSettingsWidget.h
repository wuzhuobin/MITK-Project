/**
 * @file CasePlanningSettingsWidget.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CASE_PLANNING_SETTINGS_WIDGET_H
#define CASE_PLANNING_SETTINGS_WIDGET_H

// qt
#include <QWidget>

// std
#include <memory>

namespace Ui
{
class CasePlanningSettingsWidget;
}
class CasePlanningSettingsWidget : public QWidget
{
  Q_OBJECT
public:
  explicit CasePlanningSettingsWidget(QWidget* parent = nullptr);
  ~CasePlanningSettingsWidget() override;

private:
  std::unique_ptr<Ui::CasePlanningSettingsWidget> mUi;
};

#endif  // !CASE_PLANNING_SETTINGS_WIDGET_H