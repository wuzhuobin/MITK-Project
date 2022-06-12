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
class PathSettingsWidget : public QWidget
{
  Q_OBJECT
public:
  explicit PathSettingsWidget(const QString& pathName,
                              QWidget* parent = nullptr);
  ~PathSettingsWidget() override;

private:
  std::unique_ptr<Ui::PathSettingsWidget> mUi;
};

#endif  // !PATH_SETTINGS_WIDGET_H