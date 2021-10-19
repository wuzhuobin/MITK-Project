#ifndef ACETABULAR_PREP_WIDGET
#define ACETABULAR_PREP_WIDGET

#include "mitkReamingFilter.h"

// mitk
#include <mitkSurface.h>

// qt
#include <QStackedWidget>

namespace Ui{
class AcetabularPrepWidget;
}
class AcetabularPrepView;

class AcetabularPrepWidget : public QStackedWidget
{
  Q_OBJECT;
public:
  explicit AcetabularPrepWidget(AcetabularPrepView *view, QWidget *parent = nullptr);
  virtual ~AcetabularPrepWidget() override;

public Q_SLOTS:
  void Action_Pelvis_Checkpoint_triggered(bool checked = false);
  void Action_Pelvis_Landmark_triggered(bool checked = false);
  void Action_Pelvis_Registration_triggered(bool checked = false);
  void Action_RIO_Registratoin_triggered(bool checked = false);
  void Action_Cup_Reaming_triggered(bool checked = false);
  void Action_Cup_Impaction_triggered(bool checked = false);

private:
  Ui::AcetabularPrepWidget *mUi;
  AcetabularPrepView *mView;
  int pelvisLandmarkIndex = 0;
  int pelvisRegistrationIndex = 0;

  mitk::ReamingFilter::Pointer reamingFilter;
  mitk::Surface::Pointer realReamer;

  Q_DISABLE_COPY(AcetabularPrepWidget);
  void SetPelvisLandmarkIndex(int index);
  void SetPelvisRegistrationIndex(int index);

  enum ORIENTATION {
    X = 0,
    Y,
    Z
  };

  void transformReamer(unsigned int orientation, double t, double r);

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
#endif //! ACETABULAR_PREP_WIDGET