#ifndef PLANNING_WIDGET_H
#define PLANNING_WIDGET_H

// qt
#include <QWidget>

namespace Ui {
class PlanningWidget;
}

class PlanningWidget : public QWidget {
public:
  explicit PlanningWidget(QWidget *parent = nullptr);
  ~PlanningWidget() override;

protected:
  void changeEvent(QEvent *event) override;

private:
  void enablePlanningWithBounding(bool enable);
  void on_buttonGroup_buttonToggled(class QAbstractButton *button, bool checked);

private:
  Ui::PlanningWidget *ui;
};

#endif //! PLANNING_WIDGET_H