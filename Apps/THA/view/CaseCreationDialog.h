#ifndef CASE_CREATION_DIALOG
#define CASE_CREATION_DIALOG

// qt
#include <qdialog.h>
#include <qwidget.h>

// std
#include <memory>

namespace Ui
{
class CaseCreationDialog;
}
class CaseCreationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CaseCreationDialog(unsigned int id = 0, QWidget* parent = nullptr);
    ~CaseCreationDialog() override;

private:
    std::unique_ptr<Ui::CaseCreationDialog> mUi;
};

#endif  //! CASE_CREATION_DIALOG