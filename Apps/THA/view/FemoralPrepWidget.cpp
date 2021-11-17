#include "FemoralPrepWidget.h"

#include "ui_FemoralPrepWidget.h"

FemoralPrepWidget::FemoralPrepWidget(QWidget* parent) :
    QStackedWidget(parent), mUi(std::make_unique<Ui::FemoralPrepWidget>())
{
    mUi->setupUi(this);
}