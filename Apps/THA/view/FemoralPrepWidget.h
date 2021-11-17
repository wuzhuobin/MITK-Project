#ifndef FEMORAL_PREP_WIDGET_H
#define FEMORAL_PREP_WIDGET_H

// std
#include <memory>

// qt
#include <QStackedWidget>

namespace Ui
{
class FemoralPrepWidget;
}

class FemoralPrepWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit FemoralPrepWidget(QWidget* parent = nullptr);

private:
    std::unique_ptr<Ui::FemoralPrepWidget> mUi;
};

#endif  //! FEMORAL_PREP_WIDGET_H