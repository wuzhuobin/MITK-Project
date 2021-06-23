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
    class THADicomLocalStorageWidget* mInternal;
    class CaseModel* mCaseModel;
    Q_SLOT void on_pushButtonImport_clicked(bool checked = false);
    Q_SLOT void createCase(QHash<QString, QVariant> map);
};
#endif  // CASE_MANAGEMENT_DIALOG