#ifndef ACETABULAR_PREP_WIDGET
#define ACETABULAR_PREP_WIDGET
// qt
#include <QStackedWidget>

namespace Ui{
class AcetabularPrepWidget;
}

class AcetabularPrepWidget: public QStackedWidget
{
  Q_OBJECT;
public:
  explicit AcetabularPrepWidget(QWidget *parent = nullptr);
  virtual ~AcetabularPrepWidget() override;

public Q_SLOTS:
  void Action_Pelvis_Checkpoint_triggered(bool checked = false);
  void Action_Pelvis_Landmark_triggered(bool checked = false);
  void Action_Pelvis_Registration_triggered(bool checked = false);
  void Action_RIO_Registratoin_triggered(bool checked = false);

private:
  Ui::AcetabularPrepWidget *ui;
  int pelvisLandmarkIndex = 0;
  int pelvisRegistrationIndex = 0;
  Q_DISABLE_COPY(AcetabularPrepWidget);
  void SetPelvisLandmarkIndex(int index);
  void SetPelvisRegistrationIndex(int index);

private Q_SLOTS:

  void on_AcetabularPrepWidget_currentChanged(int index);

  void on_pushButtonCaptureLandmark_clicked(bool checked = false);

  void on_pushButtonClearLandmark_clicked(bool checked = false);

  void on_pushButtonClearAllPelvis_clicked(bool checked = false);

  void on_pushButtonCaptureRegistration_clicked(bool checked = false);

  void on_pushButtonClearLastPoint_clicked(bool checked = false);

  void on_pushButtonClearAllPoints_clicked(bool checked = false);

  void on_pushButtonVerifyRegistration_clicked(bool checked = false);
};
#endif //! ACETABULAR_PREP_WIDGET