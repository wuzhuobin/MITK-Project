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
  enum VIEWS
  {
    VIEWER_DEFAULT = 0,
    CT,
    _3D_SLICER,
    X_RAY,
    REAMING,
    VIEWER_UNTILITY
  };

  enum MODES
  {
    PLAN_DEFAULT = 0,
    PRE_OP,
    CUP_PLAN,
    STEM_PLAN,
    REDUCED
  };

public:
  explicit THAStdMultiWidget(
    QWidget *parent = nullptr,
    Qt::WindowFlags f = nullptr,
    const QString &name = "stdmulti");
//   virtual ~THAStdMultiWidget() override;
  virtual void InitializeMultiWidget() override;
  int GetView() const { return this->mViewerMode; }
  void SetView(int mode);
  int GetMode() const { return this->mode; }
  void SetMode(int mode);
  bool IsRegistrationMode() const { return this->registrationMode; }
  void SetRegistrationMode(bool flag);
private:
  int mViewerMode;
  int mode;
  bool registrationMode;
  GroupBoxGadget *groupBoxGadget[3];
  StemParameterGadget *stemParameterGadget;
  ImplantAssessmentGadget *implantAssessmentGadget;
  CupParameterGadget *cupParameterGadget;
	//CupResultGadget *cupResultGadget;

};
#endif //!THA_STD_MULTI_WIDGET
