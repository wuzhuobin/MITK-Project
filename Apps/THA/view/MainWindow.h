#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
// qt
#include <QMainWindow>

namespace Ui { class MainWindow; }
class QAbstractButton;
class QActionGroup;
class QAction;
class MainWindow: public QMainWindow
{
  Q_OBJECT;
public:
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow() override;
  void Test();

private:
  Ui::MainWindow *ui;
  class CasePlanningWidget *casePlanning;
  class AcetabularPrepWidget *acetabularPrep;
  QActionGroup *actionGroup;
  void SetCurrentActionIndex(int index);
  int currentActionIndex = 0;
  Q_DISABLE_COPY(MainWindow);

private Q_SLOTS:
  void on_radioButtonOptions_toggled(bool checked);
  void on_buttonGroupMode_buttonClicked(QAbstractButton *button) const;
  void on_buttonGroupView_buttonClicked(QAbstractButton *button) const;
  void on_buttonGroupWorkflow_buttonClicked(QAbstractButton *button);
  void on_pushButtonNext_clicked(bool checked = false);
  void on_pushButtonBack_clicked(bool checked = false);
  void OnActionsTriggered(QAction *action) const;
};

#endif //!MAIN_WINDOW_H