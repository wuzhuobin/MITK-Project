#ifndef STEM_PARAMETER_GADGET_H
#define STEM_PARAMETER_GADGET_H

#include <QWidget>

namespace Ui
{
  class StemParameterGadget;
}
class StemParameterGadgetOrientationCommand;

class StemParameterGadget : public QWidget
{
  Q_OBJECT

public:
  //explicit StemParameterGadget(QWidget *parent = 0);
  //~StemParameterGadget();
	explicit StemParameterGadget(
		const QString &femoralStem = QStringLiteral("femoral_stem_transformed"),
		const QString &femoralNeck = QStringLiteral("femoral_neck_transformed"),
		const QString &femoralHead = QStringLiteral("femoral_head_transformed"),
		QWidget *parent = 0);
	explicit StemParameterGadget(QWidget *parent) :
		StemParameterGadget(
			QStringLiteral("femoral_stem_transformed"),
			QStringLiteral("femoral_neck_transformed"),
			QStringLiteral("femoral_head_transformed"),
			parent
		) {}
	virtual ~StemParameterGadget() override;
	void observerStem() const;

  void SetFemoralVersion(int);

  int GetFemoralVersion();
private:
	QString femoralStem;
	QString femoralNeck;
	QString femoralHead;

	friend class StemParameterGadgetOrientationCommand;

private Q_SLOTS:
  void on_spinBoxFemoralVersion_editingFinished();

private:
  Ui::StemParameterGadget *ui;
};

#endif //!STEM_PARAMTER_GADGET_H
