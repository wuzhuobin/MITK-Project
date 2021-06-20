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
    mUi->spinBoxID->setValue(id);
    

    connect(mUi->lineEditName, &QLineEdit::textChanged, mUi->lineEditFileName, &QLineEdit::setText);
}

CaseCreationDialog::~CaseCreationDialog() = default;