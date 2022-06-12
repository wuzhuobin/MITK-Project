#ifndef GROUP_BOX_GADGET_H
#define GROUP_BOX_GADGET_H
// qt
#include <QGroupBox>

namespace Ui {
class GroupBoxGadget;
}

class GroupBoxGadget : public QGroupBox {
  Q_OBJECT;

public:
  enum class Orientation { AXIAL, SAGITTAL, CORONAL };

public:
  explicit GroupBoxGadget(Orientation orientation, QWidget *parent = nullptr);
  virtual ~GroupBoxGadget() override;

  void setTarget(QString target) { this->target = target; }
  QString getTarget() const { return this->target; }
  void setTargetCor(QString targetCor) { this->targetCor = targetCor; }
  QString getTargetCor() const { return this->targetCor; }

  double getTranslationSpeed() const { return this->translationSpeed; }
  void setTranslationSpeed(double speed) { this->translationSpeed = speed; }
  double getRotationSpeed() const { return this->rotationSpeed; }
  void setRotationSpeed(double speed) { this->rotationSpeed = speed; }

private:
  Ui::GroupBoxGadget *mUi;
  Orientation currentOrientation;
  QString target;
  QString targetCor;
  double translationSpeed = 2;
  double rotationSpeed = 5;

  void internalTransform(double position[3], double orientation[3]);
  Q_SLOT void on_pushButtonAntiClockwise_clicked(bool checked = false);
  Q_SLOT void on_pushButtonClockwise_clicked(bool checked = false);
  Q_SLOT void on_pushButtonDown_clicked(bool checked = false);
  Q_SLOT void on_pushButtonLeft_clicked(bool checked = false);
  Q_SLOT void on_pushButtonRight_clicked(bool checked = false);
  Q_SLOT void on_pushButtonUp_clicked(bool checked = false);
};

#endif //! GROUP_BOX_GADGET_H