#ifndef ACETABULAR_PREP_NEXT_BACK_WIDGET_H
#define ACETABULAR_PREP_NEXT_BACK_WIDGET_H

// qt
#include <QWidget>

// std
#include <memory>

namespace Ui { class AcetabularPrepNextBackWidget; }

class AcetabularPrepNextBackWidget : public QWidget
{
  Q_OBJECT
public:
  explicit AcetabularPrepNextBackWidget(QWidget* parent = nullptr);
  ~AcetabularPrepNextBackWidget() override;
  
  Q_SIGNAL void backClicked();
  Q_SIGNAL void nextClicked();
private:
  std::unique_ptr<Ui::AcetabularPrepNextBackWidget> mUi;
  Q_SLOT void on_pushButtonBack_clicked(bool checked = false);
  Q_SLOT void on_pushButtonNext_clicked(bool checked = false);
};

#endif //!ACETABULAR_PREP_NEXT_BACK_WIDGET_H