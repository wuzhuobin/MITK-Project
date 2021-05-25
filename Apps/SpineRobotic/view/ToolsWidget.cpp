#include "ToolsWidget.h"
#include "ui_ToolsWidget.h"
#include <qwidget.h>

ToolsWidget::ToolsWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ToolsWidget) {
    
    this->ui->setupUi(this);
}

ToolsWidget::~ToolsWidget()
{
    delete this->ui;
}