#ifndef SEGMENTATION_WIDGET_H
#define SEGMENTATION_WIDGET_H

// qt
#include <QWidget>

// mitk
#include <mitkBoundingShapeInteractor.h>

namespace Ui {
class SegmentationWidget;
}
class SegmentationWidget: public QWidget
{
public:
  explicit SegmentationWidget(QWidget *parent = nullptr);
  ~SegmentationWidget() override;

protected:
  void changeEvent(QEvent *event) override;

private:
    Ui::SegmentationWidget *ui;
    mitk::BoundingShapeInteractor::Pointer boundingShapeInteractor;
    class QButtonGroup *buttonGroup;

    Q_SLOT void on_pushButtonMarkerSegmentation_clicked(bool checked = false);
    Q_SLOT void on_pushButtonLabeling_clicked(bool checked = false);
    Q_SLOT void on_stackedWidget_currentChanged(int index);
    Q_SLOT void onButtonToggled(class QAbstractButton *button, bool checked);
};

#endif //! SEGMENTATION_WIDGET_H