#ifndef REAMING_FILTER_1_H
#define REAMING_FILTER_1_H

// vtk
#include <vtkPolyDataAlgorithm.h>
#include <vtkSmartPointer.h>

class vtkImageData;
class vtkImageDilateErode3D;
class vtkImageMathematics;
class vtkPolyDataToImageStencil;
class vtkImageStencil;
class vtkAbstractTransform;
class vtkTransformPolyDataFilter;
class vtkDiscreteFlyingEdges3D;
class vtkWindowedSincPolyDataFilter;
class ReamingFilter2 : public vtkPolyDataAlgorithm
{
public:
  static ReamingFilter2* New();
  vtkTypeMacro(ReamingFilter2, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  void SetPelvis(vtkImageData* pelvis);
  vtkImageData* GetPelvis();
  void SetReamer(vtkPolyData* reamer);
  vtkPolyData* GetReamer();
  void SetReamerTrajectory(vtkPolyData* trajectory);
  vtkPolyData* GetReamerTrajectory();

  vtkSetMacro(Size, int);
  vtkGetMacro(Size, int);

  virtual void SetImageTransform(vtkAbstractTransform* ImageTransform);
  vtkGetObjectMacro(ImageTransform, vtkAbstractTransform);

protected:
  template <typename T>
  using Ptr = vtkSmartPointer<T>;
  int Size = 2;
  vtkAbstractTransform* ImageTransform = nullptr;

  Ptr<vtkTransformPolyDataFilter> TransformPolyDataFilter1;
  Ptr<vtkTransformPolyDataFilter> TransformPolyDataFilter2;
  Ptr<vtkPolyDataToImageStencil> PolyDataToImageStencil1;
  Ptr<vtkPolyDataToImageStencil> PolyDataToImageStencil2;
  Ptr<vtkImageStencil> ImageStencil1;
  Ptr<vtkImageStencil> ImageStencil2;
  Ptr<vtkImageDilateErode3D> Erode;
  Ptr<vtkImageMathematics> ImageSubstractTrajectory;
  Ptr<vtkImageMathematics> ImageMultiplyBy2;
  Ptr<vtkImageMathematics> ImageAddTrajectory;
  Ptr<vtkImageMathematics> ImageAddErode;
  Ptr<vtkDiscreteFlyingEdges3D> DiscreteFlyingEdges;
  Ptr<vtkTransformPolyDataFilter> TransformPolyDataFilter3;
  Ptr<vtkWindowedSincPolyDataFilter> WindowedSincPolyDataFilter;

  ReamingFilter2();
  int RequestData(vtkInformation* info,
                  vtkInformationVector** input,
                  vtkInformationVector* output) override;
  int FillInputPortInformation(int port, vtkInformation* info) override;
};

#endif  // !REAMING_FILTER_1_H
