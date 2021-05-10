#include "MainWindow.h"
#include "IOController.h"
#include "ui_MainWindow.h"

// qt
#include <QActionGroup>
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  this->ui->setupUi(this);

  this->initializeMenu();
  this->initializeConnection();
  this->actionsTriggered(nullptr);

  QString fileName = QCoreApplication::arguments().size() > 1
                         ? QCoreApplication::arguments()[1]
                         : "";
  if (!fileName.isEmpty()) {
    IOController::getInstance()->loadScene(fileName);
    this->ui->multiWidget->InitializeMultiWidget();
    // this->SetCurrentActionIndex(0);
  }
}

MainWindow::~MainWindow() { delete this->ui; }

void MainWindow::test() {
  this->actionsTriggered(this->ui->action_Segmentation);
}

void MainWindow::initializeMenu() {
  QMenu *menuShow = new QMenu(this);
  menuShow->addAction(this->ui->action_Segmentation);
  this->ui->toolButtonShow->setMenu(menuShow);

  QMenu *menuView = new QMenu(this);
  menuView->addAction(this->ui->action_Whole_Spine_CT);
  menuView->addAction(this->ui->action_Single_Vertebra_CT);
  menuView->addAction(this->ui->action_Whole_Spine_X_Ray);
  menuView->addAction(this->ui->action_Whole_Spine_MRI);
  menuView->addAction(this->ui->action_Single_Vertebra_MRI);
  this->ui->toolButtonView->setMenu(menuView);

  QMenu *toolMenu = new QMenu(this);
  this->ui->toolButtonTools->setMenu(toolMenu);
}

void MainWindow::initializeConnection() {
  QActionGroup *actionGroup = new QActionGroup(this);
  actionGroup->setExclusive(true);
  actionGroup->addAction(this->ui->action_Segmentation);

  connect(actionGroup, &QActionGroup::triggered, this,
          &MainWindow::actionsTriggered);
}

void MainWindow::actionsTriggered(class QAction *action) {
  this->ui->stackedWidget->hide();
  if (action == this->ui->action_Segmentation) {
    this->ui->stackedWidget->setCurrentWidget(this->ui->segmentationWidget);
    this->ui->stackedWidget->show();
  }
}