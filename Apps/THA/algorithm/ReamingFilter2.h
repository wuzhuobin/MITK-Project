#ifndef REAMING_FILTER_1_H
#define REAMING_FILTER_1_H

// vtk
#include <vtkPolyDataAlgorithm.h>
#include <vtkSmartPointer.h>


class ReamingFilter2 : public vtkPolyDataAlgorithm
{
public:
  static ReamingFilter2 *New();
  vtkTypeMacro(ReamingFilter2, vtkPolyDataAlgorithm);
  virtual void PrintSelf(ostream &os, vtkIndent indent) override;


protected:
  ReamingFilter2();
  virtual int RequestData(vtkInformation *info, vtkInformationVector **input,
                          vtkInformationVector *output) override;
};

#endif // !REAMING_FILTER_1_H
