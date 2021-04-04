#include "CupParameterGadget.h"
#include "ui_CupParameterGadget.h"
// mitk
#include <mitkRenderingManager.h>
#include <mitkDataStorage.h>
#include <mitkPointSet.h>
#include <mitkSurface.h>
// #include <mitkVectorProperty.h>
// itk
#include <itkCommand.h>
#include <itkEuler3DTransform.h>

class CupParameterGadgetCommand: public itk::Command
{
public:

  typedef CupParameterGadgetCommand Self;
  typedef itk::Command Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(Self, Superclass);

  CupParameterGadgetCommand() {}
  virtual ~CupParameterGadgetCommand() override {}

  virtual void Execute(itk::Object *caller, const itk::EventObject & event) override
  {
    this->Execute(const_cast<const itk::Object*>(caller), event);
  }

  virtual void Execute(const itk::Object *caller, const itk::EventObject & event) override
  {
    itkNotUsed(event);

    typedef typename itk::MatrixOffsetTransformBase<double, 3> MatrixOffsetTransformBase;
    const MatrixOffsetTransformBase * matrixOffsetTransformBase =
      static_cast<const MatrixOffsetTransformBase*>(caller);
    typedef typename itk::Euler3DTransform<double> Euler3DTransform;
    Euler3DTransform::Pointer euler3DTransform =
      Euler3DTransform::New();
    euler3DTransform->SetMatrix(matrixOffsetTransformBase->GetMatrix());
    double angleX = euler3DTransform->GetAngleX() * itk::Math::deg_per_rad;
    double angleY = euler3DTransform->GetAngleY() * itk::Math::deg_per_rad;
    double angleZ = euler3DTransform->GetAngleZ() * itk::Math::deg_per_rad;

    mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();
    mitk::PointSet *pointSet = ds->GetNamedObject<mitk::PointSet>(this_->cupCor.toStdString());
    mitk::Point3D origin = pointSet->GetPoint(0);

    // Since the model has been rotated 40 degreee by default.
    this_->ui->spinBoxCupPlanInclination->setValue(angleY);
    this_->ui->spinBox_CupPlanVersion->setValue(angleZ);
    mitk::PointSet *pointset = ds->GetNamedObject<mitk::PointSet>(this_->nativeCor.toStdString());
		mitk::Point3D point3d = pointset->GetPoint(1);
    // @todo current calculation seems to not correct.
    this_->ui->spinBoxMedial->setValue(origin[0] - point3d[0]);
    this_->ui->spinBoxPosterior->setValue(origin[1] - point3d[1]);
    this_->ui->spinBoxSuperior->setValue(origin[2] - point3d[2]);
  }
  const CupParameterGadget *this_ = nullptr;
private:
  ITK_DISALLOW_COPY_AND_ASSIGN(CupParameterGadgetCommand);
};

CupParameterGadget::CupParameterGadget(
  const QString &acetabularShell,
  const QString &acetabularLiner,
  const QString &cupCor,
  const QString &nativeCor,
  QWidget *parent
  ):
  QWidget(parent),
  ui(new Ui::CupParameterGadget),
  acetabularShell(acetabularShell),
  acetabularLiner(acetabularLiner),
  cupCor(cupCor),
  nativeCor(nativeCor)
{
  this->ui->setupUi(this);
}

CupParameterGadget::~CupParameterGadget()
{
  delete this->ui;
}

void CupParameterGadget::ObserverCup() const
{
  mitk::DataStorage *ds = 
    mitk::RenderingManager::GetInstance()->GetDataStorage();
  mitk::Surface * surface[2] = {
    ds->GetNamedObject<mitk::Surface>(this->acetabularShell.toStdString()),
    ds->GetNamedObject<mitk::Surface>(this->acetabularLiner.toStdString())
  };
  CupParameterGadgetCommand::Pointer oCommand =
    CupParameterGadgetCommand::New();
  oCommand->this_ = this;
  surface[0]->GetGeometry()->GetIndexToWorldTransform()->AddObserver(itk::ModifiedEvent(), oCommand);
  surface[0]->GetGeometry()->GetIndexToWorldTransform()->Modified();
}
