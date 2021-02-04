#ifndef CASEPLANNINGWIDGET_H
#define CASEPLANNINGWIDGET_H

#include <QStackedWidget>

namespace Ui { class CasePlanningWidget; }

class QActionGroup;
class CasePlanningWidget : public QStackedWidget
{
  Q_OBJECT;

public:
  explicit CasePlanningWidget(QWidget *parent = nullptr);
  virtual ~CasePlanningWidget() override;
  const QActionGroup * GetActions() const { return this->actions; }

private:
  Ui::CasePlanningWidget *ui;
  QActionGroup *actions;
  Q_DISABLE_COPY(CasePlanningWidget);

private Q_SLOTS:

  void on_action_Pelvis_CT_Landmark_triggered(bool checked = false);
  void on_action_Implant_Planning_triggered(bool checked = false);
  void on_action_Broach_Tracking_triggered(bool checked = false);
  void on_CasePlanningWidget_currentChanged(int index);
};
#endif // CASEPLANNINGWIDGET_H
