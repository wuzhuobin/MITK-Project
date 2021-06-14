#ifndef apps_svpk_lib_shutdown_dialog_h_
#define apps_svpk_lib_shutdown_dialog_h_

#include <QMainWindow>
#include <QDebug>
#include <QApplication>
#include <QPushButton>

namespace Ui { class ShutdownDialog; }

class ShutdownDialog : public QMainWindow
{
    Q_OBJECT

public:
    ShutdownDialog(QWidget *parent = nullptr);
    ~ShutdownDialog();

private:
    Ui::ShutdownDialog *m_ui = nullptr;
    void initialize();
};
#endif // apps_svpk_lib_shutdown_dialog_h_
