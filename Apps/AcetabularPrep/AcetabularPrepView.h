#ifndef ACETABULAR_PREP_VIEW_H
#define ACETABULAR_PREP_VIEW_H

// std
#include <memory>

// qt
#include <QWidget>
namespace Ui {
  class AcetabularPrepView;
}
class AcetabularPrepView : public QWidget
{
  Q_OBJECT
public:
  explicit AcetabularPrepView(QWidget *parent = nullptr);
  ~AcetabularPrepView() override;

private:
  std::unique_ptr<Ui::AcetabularPrepView> mUi;
};

#endif // !ACETABULAR_PREP_VIEW_H
