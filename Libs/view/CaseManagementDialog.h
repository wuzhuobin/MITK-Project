#ifndef CASE_MANAGEMENT_DIALOG
#define CASE_MANAGEMENT_DIALOG

// qt
#include <QDialog>

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

  Q_SIGNAL void caseCreated(const QString& caseName,
                            const QStringList& dicomFiles);
  Q_SIGNAL void caseImported(const QString& caseName);

private:
  Ui::CaseManagementDialog* mUi = nullptr;
  class CaseModel* mCaseModel;
  Q_SLOT void on_pushButtonImport_clicked(bool checked = false);
  Q_SLOT void on_lineEditSearch_textChanged(const QString& text);
  Q_SLOT void createCase(const QStringList& dicoms);
};
#endif  // CASE_MANAGEMENT_DIALOG