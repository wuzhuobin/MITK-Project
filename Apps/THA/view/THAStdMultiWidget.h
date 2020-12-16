#ifndef THA_STD_MULTI_WIDGET
#define THA_STD_MULTI_WIDGET
// mitk
#include <QmitkStdMultiWidget.h>
//
class GroupBoxGadget;
class StemParameterGadget;
class ImplantAssessmentGadget;
class CupParameterGadget;

class THAStdMultiWidget: public QmitkStdMultiWidget
{
  Q_OBJECT;

public:
  enum VIEWER_MODE
  {
    VIEWER_DEFAULT = 0,
    VIEWER_2D,
    VIEWER_3D,
    VIEWER_XRAY,
    VIEWER_REAMING,
    VIEWER_UNTILITY
  };

  enum PLAN_MODE
  {
    PLAN_DEFAULT = 0,
    PLAN_PREOP,
    PLAN_CUP,
    PLAN_STEM,
    PLAN_REDUCED
  };

public:
  explicit THAStdMultiWidget(
    QWidget *parent = nullptr,
    Qt::WindowFlags f = nullptr,
    const QString &name = "stdmulti");
  virtual void InitializeMultiWidget() override;
//   virtual ~THAStdMultiWidget() override;
  int getViewerMode() const { return this->viewerMode; }
  void setViewerMode(int mode);
  int getPlanMode() const { return this->planMode; }
  void setPlanMode(int mode);
  bool isRegistrationMode() const { return this->registrationMode; }
  void setRegistrationMode(bool flag);
  void visualize(mitk::DataStorage *ds, QString side);
	//void setActualResult(bool);
private:
  int viewerMode;
  int planMode;
  bool registrationMode;
  GroupBoxGadget *groupBoxGadget[3];
  StemParameterGadget *stemParameterGadget;
  ImplantAssessmentGadget *implantAssessmentGadget;
  CupParameterGadget *cupParameterGadget;
	//CupResultGadget *cupResultGadget;

};
#endif //!THA_STD_MULTI_WIDGET
