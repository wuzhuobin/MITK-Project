#include "AcetabularPrepView.h"

#include "ui_AcetabularPrepView.h"


AcetabularPrepView::AcetabularPrepView(QWidget* parent) : mUi(std::make_unique<Ui::AcetabularPrepView>())
{
    mUi->setupUi(this);
}

AcetabularPrepView::~AcetabularPrepView() = default;

void AcetabularPrepView::updateView(View view)
{
    switch (view)
    {
        case View::AcetabularPinInstallationAndAcquisition:
            mUi->stackedWidgetViewer->setCurrentWidget(mUi->imageWidget);
            break;
    }
}