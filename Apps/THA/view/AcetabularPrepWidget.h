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
private:
  Ui::AcetabularPrepWidget *ui;
};
#endif //! ACETABULAR_PREP_WIDGET