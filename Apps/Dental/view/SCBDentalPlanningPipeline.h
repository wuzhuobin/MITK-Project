#ifndef SCBDentalPlanningPipeline_H
#define SCBDentalPlanningPipeline_H

#include <QButtonGroup>
#include <QWidget>

#include "ui_SCBDentalPlanningPipeline.h"

// #include "SCBDentalFixture.h"
// #include "SCBDentalCrown.h"
class SCBPolyData;
class SCBPolyDataActor;

class SCBDentalPlanningPipeline : public QWidget,
                                  public Ui::SCBDentalPlanningPipeline
{
  Q_OBJECT

public:
  explicit SCBDentalPlanningPipeline(QWidget* parent = nullptr);

  Q_SIGNAL void signalConfirmPlanning();

private:
  QButtonGroup buttonGroupTeethPosition;

  Q_SLOT void on_pushButtonEnterImplantLibrary_clicked(bool clicked);
  Q_SLOT void on_pushButtonAdvancedOption_clicked();
  Q_SLOT void on_pushButtonAutoReconstruction_clicked();
  Q_SLOT void on_tableWidgetTeeth_cellClicked(int row, int column);
  Q_SLOT bool enterImplantLibrary(int id, bool checked);
  Q_SLOT void addImplant(QString uniqueName);
  Q_SLOT void removeImplant(QString uniqueName);
};

#endif  // SCBDentalPlanningPipeline_H
