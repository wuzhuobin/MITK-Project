#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
// qt
#include <QMainWindow>

namespace Ui { class MainWindow; }
class QAbstractButton;

class MainWindow: public QMainWindow
{
  Q_OBJECT;
public:
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow() override;

private:
  Ui::MainWindow *ui;
  Q_DISABLE_COPY(MainWindow);

private Q_SLOTS:
  void on_radioButtonOptions_toggled(bool checked);
  void on_buttonGroupMode_buttonClicked(QAbstractButton *button);
  void on_buttonGroupView_buttonClicked(QAbstractButton *button);
};

#endif //!MAIN_WINDOW_H