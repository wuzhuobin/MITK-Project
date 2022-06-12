#ifndef GROUP_BOX_GADGET_H
#define GROUP_BOX_GADGET_H
// qt
#include <QGroupBox>

namespace Ui
{
class GroupBoxGadget;
}

class GroupBoxGadget : public QGroupBox
{
  Q_OBJECT;

public:
  enum class Orientation
  {
    AXIAL,
    SAGITTAL,
    CORONAL
  };

public:
  explicit GroupBoxGadget(Orientation orientation, QWidget* parent = nullptr);
  virtual ~GroupBoxGadget() override;

  void setTarget(QString target) { mTarget = target; }
  QString getTarget() const { return mTarget; }
  void setTargetCor(QString targetCor) { mTargetCor = targetCor; }
  QString getTargetCor() const { return mTargetCor; }

  double getTranslationSpeed() const { return mTranslationSpeed; }
  void setTranslationSpeed(double speed) { mTranslationSpeed = speed; }
  double getRotationSpeed() const { return mRotationSpeed; }
  void setRotationSpeed(double speed) { mRotationSpeed = speed; }

private:
  Ui::GroupBoxGadget* mUi;
  Orientation mCurrentOrientation;
  QString mTarget;
  QString mTargetCor;
  double mTranslationSpeed = 2;
  double mRotationSpeed = 5;

  void internalTransform(double position[3], double orientation[3]);
  Q_SLOT void on_pushButtonAntiClockwise_clicked(bool checked = false);
  Q_SLOT void on_pushButtonClockwise_clicked(bool checked = false);
  Q_SLOT void on_pushButtonDown_clicked(bool checked = false);
  Q_SLOT void on_pushButtonLeft_clicked(bool checked = false);
  Q_SLOT void on_pushButtonRight_clicked(bool checked = false);
  Q_SLOT void on_pushButtonUp_clicked(bool checked = false);
};

#endif  //! GROUP_BOX_GADGET_H