/**
 * @file PathSettingsWidget.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PATH_SETTINGS_WIDGET_H
#define PATH_SETTINGS_WIDGET_H

// qt
#include <QWidget>

// std
#include <memory>

namespace Ui
{
class PathSettingsWidget;
}
class QRadioButton;
class PathSettingsWidget : public QWidget
{
  Q_OBJECT
public:
  explicit PathSettingsWidget(const QString& pathName = QString(),
                              QWidget* parent = nullptr);
  ~PathSettingsWidget() override;
  QRadioButton* getRadioButton() const;
  const QString getPathName() const;
  double getDiameter() const;
  void setDiameter(double diameter);
  bool getVisibility() const;

private:
  std::unique_ptr<Ui::PathSettingsWidget> mUi;
  Q_SLOT void on_toolButtonDelete_clicked(bool checked = false);
  Q_SLOT void on_toolButtonHide_toggled(bool checked);
};

#endif  // !PATH_SETTINGS_WIDGET_H