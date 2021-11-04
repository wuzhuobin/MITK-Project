#include "AcetabularPrepView.h"

#include "ui_AcetabularPrepView.h"

AcetabularPrepView::AcetabularPrepView(QWidget* parent) : mUi(std::make_unique<Ui::AcetabularPrepView>())
{
    mUi->setupUi(this);
}

AcetabularPrepView::~AcetabularPrepView() = default;

void AcetabularPrepView::updateView(View view)
{
    mUi->cupParameterGadget->setVisible(false);
    mUi->progressBar->setVisible(false);
    mUi->frameRegistration->setVisible(false);
    switch (view)
    {
        case View::AcetabularPinInstallationAndAcquisition:
            mUi->stackedWidgetViewer->setCurrentWidget(mUi->imageWidget);
            break;
        case View::AcetabularLandmarkAcquisition:
            mUi->stackedWidgetViewer->setCurrentWidget(mUi->multiStdWidget);
            mUi->progressBar->setVisible(true);
        default:
            break;
    }
}