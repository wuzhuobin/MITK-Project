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

private Q_SLOTS:

  void on_AcetabularPrepWidget_currentChanged(int index);

  void on_pushButtonCaptureLandmark_clicked(bool checked = false);

  void on_pushButtonClearLandmark_clicked(bool checked = false);

  void on_pushButtonClearAllPelvis_clicked(bool checked = false);

  void on_pushButtonCaptureRegistration_clicked(bool checked = false);

  void on_pushButtonClearLastPoint_clicked(bool checked = false);

  void on_pushButtonClearAllPoints_clicked(bool checked = false);

  void on_pushButtonVerifyRegistration_clicked(bool checked = false);

  void on_pushButtonXRA_clicked(bool checked = false);

  void on_pushButtonXRM_clicked(bool checked = false);

  void on_pushButtonXTA_clicked(bool checked = false);

  void on_pushButtonXTM_clicked(bool checked = false);

  void on_pushButtonYRA_clicked(bool checked = false);

  void on_pushButtonYRM_clicked(bool checked = false);

  void on_pushButtonYTA_clicked(bool checked = false);

  void on_pushButtonYTM_clicked(bool checked = false);

  void on_pushButtonZRA_clicked(bool checked = false);

  void on_pushButtonZRM_clicked(bool checked = false);

  void on_pushButtonZTA_clicked(bool checked = false);

  void on_pushButtonZTM_clicked(bool checked = false);
};
#endif  //! ACETABULAR_PREP_WIDGET