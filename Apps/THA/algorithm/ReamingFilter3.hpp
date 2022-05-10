#ifndef REAMING_FILTER_3_HPP
#define REAMING_FILTER_3_HPP

// vtk
#include <vtkPolyDataAlgorithm.h>
#include <vtkSmartPointer.h>

class vtkBooleanOperationPolyDataFilter;
class vtkTriangleFilter;
class vtkCleanPolyData;
class vtkDecimatePro;
class ReamingFilter3 : public vtkPolyDataAlgorithm
{
public:
  static ReamingFilter3* New();
  vtkTypeMacro(ReamingFilter3, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /** @{ */
  /**
   * @brief Input1
   */
  void SetReamer(vtkPolyData* reamer);
  vtkPolyData* GetReamer();
  /** @} */
  /** @{ */
  /**
   * @brief Input2
   */
  void SetReamerTrajectory(vtkPolyData* trajectory);
  vtkPolyData* GetReamerTrajectory();
  /** @} */

  vtkSetMacro(Size, int);
  vtkGetMacro(Size, int);

  vtkBooleanMacro(UseSmooth, bool);
  vtkSetMacro(UseSmooth, bool);
  vtkGetMacro(UseSmooth, bool);

  vtkBooleanMacro(Reset, bool);
  vtkSetMacro(Reset, bool);
  vtkGetMacro(Reset, bool);

  vtkBooleanMacro(Decimate, bool);
  vtkSetMacro(Decimate, bool);
  vtkGetMacro(Decimate, bool);

  // vtkBooleanMacro(UseTriangleInput, bool);
  // vtkSetMacro(UseTriangleInput, bool);
  // vtkGetMacro(UseTriangleInput, bool);

  // vtkBooleanMacro(UseTriangleReamer, bool);
  // vtkSetMacro(UseTriangleReamer, bool);
  // vtkGetMacro(UseTriangleReamer, bool);

protected:
  template <typename T>
  using Ptr = vtkSmartPointer<T>;
  int Size = 2;
  bool UseSmooth = false;
  bool Decimate = true;
  bool Reset = true;
  bool UseTriangleInput = true;
  bool UseTriangleReamer = true;

  Ptr<vtkPolyData> IntermediatePolyData;
  Ptr<vtkBooleanOperationPolyDataFilter> BooleanOperationPolyDataFilter;
  Ptr<vtkTriangleFilter> TriangleFilter1;
  Ptr<vtkCleanPolyData> CleanPolyData1;
  Ptr<vtkDecimatePro> DecimatePro;
  Ptr<vtkTriangleFilter> TriangleFilter2;
  Ptr<vtkCleanPolyData> CleanPolyData2;

  ReamingFilter3();
  int RequestData(vtkInformation* info,
                  vtkInformationVector** input,
                  vtkInformationVector* output) override;
  int FillInputPortInformation(int port, vtkInformation* info) override;
};

#endif  // !REAMING_FILTER_3_HPP
