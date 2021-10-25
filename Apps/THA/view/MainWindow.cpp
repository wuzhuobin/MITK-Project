#include "MainWindow.h"

#include "AcetabularPrepWidget.h"
#include "AdminMenu.h"
#include "CasePlanningWidget.h"
#include "IOController.h"
#include "ui_MainWindow.h"

// qt
#include <QActionGroup>
#include <QFileDialog>

// mitk
#include <mitkRenderingManager.h>

const static struct MainWindowResourceInit
{
    MainWindowResourceInit() { Q_INIT_RESOURCE(resources); }
} GInit;

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), mUi(new Ui::MainWindow), mActionGroup(new QActionGroup(this))
{
    mUi->setupUi(this);

    mUi->toolButtonAdmin->setMenu(new AdminMenu);

    connect(mActionGroup, &QActionGroup::triggered, this, &MainWindow::onActionsTriggered);
    connect(IOController::getInstance(), &IOController::sceneLoaded, this, &MainWindow::onsceneLoaded);
    // the actions should be added in correct order.

    // Login
    mActionGroup->addAction(mUi->action_Login);

    // Manage Case
    mActionGroup->addAction(mUi->action_Manage_Case);

    // CasePlanningWidget
    mActionGroup->addAction(mUi->action_Pelvis_CT_Landmark);
    mActionGroup->addAction(mUi->action_Implant_Planning);
    mActionGroup->addAction(mUi->action_Broach_Tracking);
    connect(mUi->action_Implant_Planning,
            &QAction::triggered,
            mUi->pageCasePlanning,
            &CasePlanningWidget::Action_Implant_Planning_triggered);
    connect(mUi->action_Broach_Tracking,
            &QAction::triggered,
            mUi->pageCasePlanning,
            &CasePlanningWidget::Action_Broach_Tracking_triggered);
    connect(mUi->action_Pelvis_CT_Landmark,
            &QAction::triggered,
            mUi->pageCasePlanning,
            &CasePlanningWidget::Action_Pelvis_CT_Landmark_triggered);

    // AcetabularPrepWidget
    mActionGroup->addAction(mUi->action_RIO_Registratoin);
    mActionGroup->addAction(mUi->action_Pelvis_Checkpoint);
    mActionGroup->addAction(mUi->action_Pelvis_Landmark);
    mActionGroup->addAction(mUi->action_Pelvis_Registration);
    mActionGroup->addAction(mUi->action_Cup_Reaming);
    mActionGroup->addAction(mUi->action_Cup_Impaction);
    connect(mUi->action_RIO_Registratoin,
            &QAction::triggered,
            mUi->pageAcetabularPrep,
            &AcetabularPrepWidget::Action_RIO_Registratoin_triggered);
    connect(mUi->action_Pelvis_Checkpoint,
            &QAction::triggered,
            mUi->pageAcetabularPrep,
            &AcetabularPrepWidget::Action_Pelvis_Checkpoint_triggered);
    connect(mUi->action_Pelvis_Landmark,
            &QAction::triggered,
            mUi->pageAcetabularPrep,
            &AcetabularPrepWidget::Action_Pelvis_Landmark_triggered);
    connect(mUi->action_Pelvis_Registration,
            &QAction::triggered,
            mUi->pageAcetabularPrep,
            &AcetabularPrepWidget::Action_Pelvis_Registration_triggered);
    connect(mUi->action_Cup_Reaming,
            &QAction::triggered,
            mUi->pageAcetabularPrep,
            &AcetabularPrepWidget::Action_Cup_Reaming_triggered);
    connect(mUi->action_Cup_Impaction,
            &QAction::triggered,
            mUi->pageAcetabularPrep,
            &AcetabularPrepWidget::Action_Cup_Impaction_triggered);
    
    // Final Results
    mActionGroup->addAction(mUi->action_Final_Result);

    mitk::DataStorage* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
    mUi->levelWindow->SetDataStorage(ds);

    // hide contrast, luminance, zoom buttons
    mUi->toolButtonContrast->setVisible(false);
    mUi->toolButtonLuminance->setVisible(false);
    mUi->toolButtonZoom->setVisible(false);

    QString fileName = QCoreApplication::arguments().size() > 1 ? QCoreApplication::arguments()[1] : "";
    if (!fileName.isEmpty())
    {
        IOController::getInstance()->loadScene(fileName);
    }
    setCurrentActionIndex(0);
}

MainWindow::~MainWindow()
{
    delete mUi;
}

void MainWindow::test()
{
    MITK_INFO << __func__;
    IOController::getInstance()->loadScene(IOController::getBaseProject());
    setCurrentActionIndex(mActionGroup->actions().indexOf(mUi->action_Pelvis_CT_Landmark));
    // mUi->stackedWidgetViewer->setCurrentWidget(mUi->pageCaseManagement);
}

void MainWindow::setCurrentActionIndex(int index)
{
    // bounding
    MITK_INFO << __func__;
    MITK_INFO << index;
    if (index >= mActionGroup->actions().size())
    {
        mCurrentActionIndex = mActionGroup->actions().size() - 1;
    }
    else if (index < 0)
    {
        mCurrentActionIndex = 0;
    }
    else
    {
        mCurrentActionIndex = index;
    }

    mActionGroup->actions()[mCurrentActionIndex]->trigger();
}

void MainWindow::showFinalResult()
{

}

void MainWindow::on_radioButtonOptions_toggled(bool checked)
{
    if (checked)
    {
        QString fileName = QFileDialog::getOpenFileName(this, "Scene", QString(), tr("MITK (*.mitk)"));
        IOController::getInstance()->loadScene(fileName);
    }
}

void MainWindow::on_buttonGroupMode_buttonClicked(QAbstractButton* button) const
{
    MITK_INFO << __func__ << ' ' << button->objectName().toStdString();
    for (QAbstractButton* button_ : mUi->buttonGroupMode->buttons())
    {
        if (button_ != button)
        {
            button_->setChecked(false);
        }
    }

    if (mUi->toolButtonPrepOpMode->isChecked())
    {
        mUi->multiWidget->SetMode(THAStdMultiWidget::MODES::MODE_PRE_OP);
    }
    else if (mUi->toolButtonCupPlanMode->isChecked())
    {
        mUi->multiWidget->SetMode(THAStdMultiWidget::MODES::MODE_CUP_PLAN);
    }
    else if (mUi->toolButtonStemPlanMode->isChecked())
    {
        mUi->multiWidget->SetMode(THAStdMultiWidget::MODES::MODE_STEM_PLAN);
    }
    else if (mUi->toolButtonReducedMode->isChecked())
    {
        mUi->multiWidget->SetMode(THAStdMultiWidget::MODES::MODE_REDUCED);
    }
    else
    {
        mUi->multiWidget->SetMode(THAStdMultiWidget::MODES::MODE_DEFAULT);
    }
}

void MainWindow::on_buttonGroupView_buttonClicked(QAbstractButton* button) const
{
    MITK_INFO << __func__ << ' ' << button->objectName().toStdString();
    for (QAbstractButton* button_ : mUi->buttonGroupView->buttons())
    {
        if (button_ != button)
        {
            button_->setChecked(false);
        }
    }

    if (mUi->toolButton3DSlicerView->isChecked())
    {
        mUi->multiWidget->SetView(THAStdMultiWidget::VIEWS::VIEW_3D_SLICER);
    }
    else if (mUi->toolButtonCTView->isChecked())
    {
        mUi->multiWidget->SetView(THAStdMultiWidget::VIEWS::VIEW_CT);
    }
    else if (mUi->toolButtonReamingView->isChecked())
    {
        mUi->multiWidget->SetView(THAStdMultiWidget::VIEWS::VIEW_REAMING);
    }
    else if (mUi->toolButtonXRayView->isChecked())
    {
        mUi->multiWidget->SetView(THAStdMultiWidget::VIEWS::VIEW_X_RAY);
    }
    else
    {
        mUi->multiWidget->SetView(THAStdMultiWidget::VIEWS::VIEW_DEFAULT);
    }
}

void MainWindow::on_buttonGroupWorkflow_buttonClicked(QAbstractButton* button)
{
    if (button == mUi->radioButtonPreOpRIOCheck)
    {
    }
    else if (button == mUi->radioButtonFemoralPrep)
    {
    }
    else if (button == mUi->radioButtonCasePlanning)
    {
        setCurrentActionIndex(mActionGroup->actions().indexOf(mUi->action_Pelvis_CT_Landmark));
    }
    else if (button == mUi->radioButtonAcetabularPrep)
    {
        setCurrentActionIndex(mActionGroup->actions().indexOf(mUi->action_RIO_Registratoin));
    }
    else if (button == mUi->radioButtonFinalResult)
    {
        setCurrentActionIndex(mActionGroup->actions().indexOf(mUi->action_Final_Result));
    }
}

void MainWindow::on_pushButtonNext_clicked(bool /*checked*/)
{
    MITK_INFO << __func__;

    setCurrentActionIndex(++mCurrentActionIndex);
}

void MainWindow::on_pushButtonBack_clicked(bool /*checked*/)
{
    MITK_INFO << __func__;
    setCurrentActionIndex(--mCurrentActionIndex);
}

void MainWindow::on_pageLogin_accepted()
{
    MITK_INFO << __func__;
    setCurrentActionIndex(++mCurrentActionIndex);
}

void MainWindow::on_pageCaseManagement_accepted()
{
    MITK_INFO << __func__;
    setCurrentActionIndex(++mCurrentActionIndex);
}

void MainWindow::onActionsTriggered(QAction* action) const
{
    if (action == mActionGroup->actions().last())
    {
        mUi->pushButtonNext->setEnabled(false);
        mUi->pushButtonBack->setEnabled(true);
    }
    else if (action == mActionGroup->actions().first())
    {
        mUi->pushButtonNext->setEnabled(true);
        mUi->pushButtonBack->setEnabled(false);
    }
    else
    {
        mUi->pushButtonNext->setEnabled(true);
        mUi->pushButtonBack->setEnabled(true);
    }

    if (mActionGroup->actions().indexOf(action) <= mActionGroup->actions().indexOf(mUi->action_Login))
    {
        mUi->frameWorkflow->setVisible(false);
        mUi->pushButtonBack->setVisible(false);
        mUi->pushButtonNext->setVisible(false);
        mUi->stackedWidget->setCurrentWidget(mUi->pageEmpty);
    }
    else if (mActionGroup->actions().indexOf(action) <= mActionGroup->actions().indexOf(mUi->action_Manage_Case))
    {
        mUi->frameWorkflow->setVisible(true);
        mUi->pushButtonBack->setVisible(true);
        mUi->pushButtonNext->setVisible(true);
        mUi->stackedWidget->setCurrentWidget(mUi->pageEmpty);
    }
    else if (mActionGroup->actions().indexOf(action) <= mActionGroup->actions().indexOf(mUi->action_Broach_Tracking))
    {
        mUi->frameWorkflow->setVisible(true);
        mUi->pushButtonBack->setVisible(true);
        mUi->pushButtonNext->setVisible(true);
        mUi->radioButtonCasePlanning->setChecked(true);
        mUi->stackedWidget->setCurrentWidget(mUi->pageCasePlanning);
    }
    else if (mActionGroup->actions().indexOf(action) <= mActionGroup->actions().indexOf(mUi->action_Cup_Impaction))
    {
        mUi->frameWorkflow->setVisible(true);
        mUi->pushButtonBack->setVisible(true);
        mUi->pushButtonNext->setVisible(true);
        mUi->radioButtonAcetabularPrep->setChecked(true);
        mUi->stackedWidget->setCurrentWidget(mUi->pageAcetabularPrep);
    }

    if (action == mUi->action_Login)
    {
        mUi->stackedWidgetViewer->setCurrentWidget(mUi->pageLogin);
    }
    else if (action == mUi->action_Manage_Case)
    {
        mUi->stackedWidgetViewer->setCurrentWidget(mUi->pageCaseManagement);
    }
    else if (action == mUi->action_RIO_Registratoin)
    {
        mUi->stackedWidgetViewer->setCurrentWidget(mUi->pageImage);
        mUi->imageWidget->SetMode(ImageWidget::MODE::RIO_REGISTRATION);
    }
    else if (action == mUi->action_Pelvis_Checkpoint)
    {
        mUi->stackedWidgetViewer->setCurrentWidget(mUi->pageImage);
        mUi->imageWidget->SetMode(ImageWidget::MODE::PELVIS_CHECKPOINT);
    }
    else if (action == mUi->action_Pelvis_Landmark)
    {
        mUi->stackedWidgetViewer->setCurrentWidget(mUi->pageMultiWidget);
        mUi->multiWidget->SetOther(true);
    }
    else if (action == mUi->action_Pelvis_Registration)
    {
        mUi->stackedWidgetViewer->setCurrentWidget(mUi->pageMultiWidget);
        mUi->multiWidget->SetOther(true);
    }
    else if (action == mUi->action_Cup_Reaming)
    {
        mUi->stackedWidgetViewer->setCurrentWidget(mUi->multiWidget);
        mUi->multiWidget->SetOther(true);
    }
    else if (action == mUi->action_Cup_Impaction)
    {
        mUi->stackedWidgetViewer->setCurrentWidget(mUi->multiWidget);
        mUi->multiWidget->SetOther(true);
    }
    else if (action == mUi->action_Final_Result)
    {
        mUi->stackedWidgetViewer->setCurrentWidget(mUi->pageFinalResult);
    }
    else
    {
        mUi->multiWidget->SetOther(false);
        mUi->stackedWidgetViewer->setCurrentWidget(mUi->pageMultiWidget);
    }
}

void MainWindow::onsceneLoaded()
{
    mUi->multiWidget->InitializeMultiWidget();
}