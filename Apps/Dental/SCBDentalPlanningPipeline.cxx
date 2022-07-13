// me
#include "SCBDentalPlanningPipeline.h"
// #include "SCBDentalMainWindow.h"
// #include "ui_SCBDentalMainWindow.h"
// #include "SCBDentalImplantLibrary.h"
// #include "../Src/MainWindow/SCBHub.h"
// #include "../Src/MainWindow/SCBMainWindow.h"
// #include "SCBScene.h"
// #include "SCBPolyDataSourceWidget.h"
// #include "SCBWidgetCollection.h"
// #include "SCBDentalMainWindow.h"
// #include "SCBNonPlanarViewer.h"
// #include "SCBDentalReconstructionOptionDialog.h"
// #include "SCBPolyData.h"
// #include "SCBPolyDataActor.h"
// #include "SCBDentalHub.h"

// qt
#include <QDebug>
#include <QMessageBox>
#include <QStandardItemModel>

// vtk
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>

const QString IMPLANT("IMPLANT");
const QString TEETH("TEETH");

SCBDentalPlanningPipeline::SCBDentalPlanningPipeline(QWidget* parent) :
    QWidget(parent)
{
  this->setupUi(this);
  this->pushButtonEnterImplantLibrary->hide();
  this->tableWidgetTeeth->setRowCount(47);
  for (int i = 0; i < 47; ++i)
  {
    this->tableWidgetTeeth->hideRow(i);
  }
  this->buttonGroupTeethPosition.setExclusive(false);
  QList<QPushButton*> pushButtonTeethPositions =
      this->findChildren<QPushButton*>(
          QRegExp("pushButtonTeethPosition[1-9]{2}"));
  for (QList<QPushButton*>::const_iterator cit =
           pushButtonTeethPositions.cbegin();
       cit != pushButtonTeethPositions.cend();
       ++cit)
  {
    this->buttonGroupTeethPosition.addButton(
        *cit, (*cit)->objectName().remove("pushButtonTeethPosition").toInt());
  }
  connect(&this->buttonGroupTeethPosition,
          static_cast<void (QButtonGroup::*)(int, bool)>(
              &QButtonGroup::buttonToggled),
          this,
          &SCBDentalPlanningPipeline::enterImplantLibrary);
  //   connect(SCBScene::getCurrentScene(),
  //           &SCBScene::signalAddedData,
  //           this,
  //           &SCBDentalPlanningPipeline::addImplant);
  //   connect(SCBScene::getCurrentScene(),
  //           &SCBScene::signalRemovedData,
  //           this,
  //           &SCBDentalPlanningPipeline::removeImplant);

  //   // membership variables
  //   m_nerveLeft = new SCBPolyData();
  //   m_nerveRight = new SCBPolyData();
  //   m_actorLeft = new SCBPolyDataActor();
  //   m_actorRight = new SCBPolyDataActor();
}

SCBDentalPlanningPipeline::~SCBDentalPlanningPipeline() {}

void SCBDentalPlanningPipeline::on_pushButtonAdvancedOption_clicked()
{
  //   SCBDentalReconstructionOptionDialog dialog(this);
  //   dialog.setWindowFlag(Qt::FramelessWindowHint);
  //   dialog.exec();
}

void SCBDentalPlanningPipeline::on_pushButtonAutoReconstruction_clicked()
{
  //   m_nerveLeft->setAbsolutePath(QStringList(
  //       QCoreApplication::applicationDirPath() +
  //       "/HardCode/nerve-left.stl"));
  //   m_nerveRight->setAbsolutePath(QStringList(
  //       QCoreApplication::applicationDirPath() +
  //       "/HardCode/nerve-right.stl"));
  //   m_nerveLeft->readData();
  //   m_nerveRight->readData();

  //   m_nerveLeft->setColor(1, 0, 0);
  //   m_nerveRight->setColor(1, 0, 0);

  //   m_actorLeft->setRenderDataSet(m_nerveLeft);
  //   SCBDentalHub::self->mainWindow->getViewer(3)->AddProp(m_actorLeft);

  //   m_actorRight->setRenderDataSet(m_nerveRight);
  //   SCBDentalHub::self->mainWindow->getViewer(3)->AddProp(m_actorRight);

  //   SCBDentalHub::self->mainWindow->getViewer(3)->Render();
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

bool SCBDentalPlanningPipeline::enterImplantLibrary(int id, bool checked)
{
  //   SCBScene* scene = SCBScene::getCurrentScene();
  //   if (!checked)
  //   {
  //     if (QMessageBox::No ==
  //         QMessageBox::question(this,
  //                               tr("Delete Implant"),
  //                               tr("Are you sure to delete this implant?")))
  //     {
  //       this->buttonGroupTeethPosition.blockSignals(true);
  //       this->buttonGroupTeethPosition.button(id)->setChecked(true);
  //       this->buttonGroupTeethPosition.blockSignals(false);
  //       return false;
  //     }
  //     SCBDentalFixture* fixture = scene->getDataByAlias<SCBDentalFixture>(
  //         "Fixture" + QString::number(id));
  //     if (!fixture)
  //     {
  //       qCritical() << "The Current scene does not have Fixture whose alias
  //       is"
  //                   << "Fixture" + QString::number(id);
  //       return false;
  //     }
  //     scene->removeData(fixture);
  //     SCBDentalCrown* crown =
  //         scene->getDataByAlias<SCBDentalCrown>("Crown" +
  //         QString::number(id));
  //     if (!crown)
  //     {
  //       qCritical() << "The Current scene does not have Fixture whose alias
  //       is"
  //                   << "Crown" + QString::number(id);
  //       return false;
  //     }
  //     scene->removeData(crown);

  //   }
  //   else
  //   {
  //     SCBDentalImplantLibrary implantLibraryDialog(this);

  //     if (implantLibraryDialog.exec() != QDialog::Accepted)
  //     {
  //       this->buttonGroupTeethPosition.blockSignals(true);
  //       this->buttonGroupTeethPosition.button(id)->setChecked(false);
  //       this->buttonGroupTeethPosition.blockSignals(false);
  //       return false;
  //     }
  //     const double* pos =
  //         SCBDentalHub::self->mainWindow->getViewer(0)->GetCursorPosition();

  //     SCBDentalImplant::ToothPosition toothPosition;
  //     toothPosition.fromFDI(id);
  //     SCBDentalFixture* fixture =
  //         scene->createDataByClassName<SCBDentalFixture>();
  //     fixture->setDentalFixtureBrand(implantLibraryDialog.brand);
  //     fixture->setDentalFixtureModel(implantLibraryDialog.model);
  //     fixture->setDentalFixtureShape(implantLibraryDialog.shape);
  //     fixture->setDentalFixtureLength(implantLibraryDialog.length);
  //     fixture->setDentalFixtureRadius(implantLibraryDialog.radius);
  //     fixture->setToothPosition(toothPosition);
  //     fixture->setRelativePath(QStringList() << implantLibraryDialog.path);
  //     fixture->readData("...");
  //     fixture->setAlias("Fixture" + QString::number(id));

  //     vtkSmartPointer<vtkTransform> translation =
  //         vtkSmartPointer<vtkTransform>::New();
  //     translation->Identity();
  //     translation->PostMultiply();
  //     translation->SetMatrix(fixture->getUserMatrix());
  //     translation->Translate(pos);
  //     fixture->getUserMatrix()->DeepCopy(translation->GetMatrix());

  //     SCBDentalCrown* crown = scene->createDataByClassName<SCBDentalCrown>();
  //     crown->setToothPosition(toothPosition);
  //     crown->setAlias("Crown" + QString::number(id));

  //     translation->Identity();
  //     translation->PostMultiply();
  //     translation->SetMatrix(crown->getUserMatrix());
  //     translation->Translate(pos);
  //     crown->getUserMatrix()->DeepCopy(translation->GetMatrix());

  //     scene->addData(crown);
  //     scene->addData(fixture);
  //   }
  return true;
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

void SCBDentalPlanningPipeline::on_pushButtonEnterImplantLibrary_clicked(
    bool clicked)
{
  int id = this->tableWidgetTeeth->currentRow();
  if (id < 0)
  {
    return;
  }
  if (this->enterImplantLibrary(id + 1, false))
  {
    this->enterImplantLibrary(id + 1, true);
  }

  // SCBDentalImplantLibrary implantLibraryDialog(this);
  // if (implantLibraryDialog.exec() != QDialog::Accepted) {
  //	return;
  // }
  // SCBDentalHub *hub = SCBDentalHub::self;
  // SCBScene *scene = SCBScene::getCurrentScene();
  // SCBDentalFixture *fixture = scene->addNewDataByClass<SCBDentalFixture>();
  // fixture->setAbsolutePath(QStringList() <<
  // QCoreApplication::applicationDirPath() +
  //	"/DentalFixture/Nobel_Conical_10_4.0.stl");
  // fixture->setRelativePath(QStringList() << fixture->getUniqueName() +
  // ".stl"); fixture->readData(); for (int i = 0; i <
  // SCBDentalHub::NUM_OF_ORTHOGONAL_VIEWER; ++i) { 	SCBPolyDataSourceWidget*
  // widget = hub->widgets[i]->ProducePolyDataSourceWidgets();
  //	widget->SetSCBPolyData(fixture);
  //	hub->widgets[i]->SetOneOfPolyDataSourceWidgetsEnabled(widget, true);
  //	widget->PlaceWidget(
  //		hub->mainWindow->getViewer(i)->GetCursorPosition()[0],
  //		hub->mainWindow->getViewer(i)->GetCursorPosition()[1],
  //		hub->mainWindow->getViewer(i)->GetCursorPosition()[2]);
  // }
}
