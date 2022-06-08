/**
 * @file CasePlanningWidget.h
 * @author wuzhuobin (zhuobin.wu@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CASE_PLANNING_WIDGET_H
#define CASE_PLANNING_WIDGET_H

#include <QStackedWidget>

// std
#include <memory>

namespace Ui
{
class CasePlanningWidget;
}
class CasePlanningWidget : public QStackedWidget
{
  Q_OBJECT
public:
  explicit CasePlanningWidget(QWidget* parent = nullptr);
  ~CasePlanningWidget() override;

private:
  std::unique_ptr<Ui::CasePlanningWidget> mUi;
};

#endif  //! CASE_PLANNING_WIDGET_H