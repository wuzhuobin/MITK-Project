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
  void on_CasePlanningWidget_currentChanged(int index);

private:
    Ui::CasePlanningWidget *ui;
};
#endif // CASEPLANNINGWIDGET_H
