#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
namespace Ui
{
class MainWindow;
}
class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void Test();

private:
  Ui::MainWindow *ui;

};

#endif //! MAIN_WINDOW_H