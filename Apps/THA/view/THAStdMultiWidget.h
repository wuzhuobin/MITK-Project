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
    VIEW_DEFAULT = 0,
    VIEW_CT,
    VIEW_3D_SLICER,
    VIEW_X_RAY,
    VIEW_REAMING,
    VIEW_UNTILITY
  };

  enum MODES
  {
    MODE_DEFAULT = 0,
    MODE_PRE_OP,
    MODE_CUP_PLAN,
    MODE_STEM_PLAN,
    MODE_REDUCED
  };

public:
  explicit THAStdMultiWidget(
    QWidget *parent = nullptr,
    Qt::WindowFlags f = nullptr,
    const QString &name = "stdmulti");
//   virtual ~THAStdMultiWidget() override;
  virtual void InitializeMultiWidget() override;
  int GetView() const { return this->view; }
  void SetView(int mode);
  int GetMode() const { return this->mode; }
  void SetMode(int mode);
  bool IsRegistrationMode() const { return this->registrationMode; }
  void SetRegistrationMode(bool flag);
private:
  int view;
  int mode;
  bool registrationMode;
  GroupBoxGadget *groupBoxGadget[3];
  StemParameterGadget *stemParameterGadget;
  ImplantAssessmentGadget *implantAssessmentGadget;
  CupParameterGadget *cupParameterGadget;
	//CupResultGadget *cupResultGadget;

};
#endif //!THA_STD_MULTI_WIDGET
