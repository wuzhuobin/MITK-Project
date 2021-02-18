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
  Q_DISABLE_COPY(AcetabularPrepWidget);

private Q_SLOTS:

  void on_AcetabularPrepWidget_currentChanged(int index);
};
#endif //! ACETABULAR_PREP_WIDGET