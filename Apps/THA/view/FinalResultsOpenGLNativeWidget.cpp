#include "FinalResultsOpenGLNativeWidget.h"

// mitk
#include <mitkLogMacros.h>
#include <mitkRenderingManager.h>
#include <mitkDataStorage.h>
#include <mitkSurface.h>

// vtk
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

FinalResultsOpenGLNativeWidget::FinalResultsOpenGLNativeWidget(QWidget* parent) : QVTKOpenGLNativeWidget(parent) {}

void FinalResultsOpenGLNativeWidget::initializeWidget()
{
    MITK_INFO << __func__;
    auto *ds = mitk::RenderingManager::GetInstance()->GetDataStorage();

    MITK_INFO << *ds->GetNamedObject<mitk::Surface>("femoral_head");
    auto* femoralHead = ds->GetNamedObject<mitk::Surface>("femoral_head")->GetVtkPolyData();

    auto femoralHeadMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    femoralHeadMapper->SetInputData(femoralHead);

    auto femoralHeadActor = vtkSmartPointer<vtkActor>::New();
    femoralHeadActor->SetMapper(femoralHeadMapper);

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(femoralHeadActor);

    renderWindow()->AddRenderer(renderer);
}