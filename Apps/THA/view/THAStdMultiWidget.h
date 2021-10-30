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
  virtual void InitializeMultiWidget() override;
  int getView() const { return mView; }
  void setView(int mode);
  int getMode() const { return mMode; }
  void setMode(int mode);
  void setOther(bool other);
  bool getOther() const { return mOther; }
  void setMeanIntensityProjection(bool flag);
private:
  int mView = VIEW_DEFAULT;
  int mMode = MODE_DEFAULT;
  bool mOther = false;
  bool mMeanIntensityProjection = false;
  bool mInitialized = false;
  GroupBoxGadget *mGroupBoxGadget[4];
  StemParameterGadget *mStemParameterGadget[4];
  ImplantAssessmentGadget *mImplantAssessmentGadget[4];
  CupParameterGadget *mCupParameterGadget[4];
  void UpdateViewMode();
};
#endif //!THA_STD_MULTI_WIDGET
