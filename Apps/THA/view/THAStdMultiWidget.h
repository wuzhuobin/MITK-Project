#ifndef THA_STD_MULTI_WIDGET
#define THA_STD_MULTI_WIDGET
// mitk
#include <QmitkStdMultiWidget.h>
//
class GroupBoxGadget;
class StemParameterGadget;
class ImplantAssessmentGadget;
class CupParameterGadget;
class CupImpactionParameterGadget;

class THAStdMultiWidget : public QmitkStdMultiWidget
{
  Q_OBJECT;

public:
  enum class View
  {
    VIEW_DEFAULT = 0,
    VIEW_CT,
    VIEW_3D_SLICER,
    VIEW_X_RAY,
    VIEW_REAMING,
    VIEW_UNTILITY
  };

  enum class Mode
  {
    MODE_DEFAULT = 0,
    MODE_PRE_OP,
    MODE_CUP_PLAN,
    MODE_STEM_PLAN,
    MODE_REDUCED
  };

  enum class Custom
  {
    Default,
    Other,
    MaskCut,
    CupImpaction
  };

  static THAStdMultiWidget* getInstance();
  explicit THAStdMultiWidget(QWidget* parent = nullptr,
                             Qt::WindowFlags f = nullptr,
                             const QString& name = "stdmulti");
  virtual void InitializeMultiWidget() override;
  View getView() const { return mView; }
  void setView(View view);
  Mode getMode() const { return mMode; }
  void setMode(Mode mode);
  Custom getCustom() const { return mCustom; }
  void setCustom(Custom custom);

private:
  View mView = View::VIEW_DEFAULT;
  Mode mMode = Mode::MODE_DEFAULT;
  Custom mCustom;
  bool mInitialized = false;
  GroupBoxGadget* mGroupBoxGadget[4];
  StemParameterGadget* mStemParameterGadget[4];
  ImplantAssessmentGadget* mImplantAssessmentGadget[4];
  CupParameterGadget* mCupParameterGadget[4];
  CupImpactionParameterGadget* mCupImpactionParameterGadget[4];
  void updateViewMode();
};
#endif  //! THA_STD_MULTI_WIDGET
