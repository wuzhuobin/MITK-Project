#ifndef ACETABULAR_PREP_WIDGET
#define ACETABULAR_PREP_WIDGET
// qt
#include <QStackedWidget>

namespace Ui{
class AcetabularPrepWidget;
}
class QActionGroup;

class AcetabularPrepWidget: public QStackedWidget
{
  Q_OBJECT;
public:
  explicit AcetabularPrepWidget(QWidget *parent = nullptr);
  virtual ~AcetabularPrepWidget() override;
  const QActionGroup *GetActions() const { return this->actions; }
private:
  Ui::AcetabularPrepWidget *ui;
  QActionGroup *actions;
  Q_DISABLE_COPY(AcetabularPrepWidget);
private Q_SLOTS:
  void on_action_Pelvis_Checkpoint_triggered(bool checked = false);
  void on_action_Pelvis_Landmark_triggered(bool checked = false);
  void on_action_Pelvis_Registration_triggered(bool checked = false);
  void on_action_RIO_Registratoin_triggered(bool checked = false);
  void on_AcetabularPrepWidget_currentChanged(int index);
};
#endif //! ACETABULAR_PREP_WIDGET