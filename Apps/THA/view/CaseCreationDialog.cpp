#include "CaseCreationDialog.h"

#include "ui_CaseCreationDialog.h"

// qt
#include <qdialog.h>
#include <qlineedit.h>
#include <qwidget.h>

CaseCreationDialog::CaseCreationDialog(unsigned int id, QWidget* parent) :
    QDialog(parent), mUi(std::make_unique<Ui::CaseCreationDialog>())
{
    mUi->setupUi(this);
    mUi->spinBoxID->setValue(static_cast<int>(id));
    mUi->lineEditFileName->setText(mUi->spinBoxID->text());
}

CaseCreationDialog::~CaseCreationDialog() = default;

void CaseCreationDialog::on_lineEditName_textChanged(const QString& text)
{
    mUi->lineEditFileName->setText(text + '.' + mUi->spinBoxID->text());
}
