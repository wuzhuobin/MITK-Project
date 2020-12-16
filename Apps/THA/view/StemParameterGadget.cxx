#include "StemParameterGadget.h"
#include "ui_StemParameterGadget.h"
// mitk
#include <mitkRenderingManager.h>
#include <mitkDataStorage.h>
#include <mitkPointSet.h>
#include <mitkVectorProperty.h>
// itk
#include <itkCommand.h>

class StemParameterGadgetOrientationCommand : public itk::Command
{
public:

	typedef StemParameterGadgetOrientationCommand Self;
	typedef itk::Command Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro(Self);
	itkTypeMacro(Self, Superclass);

	StemParameterGadgetOrientationCommand() {}
	virtual ~StemParameterGadgetOrientationCommand() override {}

	virtual void Execute(itk::Object *caller, const itk::EventObject & event) override
	{
		this->Execute(const_cast<const itk::Object*>(caller), event);
	}

	virtual void Execute(const itk::Object *caller, const itk::EventObject & event) override
	{
		itkNotUsed(event);
		const mitk::Vector3DProperty *orientation =
			reinterpret_cast<const mitk::Vector3DProperty*>(caller);
		// @TODO current calculation seems to correct
		if (orientation->GetValue()[0] > 180)
		{
			this_->ui->spinBoxFemoralVersion->setValue(360 - orientation->GetValue()[0]);
		}
		else
		{
			this_->ui->spinBoxFemoralVersion->setValue(orientation->GetValue()[0]);
		}
	}
	const StemParameterGadget *this_ = nullptr;
private:
	ITK_DISALLOW_COPY_AND_ASSIGN(StemParameterGadgetOrientationCommand);
};
StemParameterGadget::StemParameterGadget(const QString &femoralStem,
	const QString &femoralNeck,
	const QString &femoralHead,
	QWidget *parent) :
	QWidget(parent),
  ui(new Ui::StemParameterGadget),
	femoralStem(femoralStem),
	femoralNeck(femoralNeck),
	femoralHead(femoralHead)
{
    this->ui->setupUi(this);
}

StemParameterGadget::~StemParameterGadget()
{
    delete this->ui;
}

void StemParameterGadget::observerStem() const
{
	mitk::DataStorage *ds =
		mitk::RenderingManager::GetInstance()->GetDataStorage();
	mitk::DataNode * surfaceNode[3] = {
		ds->GetNamedNode(this->femoralStem.toStdString()),
		ds->GetNamedNode(this->femoralNeck.toStdString()),
		ds->GetNamedNode(this->femoralHead.toStdString())
	};
	StemParameterGadgetOrientationCommand::Pointer oCommand =
		StemParameterGadgetOrientationCommand::New();
	oCommand->this_ = this;
	surfaceNode[0]->GetProperty("orientation")->AddObserver(itk::ModifiedEvent(), oCommand);
	surfaceNode[0]->GetProperty("orientation")->Modified();
}

void StemParameterGadget::SetFemoralVersion(int value)
{
    //disconnect(this->ui->spinBoxFemoralVersion, SIGNAL(valuedChanged(int)), this, SLOT(on_spinBoxFemoralVersion_editingFinished(int)));
    this->ui->spinBoxFemoralVersion->setValue(value);
    //connect(this->ui->spinBoxFemoralVersion, SIGNAL(valuedChanged(int)), this, SLOT(on_spinBoxFemoralVersion_editingFinished(int)),Qt::UniqueConnection);
}

int StemParameterGadget::GetFemoralVersion()
{
    return this->ui->spinBoxFemoralVersion->value();
}

void StemParameterGadget::on_spinBoxFemoralVersion_editingFinished()
{

}
