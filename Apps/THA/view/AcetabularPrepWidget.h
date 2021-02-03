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
  Q_DISABLE_COPY(AcetabularPrepWidget)
};
#endif //! ACETABULAR_PREP_WIDGET