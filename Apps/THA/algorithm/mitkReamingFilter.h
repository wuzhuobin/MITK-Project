#ifndef MITK_REAMING_FILTER_H
#define MITK_REAMING_FILTER_H

// mitk
#include <mitkImageToSurfaceFilter.h>

namespace mitk {
class ReamingFilter : public ImageToSurfaceFilter
{
public:
  mitkClassMacro(ReamingFilter, ImageToSurfaceFilter);
  itkFactorylessNewMacro(Self);
  
  using ImageToSurfaceFilter::GetInput;
  using ImageToSurfaceFilter::SetInput;

  virtual const mitk::Surface * GetInput(unsigned int i);

  virtual void SetInput(unsigned int i, const mitk::Surface *input);

  void SetReamer(const mitk::Surface *reamer) { this->SetInput(0, reamer); }

  void SetShell(const mitk::Surface *shell) { this->SetInput(1, shell); }

  itkGetConstMacro(Tolerance, double);
  itkSetMacro(Tolerance, double);

protected:

  virtual void GenerateData() override;

  void CreateSurface(int time, vtkImageData *vtkimage, mitk::Surface *surface, const ScalarType threshold);

  Image::Pointer Stencil3DImage(const mitk::Surface *surface, int time = 0);

  double m_Tolerance = 0.0;
};
}

#endif //! MITK_REAMING_FILTER_H