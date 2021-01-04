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
    itkNotUsed(caller);
    mitk::DataStorage *ds =
      mitk::RenderingManager::GetInstance()->GetDataStorage();
    mitk::DataNode* acetabularShell = ds->GetNamedNode(this_->acetabularShell.toStdString());
    float origin[3];
    acetabularShell->GetFloatProperty("origin.x", origin[0]);
    acetabularShell->GetFloatProperty("origin.y", origin[1]);
    acetabularShell->GetFloatProperty("origin.z", origin[2]);
    // @TODO current calculation seems to correct
		// if (orientation->GetValue()[1] > 180)
		// {
		// 	this_->ui->spinBoxCupPlanInclination->setValue(360 - orientation->GetValue()[1]);
		// 	this_->ui->spinBox_CupPlanVersion->setValue(orientation->GetValue()[2]);
		// }
		// else
		// {
		// 	this_->ui->spinBoxCupPlanInclination->setValue(orientation->GetValue()[1]);
		// 	this_->ui->spinBox_CupPlanVersion->setValue(orientation->GetValue()[2]);
		// }
    mitk::PointSet *pointset = ds->GetNamedObject<mitk::PointSet>(this_->cor.toStdString());
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
