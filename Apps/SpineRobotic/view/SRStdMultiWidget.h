#ifndef SR_STD_MULTI_WIDGET_H
#define SR_STD_MULTI_WIDGET_H

// mitk
#include <QmitkStdMultiWidget.h>
// #include <mitkPlanarFigureInteractor.h>

class SRStdMultiWidget : public QmitkStdMultiWidget {
  Q_OBJECT;

public:
  explicit SRStdMultiWidget(QWidget *parent = nullptr,
                            Qt::WindowFlags f = nullptr,
                            const QString &name = "stdmulti");
  void InitializeMultiWidget() override;

  void enableGroupBox(bool flag);

  void enablePlanarLine(bool flag);

  void enablePlanarAngle(bool flag);

private:
  // mitk::PlanarFigureInteractor::Pointer planarFigureInteractor;
  class GroupBoxGadget *groupBoxGadget[4];
};

#endif //! SR_STD_MULTI_WIDGET_H