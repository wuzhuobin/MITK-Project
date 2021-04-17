#include "case_manage_view.h"
#include "ui_case_manage_view.h"



CaseManageView::CaseManageView(QWidget *parent) :QWidget(parent)
{
    this->ui = new Ui::CaseManageView();
    this->ui->setupUi(this);

}

CaseManageView::~CaseManageView()
{
    delete this->ui;
}
