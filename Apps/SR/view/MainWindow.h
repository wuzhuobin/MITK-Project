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

class QActionGroup;
class QAction;
class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;
  void test();

private:
  std::unique_ptr<Ui::MainWindow> mUi;
  int mCurrentActionIndex = 0;
  QActionGroup* mActionGroup;

  void initializeMenu();
  void initializeConnection();
  void setCurrentActionIndex(int index);

  Q_SLOT void on_toolButtonNext_clicked(bool checked = false);
  Q_SLOT void on_toolButtonPrevious_clicked(bool checked = false);
  Q_SLOT void on_toolButtonLoad_clicked(bool checked = false);
  Q_SLOT void on_toolButtonExport_clicked(bool checked = false);
  Q_SLOT void onActionsTriggered(QAction* action);
  Q_SLOT void onSceneLoaded();
};

#endif  //! MAIN_WINDOW_H