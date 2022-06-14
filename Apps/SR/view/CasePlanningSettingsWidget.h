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
class QRadioButton;
class CasePlanningSettingsWidget : public QWidget
{
  Q_OBJECT
public:
  explicit CasePlanningSettingsWidget(const QString& objectName = QString(),
                                      QWidget* parent = nullptr);
  ~CasePlanningSettingsWidget() override;

  QRadioButton* getRadioButton() const;
  const QString getCasePlanningName() const;

private:
  std::unique_ptr<Ui::CasePlanningSettingsWidget> mUi;
  Q_SLOT void on_toolButtonDelete_clicked(bool checked = false);
  Q_SLOT void on_toolButtonHide_toggled(bool checked);
};

#endif  // !CASE_PLANNING_SETTINGS_WIDGET_H