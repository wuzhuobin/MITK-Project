#ifndef CASEPLANNINGWIDGET_H
#define CASEPLANNINGWIDGET_H

#include <QStackedWidget>

namespace Ui { class CasePlanningWidget; }

class QComboBox;
class CasePlanningWidget : public QStackedWidget
{
  Q_OBJECT;

public:
  explicit CasePlanningWidget(QWidget *parent = nullptr);
  virtual ~CasePlanningWidget() override;

public Q_SLOTS:

  void action_Pelvis_CT_Landmark_triggered(bool checked = false);
  void action_Implant_Planning_triggered(bool checked = false);
  void action_Broach_Tracking_triggered(bool checked = false);

private:
  Ui::CasePlanningWidget *ui;
  Q_DISABLE_COPY(CasePlanningWidget);

private Q_SLOTS:
  void on_CasePlanningWidget_currentChanged(int index);
};
#endif // CASEPLANNINGWIDGET_H
