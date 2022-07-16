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
  explicit SCBDentalPlanningPipeline(QWidget* parent = 0);
  ~SCBDentalPlanningPipeline();

  QButtonGroup buttonGroupTeethPosition;

public Q_SLOTS:
  void on_pushButtonEnterImplantLibrary_clicked(bool clicked);
  bool enterImplantLibrary(int id, bool checked);
  void addImplant(QString uniqueName);
  void removeImplant(QString uniqueName);
  // void import();
  // void clear();
  void on_pushButtonAdvancedOption_clicked();
  void on_pushButtonAutoReconstruction_clicked();
  void on_tableWidgetTeeth_cellClicked(int row, int column);

Q_SIGNALS:
  void signalConfirmPlanning();

private:
  SCBPolyData* m_nerveLeft;
  SCBPolyData* m_nerveRight;
  SCBPolyDataActor* m_actorLeft;
  SCBPolyDataActor* m_actorRight;
};

#endif  // SCBDentalPlanningPipeline_H
