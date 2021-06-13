#include "CaseManagementWidget.h"

#include "ui_CaseManagementWidget.h"
// #include "theme.h"
#include "AdminMenu.h"

// mitk
#include <QmitkDicomExternalDataWidget.h>

struct CaseManagementWidgetResourceInit
{
    CaseManagementWidgetResourceInit() { Q_INIT_RESOURCE(resource); }
} gInit;

CaseManagementWidget::CaseManagementWidget(QWidget* parent) :
    QWidget(parent), mUi(new Ui::CaseManagementWidget), mPatientTableModel(1, 3)
{
    mUi->setupUi(this);
    mUi->patientTableView->setModel(&mPatientTableModel);
    mUi->toolButtonAdmin->setMenu(new AdminMenu);

    mPatientTableModel.setItem(0, 0, new QStandardItem("unknown"));
    mPatientTableModel.setItem(0, 1, new QStandardItem("1013618784"));
    mPatientTableModel.setItem(0, 2, new QStandardItem("20200604"));

    QStringList horizontalHeaderLabels;
    horizontalHeaderLabels << tr("Modality");
    horizontalHeaderLabels << tr("Study Date");
    horizontalHeaderLabels << tr("Study ID");
    horizontalHeaderLabels << tr("Name");
    horizontalHeaderLabels << tr("Gender");
    horizontalHeaderLabels << tr("Age");
    horizontalHeaderLabels << tr("Diagnosis");
    horizontalHeaderLabels << tr("Size");

    mUi->tableWidget->setRowCount(1);
    mUi->tableWidget->setColumnCount(8);
    // mUi->tableWidget->horizontalHeader()->setVisible(true);
    mUi->tableWidget->setHorizontalHeaderLabels(horizontalHeaderLabels);
    mUi->tableWidget->setItem(0, 0, new QTableWidgetItem("CT"));
    mUi->tableWidget->setItem(0, 1, new QTableWidgetItem("20200604"));
    mUi->tableWidget->setItem(0, 2, new QTableWidgetItem("1013618784"));
    mUi->tableWidget->setItem(0, 3, new QTableWidgetItem("unknown"));
    mUi->tableWidget->setItem(0, 4, new QTableWidgetItem(tr("Male")));
    mUi->tableWidget->setItem(0, 5, new QTableWidgetItem(tr("50")));
    mUi->tableWidget->setItem(0, 6, new QTableWidgetItem(tr("N/A")));
    mUi->tableWidget->setItem(0, 7, new QTableWidgetItem(tr("747MB")));

    mUi->picture1->setVisible(false);
    mUi->picture2->setVisible(false);
    mUi->picture3->setVisible(false);

    mUi->labelTextBackground1->setVisible(false);
    mUi->labelTextBackground2->setVisible(false);
    mUi->labelTextBackground3->setVisible(false);

    mUi->labelTextCategory1->setVisible(false);
    mUi->labelTextCategory2->setVisible(false);
    mUi->labelTextCategory3->setVisible(false);

    mUi->labelTextSliceNum1->setVisible(false);
    mUi->labelTextSliceNum2->setVisible(false);
    mUi->labelTextSliceNum3->setVisible(false);

    //   Theme::instance()->setFlowViewStyle(this);
    //   Theme::instance()->setleftWidget(mUi->widget);
    //   Theme::instance()->setLabelStyle_14pxWhite(mUi->userNameLabel);

    //   Theme::instance()->setUserNameLabelStyle(mUi->userNameLabel);
    //   // set Button style
    //   Theme::instance()->setButtonStyle(mUi->importBtn);
    //   Theme::instance()->setButtonStyle(mUi->manageBtn);
    //   Theme::instance()->setButtonStyle(mUi->nextStepBtn);
    //   Theme::instance()->setButtonStyle(mUi->selectAllBtn);
    //   Theme::instance()->setButtonStyle(mUi->deleteBtn);
    //   Theme::instance()->setButtonStyle(mUi->exportBtn);

    //   Theme::instance()->setLabelPicInfoStyle(
    //       mUi->picture1, mUi->labelTextBackground1, mUi->labelTextCategory1,
    //       mUi->labelTextSliceNum1);
    //   Theme::instance()->setLabelPicInfoStyle(
    //       mUi->picture2, mUi->labelTextBackground2, mUi->labelTextCategory2,
    //       mUi->labelTextSliceNum2);
    //   Theme::instance()->setLabelPicInfoStyle(
    //       mUi->picture3, mUi->labelTextBackground3, mUi->labelTextCategory3,
    //       mUi->labelTextSliceNum3);

    //   // tableView style init
    //   Theme::instance()->setTableViewStyle(mUi->patientTableView);
    //   mUi->patientTableView->horizontalHeader()->hide(); // hide column title
    //   Theme::instance()->setLabelStyle_14pxGray(mUi->labelName);
    //   Theme::instance()->setLabelStyle_14pxGray(mUi->labelID);
    //   Theme::instance()->setLabelStyle_14pxGray(mUi->labelImportTime);

    //   // set tableWidget style
    //   Theme::instance()->setTableWidgetStyle(mUi->tableWidget);
    //   Theme::instance()->setTableWidgetStyle(mUi->tableWidgetPlan);

    //   // set tabwidget style
    //   Theme::instance()->setTabWidgetStyle(mUi->tabWidget);
    mUi->tabWidget->setCurrentIndex(0);

    connect(this, &CaseManagementWidget::finished, this, &CaseManagementWidget::hide);
}

CaseManagementWidget::~CaseManagementWidget()
{
    delete mUi;
}

void CaseManagementWidget::on_nextStepBtn_clicked(bool checked)
{
    emit finished();
}
