#ifndef GROUP_BOX_GADGET_H
#define GROUP_BOX_GADGET_H
// qt
#include <QGroupBox>

namespace Ui { class GroupBoxGadget; }

class GroupBoxGadget: public QGroupBox
{
  Q_OBJECT;
public:
  enum ORIENTATION
  {
    AXIAL,
    SAGITTAL,
    CORONAL
  };

public:
  explicit GroupBoxGadget(int orientation, 
    QString target,
    QString targetCor,
		QWidget *parent = nullptr);
  virtual ~GroupBoxGadget() override;

  double getTranslationSpeed() const { return this->translationSpeed; }
  void setTranslationSpeed(double speed) { this->translationSpeed = speed; }
  double getRotationSpeed() const { return this->rotationSpeed; }
  void setRotationSpeed(double speed) { this->rotationSpeed = speed; }

private:
  Ui::GroupBoxGadget *ui;
  int currentOrientation;
  double translationSpeed;
  double rotationSpeed;
  QString target;
  QString targetCor;

  void internalTransform(double position[3], double orientation[3]);
  Q_SLOT void on_pushButtonAntiClockwise_clicked(bool checked = false);
  Q_SLOT void on_pushButtonClockwise_clicked(bool checked = false);
  Q_SLOT void on_pushButtonDown_clicked(bool checked = false);
  Q_SLOT void on_pushButtonLeft_clicked(bool checked = false);
  Q_SLOT void on_pushButtonRight_clicked(bool checked = false);
  Q_SLOT void on_pushButtonUp_clicked(bool checked = false);
};

#endif //!GROUP_BOX_GADGET_H