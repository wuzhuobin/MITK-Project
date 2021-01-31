#ifndef MITK_REAMER_SOURCE_H
#define MITK_REAMER_SOURCE_H
// mitk
#include <mitkSurfaceSource.h>

namespace mitk
{
class ReamerSource: public SurfaceSource
{
public:
  mitkClassMacro(ReamerSource, SurfaceSource);
  itkFactorylessNewMacro(Self);
  itkCloneMacro(Self);

  virtual void GenerateData() override;

  itkGetConstMacro(Radius, double);
  itkSetMacro(Radius, double);

  itkGetConstMacro(Length, double);
  itkSetMacro(Length, double);

  itkGetConstMacro(ThetaResolution, int);
  itkSetMacro(ThetaResolution, int);

  itkGetConstMacro(PhiResolution, int);
  itkSetMacro(PhiResolution, int);

protected:
  double m_Radius = 10;
  double m_Length = 10;
  int m_ThetaResolution = 100;
  int m_PhiResolution = 100;
};
}


#endif //!MITK_REAMER_SOURCE_H