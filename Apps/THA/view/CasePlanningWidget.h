#ifndef CASEPLANNINGWIDGET_H
#define CASEPLANNINGWIDGET_H

#include <QStackedWidget>

namespace Ui { class CasePlanningWidget; }

class CasePlanningWidget : public QStackedWidget
{
  Q_OBJECT;

public:
  explicit CasePlanningWidget(QWidget *parent = nullptr);
  virtual ~CasePlanningWidget() override;

public Q_SLOTS:

  void Action_Pelvis_CT_Landmark_triggered(bool checked = false);
  void Action_Implant_Planning_triggered(bool checked = false);
  void Action_Broach_Tracking_triggered(bool checked = false);

private:
  Ui::CasePlanningWidget *ui;
  Q_DISABLE_COPY(CasePlanningWidget);

private Q_SLOTS:
  void on_CasePlanningWidget_currentChanged(int index);
};
#endif // CASEPLANNINGWIDGET_H
