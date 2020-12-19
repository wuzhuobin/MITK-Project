#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
// qt
#include <QMainWindow>

namespace Ui { class MainWindow; }

class MainWindow: public QMainWindow
{
  Q_OBJECT;
public:
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow() override;

public Q_SLOTS:
  void on_radioButtonOptions_toggled(bool checked);

private:
  Ui::MainWindow *ui;
private:
  Q_DISABLE_COPY(MainWindow);
};

#endif //!MAIN_WINDOW_H