#ifndef MITK_REAMER_SOURCE_H
#define MITK_REAMER_SOURCE_H
// mitk
#include <mitkSurfaceSource.h>

namespace mitk
{
class ReamerSource : public SurfaceSource
{
public:
  mitkClassMacro(ReamerSource, SurfaceSource);
  itkFactorylessNewMacro(Self);
  itkCloneMacro(Self);

  virtual void GenerateData() override;

  itkGetConstMacro(SphereRadius, double);
  itkSetMacro(SphereRadius, double);

  itkGetConstMacro(TubeRadius, double);
  itkSetMacro(TubeRadius, double);

  itkGetConstMacro(Length, double);
  itkSetMacro(Length, double);

  itkGetConstMacro(ThetaResolution, int);
  itkSetMacro(ThetaResolution, int);

  itkGetConstMacro(PhiResolution, int);
  itkSetMacro(PhiResolution, int);

  itkBooleanMacro(HaveTube);
  itkGetConstMacro(HaveTube, bool);
  itkSetMacro(HaveTube, bool);

protected:
  double m_SphereRadius = 10;
  double m_TubeRadius = 10;
  double m_Length = 10;
  int m_ThetaResolution = 100;
  int m_PhiResolution = 100;
  bool m_HaveTube = true;
};
}  // namespace mitk

#endif  //! MITK_REAMER_SOURCE_H