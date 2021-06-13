#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

// qt
#include <QMainWindow>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
  void test();

private:
  Ui::MainWindow *ui;
  int currentAction = 0;
  class QActionGroup * mActionGroup;

  void initializeMenu();
  void initializeConnection();

  Q_SLOT void on_toolButtonNext_clicked(bool checked = false);
  Q_SLOT void on_toolButtonPrevious_clicked(bool checked = false);
  Q_SLOT void on_toolButtonLoad_clicked(bool checked = false);
  Q_SLOT void on_toolButtonExport_clicked(bool checked = false);
  Q_SLOT void actionsTriggered(class QAction *action);
  Q_SLOT void onSceneLoaded();
};

#endif //! MAIN_WINDOW_H