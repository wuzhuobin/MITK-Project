#include "SegmentationWidget.h"
#include "ui_SegmentationWidget.h"

// mitk
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkGeometry3D.h>
#include <mitkGeometryData.h>
#include <mitkImage.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkBoundingShapeObjectFactory.h>
#include <usModuleRegistry.h>

// qt
#include <QToolButton>
#include <QButtonGroup>


SegmentationWidget::SegmentationWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::SegmentationWidget),
      boundingShapeInteractor(mitk::BoundingShapeInteractor::New()) {

  this->ui->setupUi(this);

  QButtonGroup *groupBox = new QButtonGroup(this);
  groupBox->setExclusive(true);


  for (int i = 0; i < 7; ++i) {
    QToolButton *toolButton = new QToolButton;
    toolButton->setObjectName("toolButtonC" + QString::number(i + 1));
    toolButton->setText("C" + QString::number(i + 1));
    toolButton->setCheckable(true);

    this->ui->gridLayoutCervial->addWidget(toolButton, i / 4, i % 4);
    groupBox->addButton(toolButton);
  }

  for (int i = 0; i < 12; ++i) {
    QToolButton *toolButton = new QToolButton;
    toolButton->setObjectName("toolButtonT" + QString::number(i + 1));
    toolButton->setText("T" + QString::number(i + 1));
    toolButton->setCheckable(true);

    this->ui->gridLayoutThracic->addWidget(toolButton, i / 4, i % 4);
    groupBox->addButton(toolButton);
  }

  for (int i = 0; i < 5; ++i) {
    QToolButton *toolButton = new QToolButton;
    toolButton->setObjectName("toolButtonL" + QString::number(i + 1));
    toolButton->setText("L" + QString::number(i + 1));
    toolButton->setCheckable(true);

    this->ui->gridLayoutLumbar->addWidget(toolButton, i / 3, i % 3);
    groupBox->addButton(toolButton);
  }

  for (int i = 0; i < 5; ++i) {
    QToolButton *toolButton = new QToolButton;
    toolButton->setObjectName("toolButtonS" + QString::number(i + 1));
    toolButton->setText("S" + QString::number(i + 1));
    toolButton->setCheckable(true);

    this->ui->gridLayoutSacral->addWidget(toolButton, i / 3, i % 3);
  }

  mitk::RegisterBoundingShapeObjectFactory();
  this->boundingShapeInteractor->LoadStateMachine(
      "BoundingShapeInteraction.xml",
      us::ModuleRegistry::GetModule("MitkBoundingShape"));
  this->boundingShapeInteractor->SetEventConfig(
      "BoundingShapeMouseConfig.xml",
      us::ModuleRegistry::GetModule("MitkBoundingShape"));

  connect(this->ui->pushButtonMarkerSegmentation, &QPushButton::clicked, this,
          &SegmentationWidget::on_pushButtonMarkerSegmentation_clicked);
  connect(this->ui->pushButtonLabeling, &QPushButton::clicked, this,
          &SegmentationWidget::on_pushButtonLabeling_clicked);
  this->ui->stackedWidget->setCurrentWidget(this->ui->pageMarkerSegmentation);
}

SegmentationWidget::~SegmentationWidget() { delete this->ui; }

void SegmentationWidget::on_pushButtonMarkerSegmentation_clicked(bool checked) {
  
  this->ui->stackedWidget->setCurrentWidget(this->ui->pageMarkerSegmentation);

  // mitk::DataStorage *ds =
  //     mitk::RenderingManager::GetInstance()->GetDataStorage();

  // mitk::Image *image = ds->GetNamedObject<mitk::Image>("image");

  // mitk::GeometryData::Pointer geoData = mitk::GeometryData::New();
  // geoData->SetGeometry(image->GetGeometry());

  // mitk::DataNode::Pointer geoDataNode = mitk::DataNode::New();
  // geoDataNode->SetData(geoData);

  // ds->Add(geoDataNode);
  // // geoDataNode->SetVisibility(true);

  // this->boundingShapeInteractor->EnableInteraction(true);
  // this->boundingShapeInteractor->SetDataNode(geoDataNode);

  // mitk::RenderingManager::GetInstance()->RequestUpdateAll();
  
}

void SegmentationWidget::on_pushButtonLabeling_clicked(bool checked) {

  this->ui->stackedWidget->setCurrentWidget(this->ui->pageLabel);
}