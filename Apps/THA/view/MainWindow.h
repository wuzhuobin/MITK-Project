#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
// qt
#include <QMainWindow>

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
    Ui::MainWindow* mUi;
    // class CasePlanningWidget* mCasePlanning;
    // class AcetabularPrepWidget* mAcetabularPrep;
    QActionGroup* mActionGroup;
    void setCurrentActionIndex(int index);
    int mCurrentActionIndex = 0;
    Q_DISABLE_COPY(MainWindow);

    Q_SLOT void on_radioButtonOptions_toggled(bool checked);
    Q_SLOT void on_buttonGroupMode_buttonClicked(QAbstractButton* button) const;
    Q_SLOT void on_buttonGroupView_buttonClicked(QAbstractButton* button) const;
    Q_SLOT void on_buttonGroupWorkflow_buttonClicked(QAbstractButton* button);
    Q_SLOT void on_pushButtonNext_clicked(bool checked = false);
    Q_SLOT void on_pushButtonBack_clicked(bool checked = false);
    Q_SLOT void onActionsTriggered(QAction* action) const;
    Q_SLOT void onSceneLoaded();
};

#endif  //! MAIN_WINDOW_H