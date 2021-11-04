#include "AcetabularPrepNextBackWidget.h"
#include "ui_AcetabularPrepNextBackWidget.h"

AcetabularPrepNextBackWidget::AcetabularPrepNextBackWidget(QWidget * parent)
  : QWidget(parent), mUi(std::make_unique<Ui::AcetabularPrepNextBackWidget>())
{
  mUi->setupUi(this);
}

AcetabularPrepNextBackWidget::~AcetabularPrepNextBackWidget() = default;

void AcetabularPrepNextBackWidget::on_pushButtonBack_clicked(bool checked)
{
  Q_EMIT backClicked();
}

void AcetabularPrepNextBackWidget::on_pushButtonNext_clicked(bool checked)
{
  Q_EMIT nextClicked();
}
