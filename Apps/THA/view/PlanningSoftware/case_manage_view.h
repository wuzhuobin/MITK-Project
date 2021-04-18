#ifndef apps_svpk_lib__case_manage_view_h_
#define apps_svpk_lib__case_manage_view_h_

#include <QWidget>
#include <QStandardItemModel>

namespace Ui { class CaseManageView; }

class CaseManageView :public QWidget
{
  Q_OBJECT
public:
    explicit CaseManageView(QWidget *parent = nullptr);
    ~CaseManageView();

Q_SIGNALS:
    void finished();

private:
    Ui::CaseManageView *ui = nullptr;

    QStandardItemModel patientTableModel;

private Q_SLOTS:
    void on_nextStepBtn_clicked(bool checked = false);
};
#endif // apps_svpk_lib__case_manage_view_h_