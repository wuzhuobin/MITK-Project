#include "AcetabularPrepView.h"
#include "ui_AcetabularPrepView.h"

AcetabularPrepView::AcetabularPrepView(QWidget * parent)
  :mUi(std::make_unique<Ui::AcetabularPrepView>())
{
  mUi->setupUi(this);
}

AcetabularPrepView::~AcetabularPrepView() = default;
