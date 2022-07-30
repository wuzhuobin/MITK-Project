/**
 * @file SCBDentalPlanningPipeline.cxx
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "SCBDentalPlanningPipeline.h"

#include "AffineBaseDataInteractor3D.h"

// qt
#include <QMessageBox>

// mitk
#include <mitkDataNode.h>
#include <mitkDataStorage.h>
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkSurface.h>
#include <usModuleRegistry.h>

SCBDentalPlanningPipeline::SCBDentalPlanningPipeline(QWidget* parent) :
    QWidget(parent)
{
  this->setupUi(this);
}

void SCBDentalPlanningPipeline::on_pushButtonAdvancedOption_clicked()
{
  //   SCBDentalReconstructionOptionDialog dialog(this);
  //   dialog.setWindowFlag(Qt::FramelessWindowHint);
  //   dialog.exec();
}

void SCBDentalPlanningPipeline::on_pushButtonAutoReconstruction_clicked()
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

  auto* nerveLeftNode = ds->GetNamedNode("nerve_left");
  nerveLeftNode->SetVisibility(true);
  auto* nerveRightNode = ds->GetNamedNode("nerve_right");
  nerveRightNode->SetVisibility(true);

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void SCBDentalPlanningPipeline::on_tableWidgetTeeth_cellClicked(int row,
                                                                int column)
{
  Q_UNUSED(column);
  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   SCBDentalFixture* fixture = scene->getDataByAlias<SCBDentalFixture>(
  //       "Fixture" + QString::number(row + 1));

  //   if (!fixture)
  //   {
  //     this->pushButtonEnterImplantLibrary->hide();
  //     return;
  //   }
  //   this->pushButtonEnterImplantLibrary->show();
  //   this->labelID->setText(QString::number(fixture->getToothPosition().toFDI()));
  //   this->labelBrand->setText(fixture->getDentalFixtureBrand());
  //   this->labelModel->setText(fixture->getDentalFixtureModel());
  //   this->labelShape->setText(fixture->getDentalFixtureShape());
  //   this->labelLength->setText(
  //       QString::number(fixture->getDentalFixtureLength()));
  //   this->labelRadius->setText(
  //       QString::number(fixture->getDentalFixtureRadius()));
}

void SCBDentalPlanningPipeline::on_buttonGroupTeethPosition_buttonToggled(
    QAbstractButton* button, bool checked)
{
  auto* ds = mitk::RenderingManager::GetInstance()->GetDataStorage();
  if (!checked)
  {
    if (QMessageBox::No ==
        QMessageBox::question(this,
                              tr("Delete Implant"),
                              tr("Are you sure to delete this implant?")))
    {
      this->buttonGroupTeethPosition->blockSignals(true);
      button->setChecked(true);
      this->buttonGroupTeethPosition->blockSignals(false);
      return;
    }
    auto* implantNode =
        ds->GetNamedNode((QString("implant_") + button->text()).toStdString());
    if (!implantNode)
    {
      return;
    }
    ds->Remove(implantNode);
  }
  else
  {
    auto* originImplant = ds->GetNamedObject<mitk::Surface>("implant");

    auto implant = originImplant->Clone();

    auto implantNode = mitk::DataNode::New();
    implantNode->SetName((QString("implant_") + button->text()).toStdString());
    implantNode->SetData(implant);
    implantNode->SetVisibility(true);
    implantNode->SetBoolProperty("pickable", true);
    ds->Add(implantNode);

    auto affineDataInteractor = AffineBaseDataInteractor3D::New();
    affineDataInteractor->LoadStateMachine(
        "AffineInteraction3D.xml",
        us::ModuleRegistry::GetModule("MitkDataTypesExt"));
    affineDataInteractor->SetEventConfig(
        "AffineMouseConfig.xml",
        us::ModuleRegistry::GetModule("MitkDataTypesExt"));
    affineDataInteractor->SetDataNode(implantNode);
  }

  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void SCBDentalPlanningPipeline::addImplant(QString uniqueName)
{
  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   SCBDentalImplant* implant =
  //       scene->getDataByUniqueName<SCBDentalImplant>(uniqueName);
  //   if (!implant)
  //   {
  //     return;
  //   }
  //   this->buttonGroupTeethPosition.blockSignals(true);
  //   this->buttonGroupTeethPosition.button(implant->getToothPosition().toFDI())
  //       ->setChecked(true);
  //   this->buttonGroupTeethPosition.blockSignals(false);
  //   QAbstractButton* button = this->buttonGroupTeethPosition.button(
  //       implant->getToothPosition().toFDI());
  //   SCBDentalHub* hub = SCBDentalHub::self;
  //   for (int i = 0; i < SCBDentalHub::NUM_OF_ORTHOGONAL_VIEWER; ++i)
  //   {
  //     SCBPolyDataSourceWidget* crownWidget =
  //         hub->widgets[i]->ProducePolyDataSourceWidgets();
  //     crownWidget->SetSCBPolyData(implant);
  //     hub->widgets[i]->SetOneOfPolyDataSourceWidgetsEnabled(crownWidget,
  //     true);
  //   }
  //   SCBDentalFixture* fixture = qobject_cast<SCBDentalFixture*>(implant);
  //   if (!fixture)
  //   {
  //     return;
  //   }
  //   int id = fixture->getToothPosition().toFDI();
  //   this->tableWidgetTeeth->setRowHidden(id - 1, false);
  //   this->tableWidgetTeeth->setItem(
  //       id - 1, 0, new QTableWidgetItem(QString::number(id)));
  //   this->tableWidgetTeeth->setItem(
  //       id - 1,
  //       1,
  //       new
  //       QTableWidgetItem(QString::number(fixture->getDentalFixtureRadius())));
  //   this->tableWidgetTeeth->setItem(
  //       id - 1,
  //       2,
  //       new
  //       QTableWidgetItem(QString::number(fixture->getDentalFixtureLength())));
  //   this->pushButtonEnterImplantLibrary->hide();
}

void SCBDentalPlanningPipeline::removeImplant(QString uniqueName)
{
  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   SCBDentalHub* hub = SCBDentalHub::self;
  //   SCBDentalImplant* implant =
  //       scene->getDataByUniqueName<SCBDentalImplant>(uniqueName);
  //   if (!implant)
  //   {
  //     return;
  //   }
  //   this->buttonGroupTeethPosition.blockSignals(true);
  //   this->buttonGroupTeethPosition.button(implant->getToothPosition().toFDI())
  //       ->setChecked(false);
  //   this->buttonGroupTeethPosition.blockSignals(false);

  //   QList<vtkSmartPointer<SCBPolyDataSourceWidget>> polyDataSourceWidgets =
  //       hub->widgets[0]->GetPolyDataSourceWidgets();
  //   int index = 0;
  //   for (QList<vtkSmartPointer<SCBPolyDataSourceWidget>>::const_iterator cit
  //   =
  //            polyDataSourceWidgets.cbegin();
  //        cit != polyDataSourceWidgets.cend();
  //        ++cit)
  //   {
  //     if ((*cit)->GetSCBPolyData() == implant)
  //     {
  //       for (int i = 0; i < SCBDentalHub::NUM_OF_ORTHOGONAL_VIEWER; ++i)
  //       {
  //         hub->widgets[i]->SetOneOfPolyDataSourceWidgetsEnabled(
  //             hub->widgets[i]->GetPolyDataSourceWidgets()[index], false);
  //       }
  //       for (int i = 0; i < SCBDentalHub::NUM_OF_ORTHOGONAL_VIEWER; ++i)
  //       {
  //         hub->widgets[i]->DestroyPolyDataSourceWidgets(
  //             hub->widgets[i]->GetPolyDataSourceWidgets()[index]);
  //         hub->mainWindow->getViewer(i)->Render();
  //       }

  //       this->tableWidgetTeeth->setRowHidden(
  //           implant->getToothPosition().toFDI() - 1, true);
  //       this->pushButtonEnterImplantLibrary->hide();
  //       return;
  //     }
  //     ++index;
  //   }
  //   if (index == 0)
  //   {
  //     qCritical() << "There is none SCBPolyDataSourceWidget. ";
  //     return;
  //   }
  //   else if (index == polyDataSourceWidgets.size())
  //   {
  //     qCritical() << "Cannot find a SCBPolyDataSourceWidget with "
  //                    "SCBDentalImplant whose unique "
  //                    "name is"
  //                 << implant->getUniqueName();
  //     qCritical() << "It is supposed to be existed. ";
  //     return;
  //   }
}