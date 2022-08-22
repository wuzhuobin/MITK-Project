#ifndef SCBDentalValidationPipeline_H
#define SCBDentalValidationPipeline_H

#include <QWidget>
#include "ui_SCBDentalValidationPipeline.h"
#include <itkImage.h>
#include <itkVersorRigid3DTransform.h>

class SCBImage;
class vtkMatrix4x4;

typedef itk::Image<short, 3> ImageType;
typedef itk::Image<unsigned short, 3> LabelImageType;

struct NeedleProperty
{
	unsigned int label;
	double majorPrincipalAxes[3];
	double centroid[3];
	double needleLength;
	double start[3];
	double end[3];
};

class SCBDentalValidationPipeline :
	public QWidget, 
	public Ui::SCBDentalValidationPipeline

{
    Q_OBJECT

public:
    explicit SCBDentalValidationPipeline(QWidget *parent = 0);
    ~SCBDentalValidationPipeline();


	void UpdateUserMatrixFromTransformWidget();
public slots:
	void manualRegistrationShow();
	void rotateXSliderValueChanged(int);
	void rotateYSliderValueChanged(int);
	void rotateZSliderValueChanged(int);
	void translateXSliderValueChanged(int);
	void translateYSliderValueChanged(int);
	void translateZSliderValueChanged(int);
	void rotateXSpinBoxValueChanged(double);
	void rotateYSpinBoxValueChanged(double);
	void rotateZSpinBoxValueChanged(double);
	void translateXSpinBoxValueChanged(double);
	void translateYSpinBoxValueChanged(double);
	void translateZSpinBoxValueChanged(double);
	void resetRegistration();
	void autoValidationRun();
	void manualValidationRun();
	void sceneDataAdded(QString);
	void clearTable();

private:
	void registration(bool);
	void validation();

	void itkTransformToWidget(itk::VersorRigid3DTransform <double>::Pointer);

	SCBImage* m_preOpImage;
	SCBImage* m_postOpImage;
	ImageType::Pointer m_preOpItkImage;
	ImageType::Pointer m_postOpItkImage;
	ImageType::Pointer m_postOpItkImageReg;
	vtkMatrix4x4* m_postOpIinitialUserMatrix;
	itk::VersorRigid3DTransform <double>::Pointer m_postOpInititalItkRegTransform;
	double m_needleVol; // need to be change dynamicly, now is set to constant
	std::vector<NeedleProperty*> m_plantedNeedlePropertyCollections;
	std::vector<NeedleProperty*> m_plannedNeedlePropertyCollections;

signals:
	void signalConfirmValidation();

};

#endif // SCBDentalValidationPipeline_H
