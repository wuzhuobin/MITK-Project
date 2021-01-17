#ifndef GROUP_BOX_GADGET_H
#define GROUP_BOX_GADGET_H
// qt
#include <QGroupBox>
#include <mitkDataStorage.h>
#include <mitkSurface.h>

namespace Ui { class GroupBoxGadget; }

class GroupBoxGadget: public QGroupBox
{
  Q_OBJECT;
public:
  enum MODE
  {
    DEFAULT = 0,
    CUP,
    STEM
  };
  enum ORIENTATION
  {
    AXIAL,
    SAGITTAL,
    CORONAL
  };

public:
  explicit GroupBoxGadget(int orientation, 
		const QString &femoralHeadCor = QStringLiteral("femoral_head_cor"),
		const QString &femoralStem = QStringLiteral("femoral_stem"),
		const QString &femoralHead = QStringLiteral("femoral_head"),
		const QString &cupCor = QStringLiteral("cup_cor"),
		const QString &acetabularShell = QStringLiteral("acetabular_shell"),
		const QString &acetabularLiner = QStringLiteral("acetabular_liner"),
		QWidget *parent = nullptr);
	explicit GroupBoxGadget(int orientation,
		QWidget *parent) :
		GroupBoxGadget(orientation,
      QStringLiteral("femoral_head_cor"),
			QStringLiteral("femoral_stem"),
			QStringLiteral("femoral_head"),
		  QStringLiteral("cup_cor"),
			QStringLiteral("acetabular_shell"),
			QStringLiteral("acetabular_liner"),
			parent
		) {}
  virtual ~GroupBoxGadget() override;

  void setMode(int mode) { this->currentMode = mode; }
  double getTranslationSpeed() const { return this->translationSpeed; }
  void setTranslationSpeed(double speed) { this->translationSpeed = speed; }
  double getRotationSpeed() const { return this->rotationSpeed; }
  void setRotationSpeed(double speed) { this->rotationSpeed = speed; }

private:
  int currentMode;
  int currentOrientation;
  double translationSpeed;
  double rotationSpeed;
  QString femoralHeadCor;
  QString femoralStem;
  QString femoralHead;
  QString acetabularShell;
  QString acetabularLiner;
  QString cupCor;

private:
  void internalTransform(double position[3], double orientation[3]);
	// void CupTransform(mitk::Surface *surface[5], mitk::DataNode *surfaceNode[5], double position[3], double orientation[3]);
	// void StemTransform(mitk::Surface *surface_stem[3], mitk::DataNode *surfaceNode_stem[3], double position[3], double orientation[3]);

private Q_SLOTS:
  void on_pushButtonAntiClockwise_clicked(bool checked = false);
  void on_pushButtonClockwise_clicked(bool checked = false);
  void on_pushButtonDown_clicked(bool checked = false);
  void on_pushButtonLeft_clicked(bool checked = false);
  void on_pushButtonRight_clicked(bool checked = false);
  void on_pushButtonUp_clicked(bool checked = false);
private:
  Ui::GroupBoxGadget *ui;
};

#endif //!GROUP_BOX_GADGET_H