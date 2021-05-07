#ifndef PIPELINE_H
#define PIPELINE_H

// qt
#include <QObject>

class MainWindow;
class LoginView;
class CaseManageView;

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
    CaseManageView *caseManageView;
};

#endif // !PIPELINE_H
