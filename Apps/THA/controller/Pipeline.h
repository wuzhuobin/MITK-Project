#ifndef PIPELINE_H
#define PIPELINE_H

// qt
#include <QObject>

class MainWindow;
class LoginView;
class CaseManagementWidget;

class Pipeline : public QObject
{
    Q_OBJECT

public:
    explicit Pipeline(QObject *parent = nullptr);
    ~Pipeline() override;

    void start();
    void test();

private:

    MainWindow *mainWindow;
    LoginView *loginView;
    CaseManagementWidget *mCaseManagementWidget;
};

#endif // !PIPELINE_H
