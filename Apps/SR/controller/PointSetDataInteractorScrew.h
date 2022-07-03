/**
 * @file PointSetDataInteractorScrew.h
 * @author wuzhuobin (jiejin2022@163.com)
 * @brief
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef POINT_SET_DATA_INTERACTOR_SCREW_H
#define POINT_SET_DATA_INTERACTOR_SCREW_H

// mitk
#include <mitkDataNode.h>
#include <mitkPointSetDataInteractor.h>

class PointSetDataInteractorScrew : public mitk::PointSetDataInteractor
{
public:
  mitkClassMacro(PointSetDataInteractorScrew, mitk::PointSetDataInteractor);
  itkFactorylessNewMacro(Self);
  itkCloneMacro(Self);

  itkSetObjectMacro(Screw, mitk::DataNode);
  itkGetObjectMacro(Screw, mitk::DataNode);

  itkSetMacro(ScrewDiameter, double);
  itkGetMacro(ScrewDiameter, double);

  itkSetMacro(ScrewLength, double);
  itkGetMacro(ScrewLength, double);

  void updateScrew();

protected:
  void AddPoint(mitk::StateMachineAction* action,
                mitk::InteractionEvent* event) override;
  void FinishMove(mitk::StateMachineAction* action,
                  mitk::InteractionEvent* event) override;

private:
  mitk::DataNode::Pointer m_Screw;
  double m_ScrewDiameter = 0;
  double m_ScrewLength = 0;
};

#endif  // ! POINT_SET_DATA_INTERACTOR_SCREW_H