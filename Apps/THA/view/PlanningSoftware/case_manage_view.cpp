#include "case_manage_view.h"
#include "ui_case_manage_view.h"
#include "theme.h"
struct CaseManageViewResourceInit
{
    CaseManageViewResourceInit()
    {
        Q_INIT_RESOURCE(resource);
    }
} init;

CaseManageView::CaseManageView(QWidget *parent)
    : QWidget(parent), ui(new Ui::CaseManageView), patientTableModel(1, 3)
{
    this->ui->setupUi(this);
    this->ui->patientTableView->setModel(&this->patientTableModel);

    this->patientTableModel.setItem(0, 0, new QStandardItem("unknown"));
    this->patientTableModel.setItem(0, 1, new QStandardItem("1013618784"));
    this->patientTableModel.setItem(0, 2, new QStandardItem("20200604"));

    QStringList horizontalHeaderLabels;
    horizontalHeaderLabels << tr("Modality");
    horizontalHeaderLabels << tr("Study Date");
    horizontalHeaderLabels << tr("Study ID");
    horizontalHeaderLabels << tr("Name");
    horizontalHeaderLabels << tr("Gender");
    horizontalHeaderLabels << tr("Age");
    horizontalHeaderLabels << tr("Diagnosis");
    horizontalHeaderLabels << tr("Size");

    this->ui->tableWidget->setRowCount(1);
    this->ui->tableWidget->setColumnCount(8);
    //this->ui->tableWidget->horizontalHeader()->setVisible(true);
    this->ui->tableWidget->setHorizontalHeaderLabels(horizontalHeaderLabels);
    this->ui->tableWidget->setItem(0, 0, new QTableWidgetItem("CT"));
    this->ui->tableWidget->setItem(0, 1, new QTableWidgetItem("20200604"));
    this->ui->tableWidget->setItem(0, 2, new QTableWidgetItem("1013618784"));
    this->ui->tableWidget->setItem(0, 3, new QTableWidgetItem("unknown"));
    this->ui->tableWidget->setItem(0, 4, new QTableWidgetItem(tr("Male")));
    this->ui->tableWidget->setItem(0, 5, new QTableWidgetItem(tr("50")));
    this->ui->tableWidget->setItem(0, 6, new QTableWidgetItem(tr("N/A")));
    this->ui->tableWidget->setItem(0, 7, new QTableWidgetItem(tr("747MB")));

    this->ui->picture1->setVisible(false);
    this->ui->picture2->setVisible(false);
    this->ui->picture3->setVisible(false);

    this->ui->labelTextBackground1->setVisible(false);
    this->ui->labelTextBackground2->setVisible(false);
    this->ui->labelTextBackground3->setVisible(false);

    this->ui->labelTextCategory1->setVisible(false);
    this->ui->labelTextCategory2->setVisible(false);
    this->ui->labelTextCategory3->setVisible(false);

    this->ui->labelTextSliceNum1->setVisible(false);
    this->ui->labelTextSliceNum2->setVisible(false);
    this->ui->labelTextSliceNum3->setVisible(false);

    Theme::instance()->setFlowViewStyle(this);
    Theme::instance()->setleftWidget(ui->widget);
    Theme::instance()->setLabelStyle_14pxWhite(ui->userNameLabel);

    Theme::instance()->setUserNameLabelStyle(ui->userNameLabel);
    //set Button style
    Theme::instance()->setButtonStyle(ui->importBtn);
    Theme::instance()->setButtonStyle(ui->manageBtn);
    Theme::instance()->setButtonStyle(ui->nextStepBtn);
    Theme::instance()->setButtonStyle(ui->selectAllBtn);
    Theme::instance()->setButtonStyle(ui->deleteBtn);
    Theme::instance()->setButtonStyle(ui->exportBtn);

    Theme::instance()->setLabelPicInfoStyle(ui->picture1, ui->labelTextBackground1, ui->labelTextCategory1, ui->labelTextSliceNum1);
    Theme::instance()->setLabelPicInfoStyle(ui->picture2, ui->labelTextBackground2, ui->labelTextCategory2, ui->labelTextSliceNum2);
    Theme::instance()->setLabelPicInfoStyle(ui->picture3, ui->labelTextBackground3, ui->labelTextCategory3, ui->labelTextSliceNum3);

    //tableView style init
    Theme::instance()->setTableViewStyle(ui->patientTableView);
    ui->patientTableView->horizontalHeader()->hide(); // hide column title
    Theme::instance()->setLabelStyle_14pxGray(ui->labelName);
    Theme::instance()->setLabelStyle_14pxGray(ui->labelID);
    Theme::instance()->setLabelStyle_14pxGray(ui->labelImportTime);

    // set tableWidget style
    Theme::instance()->setTableWidgetStyle(ui->tableWidget);
    Theme::instance()->setTableWidgetStyle(ui->tableWidgetPlan);

    //set tabwidget style
    Theme::instance()->setTabWidgetStyle(ui->tabWidget);
    ui->tabWidget->setCurrentIndex(0);

    connect(this, &CaseManageView::finished, this, &CaseManageView::hide);
}

CaseManageView::~CaseManageView()
{
    delete this->ui;
}

void CaseManageView::on_nextStepBtn_clicked(bool checked)
{
    emit this->finished();
}
