/**
 * @file AffineBaseDataInteractor3D.cpp
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-07-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "AffineBaseDataInteractor3D.h"

// mitk
#include <mitkBaseData.h>
#include <mitkDataNode.h>

void AffineBaseDataInteractor3D::SetDataNode(mitk::DataNode* node)
{
  mitk::AffineBaseDataInteractor3D::SetDataNode(node);
  if (node)
  {
    mCommand->SetCallbackFunction(
        this, &AffineBaseDataInteractor3D::selectSliceByPoint);
    node->AddObserver(itk::ModifiedEvent(), mCommand);
  }
}

void AffineBaseDataInteractor3D::selectSliceByPoint()
{
  auto* dataNode = GetDataNode();
}
