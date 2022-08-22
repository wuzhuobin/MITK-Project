/**
 * @file AffineBaseDataInteractor3D.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef AFFINE_BASE_DATA_INTERACTOR_3D_H
#define AFFINE_BASE_DATA_INTERACTOR_3D_H

// mitk
#include <mitkAffineBaseDataInteractor3D.h>

// itk
#include <itkCommand.h>

class AffineBaseDataInteractor3D : public mitk::AffineBaseDataInteractor3D
{
public:
  mitkClassMacro(AffineBaseDataInteractor3D, mitk::AffineBaseDataInteractor3D);
  itkFactorylessNewMacro(Self);
  itkCloneMacro(Self);
  void SetDataNode(mitk::DataNode* dataNode) override;

private:
  using SimpleConstMemberCommand =
      itk::SimpleMemberCommand<AffineBaseDataInteractor3D>;
  SimpleConstMemberCommand::Pointer mCommand = SimpleConstMemberCommand::New();
  void selectSliceByPoint();
};

#endif  // !AFFINE_BASE_DATA_INTERACTOR_3D_H