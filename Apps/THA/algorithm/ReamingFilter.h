#ifndef REAMING_FILTER_H
#define REAMING_FILTER_H
#pragma once
// vtk
#include <vtkPolyDataAlgorithm.h>
#include <vtkSmartPointer.h>
class vtkImageData;
class vtkExtractVOI;
class vtkPolyDataToImageStencil;
class vtkImageStencil;
class vtkDiscreteFlyingEdges3D;
class vtkWindowedSincPolyDataFilter;
class vtkImageMathematics;
class vtkTransformPolyDataFilter;
class ReamingFilter: public vtkPolyDataAlgorithm
{
public:
  static ReamingFilter *New();
  vtkTypeMacro(ReamingFilter, vtkPolyDataAlgorithm);
  virtual void PrintSelf(ostream &os, vtkIndent indent) override;

  // void SetReamer(vtkPolyData * data);
  // vtkPolyData *GetReamer();
  // vtkGetObjectMacro(Reamer, vtkPolyData);

  vtkSetVector6Macro(Extent, int);
  vtkGetVector6Macro(Extent, int);

  vtkSetMacro(KeepTrace, bool);
  vtkGetMacro(KeepTrace, bool);
  vtkBooleanMacro(KeepTrace, bool);

  vtkSetMacro(FirstUpdate, bool);
  vtkGetMacro(FirstUpdate, bool);
  vtkBooleanMacro(FirstUpdate, bool);

  vtkSetMacro(Scale, double);
  vtkGetMacro(Scale, double);

protected:
  ReamingFilter();
  virtual int RequestData(vtkInformation *info, vtkInformationVector **input,
    vtkInformationVector *output) override;
  // see algorithm for more info
  virtual int FillInputPortInformation(int port, vtkInformation *info) override;

  int Extent[6];
  bool KeepTrace;
  bool FirstUpdate;
  double Scale;
  template <typename T> using Ptr = vtkSmartPointer<T>;
  Ptr<vtkExtractVOI> extractVOI;
  Ptr<vtkPolyDataToImageStencil> polyDataToImageStencil;
  Ptr<vtkImageStencil> imageStencil1;
  Ptr<vtkPolyDataToImageStencil> polyDataToImageStencil2;
  Ptr<vtkImageStencil> imageStencil2;
  Ptr<vtkPolyDataToImageStencil> polyDataToImageStencil3;
  Ptr<vtkImageStencil> imageStencil3;
  Ptr<vtkDiscreteFlyingEdges3D> flyingEdges3D;
  Ptr<vtkWindowedSincPolyDataFilter> windowedSincPolyData;
  Ptr<vtkImageMathematics> imageMathematics;
  Ptr<vtkImageMathematics> imageMathematics2;
  Ptr<vtkImageMathematics> imageMathematics3;
  Ptr<vtkTransformPolyDataFilter> transformPolyDataFilter;
};

#endif //!REAMING_FILTER_H