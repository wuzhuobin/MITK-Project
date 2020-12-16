// qt
#include <QMainWindow>

namespace Ui { class MainWindow; }

class MainWindow: public QMainWindow
{
  Q_OBJECT;
public:
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow() override;
private:
  Ui::MainWindow *mpUi;
private:
  Q_DISABLE_COPY(MainWindow);
};