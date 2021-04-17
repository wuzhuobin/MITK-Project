#ifndef apps_svpk_lib__case_manage_view_h_
#define apps_svpk_lib__case_manage_view_h_

#include <QWidget>

namespace Ui { class CaseManageView; }

class CaseManageView :public QWidget
{
public:
    CaseManageView(QWidget *parent = nullptr);
    ~CaseManageView();


private:
    Ui::CaseManageView *ui = nullptr;

};
#endif // apps_svpk_lib__case_manage_view_h_