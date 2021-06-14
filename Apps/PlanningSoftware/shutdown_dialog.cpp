
#include "shutdown_dialog.h"
#include "ui_shutdown_dialog.h"
#include "theme.h"

ShutdownDialog::ShutdownDialog(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::ShutdownDialog)
{
    m_ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    initialize();
}

ShutdownDialog::~ShutdownDialog()
{
    delete m_ui;
}

void ShutdownDialog::initialize()
{
    // Theme::instance()->setFlowViewStyle(this);
    this->setWindowModality(Qt::WindowModality::NonModal);

    // Theme::instance()->setTitleWidgetStyle(m_ui->widget); //
    connect(m_ui->poweroffBtn, &QPushButton::clicked, this, [=]() {
        QApplication::exit();
    });
    connect(m_ui->cancelBtn, &QPushButton::clicked, this, [=]() { this->close(); });
    connect(m_ui->closeBtn, &QPushButton::clicked, this, [=]() { this->close(); });
}