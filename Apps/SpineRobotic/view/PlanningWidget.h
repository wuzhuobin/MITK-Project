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
  Ui::PlanningWidget *mUi;
  static constexpr const char* PIN_LEFT = "pin_left_";
  static constexpr const char* PIN_RIGHT = "pin_right_";

  void enablePlanningWithBounding(bool enable);
  void on_buttonGroup_buttonToggled(class QAbstractButton *button, bool checked);
  void on_comboBoxRight_activated(int index);
  void on_comboBoxLeft_activated(int index);
  void addPin(QString base, QString typeName);
};

#endif //! PLANNING_WIDGET_H