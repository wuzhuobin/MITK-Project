#ifndef CASE_MANAGEMENT_DIALOG
#define CASE_MANAGEMENT_DIALOG

// qt
#include <qdialog.h>

namespace Ui
{
class CaseManagementDialog;
}

class CaseManagementDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CaseManagementDialog(QWidget* parent = nullptr);
    ~CaseManagementDialog() override;

private:
    Ui::CaseManagementDialog* mUi = nullptr;
    class CaseModel *mCaseModel;
    Q_SLOT void on_pushButtonImport_clicked(bool checked = false);
};
#endif  // CASE_MANAGEMENT_DIALOG