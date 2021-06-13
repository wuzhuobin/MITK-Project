#ifndef CASE_MANAGEMENT_WIDGET
#define CASE_MANAGEMENT_WIDGET

#include <QStandardItemModel>
#include <QWidget>

namespace Ui
{
class CaseManagementWidget;
}

class CaseManagementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CaseManagementWidget(QWidget* parent = nullptr);
    ~CaseManagementWidget() override;

Q_SIGNALS:
    void finished();

private:
    Ui::CaseManagementWidget* mUi = nullptr;

    QStandardItemModel mPatientTableModel;

    Q_SLOT void on_nextStepBtn_clicked(bool checked = false);
};
#endif  // CASE_MANAGEMENT_WIDGET