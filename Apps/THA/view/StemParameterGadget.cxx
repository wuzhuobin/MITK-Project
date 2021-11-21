#include "StemParameterGadget.h"
#include "ui_StemParameterGadget.h"

// mitk
#include <mitkRenderingManager.h>
#include <mitkSurface.h>
#include <mitkPointSet.h>
#include <mitkDataStorage.h>

// itk
#include <itkCommand.h>
#include <itkEuler3DTransform.h>

class StemParameterGadgetCommand : public itk::Command
{
public:

	typedef StemParameterGadgetCommand Self;
	typedef itk::Command Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;

	itkNewMacro(Self);
	itkTypeMacro(Self, Superclass);

	StemParameterGadgetCommand() {}
	virtual ~StemParameterGadgetCommand() override {}

	virtual void Execute(itk::Object *caller, const itk::EventObject & event) override
	{
		this->Execute(const_cast<const itk::Object*>(caller), event);
	}

	virtual void Execute(const itk::Object *caller, const itk::EventObject & event) override
	{
		itkNotUsed(event);
    // MITK_INFO << *caller;
    typedef typename itk::MatrixOffsetTransformBase<double, 3> MatrixOffsetTransformBase;
    const MatrixOffsetTransformBase * matrixOffsetTransformBase =
      static_cast<const MatrixOffsetTransformBase*>(caller);
    typedef typename itk::Euler3DTransform<double> Euler3DTransform;
    Euler3DTransform::Pointer euler3DTransform = 
      Euler3DTransform::New();
    euler3DTransform->SetMatrix(matrixOffsetTransformBase->GetMatrix());
    double angleZ = euler3DTransform->GetAngleZ() * itk::Math::deg_per_rad;
		this_->mUi->spinBoxFemoralVersion->setValue(itk::Math::Round<int, double>(angleZ));
	}
	const StemParameterGadget *this_ = nullptr;
private:
	ITK_DISALLOW_COPY_AND_ASSIGN(StemParameterGadgetCommand);
};

StemParameterGadget::StemParameterGadget(
  const QString &femoralStem,
	const QString &femoralHead,
	QWidget *parent) :
	QWidget(parent),
  mUi(new Ui::StemParameterGadget),
	femoralStem(femoralStem),
	femoralHead(femoralHead)
{
  this->mUi->setupUi(this);
}

StemParameterGadget::~StemParameterGadget()
{
  delete this->mUi;
}

void StemParameterGadget::ObserverStem() const
{
	mitk::DataStorage *ds =
		mitk::RenderingManager::GetInstance()->GetDataStorage();
  mitk::Surface *surface[2] = {
		ds->GetNamedObject<mitk::Surface>(this->femoralStem.toStdString()),
		ds->GetNamedObject<mitk::Surface>(this->femoralHead.toStdString())
  };
	StemParameterGadgetCommand::Pointer oCommand =
		StemParameterGadgetCommand::New();
	oCommand->this_ = this;
  surface[0]->GetGeometry()->GetIndexToWorldTransform()->AddObserver(itk::ModifiedEvent(), oCommand);
  surface[0]->GetGeometry()->GetIndexToWorldTransform()->Modified();
}