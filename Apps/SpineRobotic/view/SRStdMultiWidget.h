#ifndef SR_STD_MULTI_WIDGET_H
#define SR_STD_MULTI_WIDGET_H

// mitk
#include <QmitkStdMultiWidget.h>

class SRStdMultiWidget: public QmitkStdMultiWidget
{
    Q_OBJECT;
public:
    explicit SRStdMultiWidget(
        QWidget *parent = nullptr,
        Qt::WindowFlags f = nullptr,
        const QString &name = "stdmulti");
    void InitializeMultiWidget() override;
};

#endif //! SR_STD_MULTI_WIDGET_H