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

private:
    Ui::SegmentationWidget *ui;
    mitk::BoundingShapeInteractor::Pointer boundingShapeInteractor;

private Q_SLOTS:
    void on_pushButtonMarkerSegmentation_clicked(bool checked = false);
    void on_pushButtonLabeling_clicked(bool checked = false);
};

#endif //! SEGMENTATION_WIDGET_H