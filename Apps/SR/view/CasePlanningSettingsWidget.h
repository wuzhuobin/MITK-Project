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
  double getDiameter() const;
  void setDiameter(double diameter);
  void setDoubleSpinBoxDiameterVisible(bool visible);
  bool getDoubleSpinBoxDiameterVisible() const;
  double getLength() const;
  void setLength(double length);
  void setDoubleSpinBoxLengthVisible(bool visible);
  bool getDoubleSpinBoxLengthVisible() const;
  bool getDataNodeVisibility() const;

  Q_SIGNAL void deleteClicked(bool checked = false);
  Q_SIGNAL void hideClicked(bool checked = false);

private:
  std::unique_ptr<Ui::CasePlanningSettingsWidget> mUi;
};

#endif  // !CASE_PLANNING_SETTINGS_WIDGET_H