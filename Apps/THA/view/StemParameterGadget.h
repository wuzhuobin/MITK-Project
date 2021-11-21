#ifndef STEM_PARAMETER_GADGET_H
#define STEM_PARAMETER_GADGET_H

#include <QWidget>

namespace Ui
{
  class StemParameterGadget;
}
class StemParameterGadgetCommand;

class StemParameterGadget : public QWidget
{
  Q_OBJECT

public:
	explicit StemParameterGadget(
		const QString &femoralStem = QStringLiteral("femoral_stem"),
		const QString &femoralHead = QStringLiteral("femoral_head"),
		QWidget *parent = 0);
	explicit StemParameterGadget(QWidget *parent) :
		StemParameterGadget(
			QStringLiteral("femoral_stem"),
			QStringLiteral("femoral_head"),
			parent
		) {}
	virtual ~StemParameterGadget() override;
	void ObserverStem() const;

private:
	QString femoralStem;
	QString femoralHead;

	friend class StemParameterGadgetCommand;

private:
  Ui::StemParameterGadget *mUi;
};

#endif //!STEM_PARAMTER_GADGET_H
