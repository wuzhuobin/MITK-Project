#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
// qt
#include <QMainWindow>

// std
#include <memory>

namespace Ui
{
class MainWindow;
}
class QAbstractButton;
class QActionGroup;
class QAction;
class MainWindow : public QMainWindow
{
  Q_OBJECT;

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;
  void test();

private:
  std::unique_ptr<Ui::MainWindow> mUi;
  // class CasePlanningWidget* mCasePlanning;
  // class AcetabularPrepWidget* mAcetabularPrep;
  QActionGroup* mActionGroup;
  int mCurrentActionIndex = 0;
  void setCurrentActionIndex(int index);
  Q_DISABLE_COPY(MainWindow);

  Q_SLOT void on_radioButtonOptions_toggled(bool checked);
  Q_SLOT void on_buttonGroupMode_buttonClicked(QAbstractButton* button) const;
  Q_SLOT void on_buttonGroupView_buttonClicked(QAbstractButton* button) const;
  Q_SLOT void on_buttonGroupWorkflow_buttonClicked(QAbstractButton* button);
  Q_SLOT void on_pushButtonNext_clicked(bool checked = false);
  Q_SLOT void on_pushButtonBack_clicked(bool checked = false);
  Q_SLOT void on_pageLogin_accepted();
  Q_SLOT void on_pageCaseManagement_accepted();
  Q_SLOT void onActionsTriggered(QAction* action) const;
  Q_SLOT void onsceneLoaded();
};

#endif  //! MAIN_WINDOW_H