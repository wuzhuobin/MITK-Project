#ifndef ACETABULAR_PREP_WIDGET
#define ACETABULAR_PREP_WIDGET

#include "mitkReamingFilter.h"

// qt
#include <QStackedWidget>

// vtk
#include <vtkSmartPointer.h>

class ReamingFilter2;
namespace Ui
{
class AcetabularPrepWidget;
}

class AcetabularPrepWidget : public QStackedWidget
{
  Q_OBJECT;

public:
  explicit AcetabularPrepWidget(QWidget* parent = nullptr);
  ~AcetabularPrepWidget() override;

public Q_SLOTS:
  void action_Pelvis_Checkpoint_triggered(bool checked = false);
  void action_Pelvis_Landmark_triggered(bool checked = false);
  void action_Pelvis_Registration_triggered(bool checked = false);
  void action_RIO_Registratoin_triggered(bool checked = false);
  void action_Cup_Reaming_triggered(bool checked = false);
  void action_Cup_Impaction_triggered(bool checked = false);

private:
  Ui::AcetabularPrepWidget* mUi;
  int pelvisLandmarkIndex = 0;
  int pelvisRegistrationIndex = 0;

  vtkSmartPointer<ReamingFilter2> mReamingFilter;

  Q_DISABLE_COPY(AcetabularPrepWidget);
  static vtkSmartPointer<vtkPolyData> GetTransformedVtkPolyData(
      mitk::Surface* surface);
  void setPelvisLandmarkIndex(int index);
  void setPelvisRegistrationIndex(int index);

  enum class Orientation
  {
    X = 0,
    Y,
    Z
  };

  void transformReamer(Orientation orientation, double t, double r);
  void transformImpactingCup(Orientation orientation, double t, double r);

  Q_SLOT void on_AcetabularPrepWidget_currentChanged(int index);

  Q_SLOT void on_pushButtonCaptureLandmark_clicked(bool checked = false);

  Q_SLOT void on_pushButtonClearLandmark_clicked(bool checked = false);

  Q_SLOT void on_pushButtonClearAllPelvis_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCaptureRegistration_clicked(bool checked = false);

  Q_SLOT void on_pushButtonClearLastPoint_clicked(bool checked = false);

  Q_SLOT void on_pushButtonClearAllPoints_clicked(bool checked = false);

  Q_SLOT void on_pushButtonVerifyRegistration_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformXRA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformXRM_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformXTA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformXTM_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformYRA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformYRM_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformYTA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformYTM_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformZRA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformZRM_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformZTA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonReamerTransformZTM_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionXRA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionXRM_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionXTA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionXTM_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionYRA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionYRM_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionYTA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionYTM_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionZRA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionZRM_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionZTA_clicked(bool checked = false);

  Q_SLOT void on_pushButtonCupImpactionZTM_clicked(bool checked = false);
};
#endif  //! ACETABULAR_PREP_WIDGET