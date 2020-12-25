#include "CupParameterGadget.h"
#include "ui_CupParameterGadget.h"
// mitk
#include <mitkRenderingManager.h>
#include <mitkDataStorage.h>
#include <mitkPointSet.h>
#include <mitkVectorProperty.h>
// itk
#include <itkCommand.h>

class CupParameterGadgetOrientationCommand: public itk::Command
{
public:

  typedef CupParameterGadgetOrientationCommand Self;
  typedef itk::Command Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(Self, Superclass);

  CupParameterGadgetOrientationCommand() {}
  virtual ~CupParameterGadgetOrientationCommand() override {}

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
		if (orientation->GetValue()[1] > 180)
		{
			this_->ui->spinBoxCupPlanInclination->setValue(360 - orientation->GetValue()[1]);
			this_->ui->spinBox_CupPlanVersion->setValue(orientation->GetValue()[2]);
		}
		else
		{
			this_->ui->spinBoxCupPlanInclination->setValue(orientation->GetValue()[1]);
			this_->ui->spinBox_CupPlanVersion->setValue(orientation->GetValue()[2]);
		}
			//this_->ui->spinBoxCupPlanInclination->setValue(orientation->GetValue()[1]);
			//this_->ui->spinBox_CupPlanVersion->setValue(orientation->GetValue()[2] - 180);
  }
  const CupParameterGadget *this_ = nullptr;
private:
  ITK_DISALLOW_COPY_AND_ASSIGN(CupParameterGadgetOrientationCommand);
};

class CupParameterGadgetPositionCommand: public itk::Command
{
public:

  typedef CupParameterGadgetPositionCommand Self;
  typedef itk::Command Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(Self, Superclass);

  CupParameterGadgetPositionCommand() {}
  virtual ~CupParameterGadgetPositionCommand() override {}


  virtual void Execute(itk::Object *caller, const itk::EventObject &event) override
  {
    this->Execute(const_cast<const itk::Object *>(caller), event);
  }

  virtual void Execute(const itk::Object *caller, const itk::EventObject &event) override
  {
    itkNotUsed(event);
    const mitk::Vector3DProperty *position =
      reinterpret_cast<const mitk::Vector3DProperty *>(caller);
    mitk::DataStorage *ds = 
      mitk::RenderingManager::GetInstance()->GetDataStorage();
    mitk::PointSet *pointset = ds->GetNamedObject<mitk::PointSet>(this_->cor.toStdString());
    //mitk::Point3D point3d = pointset->GetPoint(0);
		mitk::Point3D point3d = pointset->GetPoint(1);
    // @todo current calculation seems to not correct.
    this_->ui->spinBoxMedial->setValue(position->GetValue()[0] - point3d[0]);
    this_->ui->spinBoxSuperior->setValue(position->GetValue()[2] - point3d[2]);
    this_->ui->spinBoxPosterior->setValue(position->GetValue()[1] - point3d[1]);
  }

  const CupParameterGadget *this_ = nullptr;
private:
  // ITK_DISALLOW_COPY_AND_ASSIGN(CupParameterGadgetPositionCommand);
};

CupParameterGadget::CupParameterGadget(
  const QString &acetabularShell,
  const QString &acetabularLiner,
  const QString &cor,
  QWidget *parent
  ):
  QWidget(parent),
  ui(new Ui::CupParameterGadget),
  acetabularShell(acetabularShell),
  acetabularLiner(acetabularLiner),
  cor(cor)
{
  this->ui->setupUi(this);
}

CupParameterGadget::~CupParameterGadget()
{
  delete this->ui;
}

void CupParameterGadget::observerCup() const
{
  mitk::DataStorage *ds = 
    mitk::RenderingManager::GetInstance()->GetDataStorage();
  mitk::DataNode * surfaceNode[2] = {
    ds->GetNamedNode(this->acetabularShell.toStdString()),
    ds->GetNamedNode(this->acetabularLiner.toStdString())
  };
  CupParameterGadgetOrientationCommand::Pointer oCommand =
    CupParameterGadgetOrientationCommand::New();
  oCommand->this_ = this;
  surfaceNode[0]->GetProperty("orientation")->AddObserver(itk::ModifiedEvent(), oCommand);
  surfaceNode[0]->GetProperty("orientation")->Modified();

  CupParameterGadgetPositionCommand::Pointer pCommand =
    CupParameterGadgetPositionCommand::New();
  pCommand->this_ = this;
  surfaceNode[0]->GetProperty("position")->AddObserver(itk::ModifiedEvent(), pCommand);
  surfaceNode[0]->GetProperty("position")->Modified();

}

