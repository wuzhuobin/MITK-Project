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
  int GetViewerMode() const { return this->mViewerMode; }
  void SetViewerMode(int mode);
  int GetPlanMode() const { return this->mpPlanMode; }
  void SetPlanMode(int mode);
  bool IsRegistrationMode() const { return this->registrationMode; }
  void SetRegistrationMode(bool flag);
private:
  int mViewerMode;
  int mpPlanMode;
  bool registrationMode;
  GroupBoxGadget *mpGroupBoxGadget[3];
  StemParameterGadget *mpStemParameterGadget;
  ImplantAssessmentGadget *mpImplantAssessmentGadget;
  CupParameterGadget *mpCupParameterGadget;
	//CupResultGadget *cupResultGadget;

};
#endif //!THA_STD_MULTI_WIDGET
