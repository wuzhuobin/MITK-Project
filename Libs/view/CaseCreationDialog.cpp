#include "CaseCreationDialog.h"

#include "ui_CaseCreationDialog.h"

// qt
#include <qdialog.h>
#include <qdialogbuttonbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qwidget.h>

CaseCreationDialog::CaseCreationDialog(unsigned int id, QWidget* parent) :
    QDialog(parent), mUi(std::make_unique<Ui::CaseCreationDialog>())
{
    mUi->setupUi(this);
    mUi->spinBoxID->setValue(static_cast<int>(id));
    mUi->lineEditFileName->setText('.' + mUi->spinBoxID->text());
    mUi->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

CaseCreationDialog::~CaseCreationDialog() = default;

QString CaseCreationDialog::getFileName() const
{
    return mUi->lineEditFileName->text();
}

void CaseCreationDialog::on_lineEditName_textChanged(const QString& text)
{
    mUi->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!text.isEmpty());
    mUi->lineEditFileName->setText(text + '.' + mUi->spinBoxID->text());
}
