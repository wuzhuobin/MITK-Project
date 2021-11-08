// vtk
#include <vtkCallbackCommand.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCubeSource.h>
#include <vtkMatrix4x4.h>


int main(int argc, char* argv[])
{
    auto sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(5);
    sphereSource->SetPhiResolution(100);
    sphereSource->SetPhiResolution(100);
    sphereSource->Update();

    auto sphere = sphereSource->GetOutput();

    auto sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputData(sphere);

    auto sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);

    auto cubeSource = vtkSmartPointer<vtkCubeSource>::New();
    cubeSource->SetXLength(20);
    cubeSource->SetYLength(1);
    cubeSource->SetZLength(1);
    cubeSource->Update();

    auto cube = cubeSource->GetOutput();

    auto cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cubeMapper->SetInputData(cube);

    auto callback = vtkSmartPointer<vtkCallbackCommand>::New();
    callback->SetCallback([](vtkObject *caller, unsigned long, void *, void *) {
      auto *cubeActor = static_cast<vtkActor *>(caller);
      std::cout << *cubeActor;
      std::cout << *cubeActor->GetMatrix();
    });

    auto cubeActor = vtkSmartPointer<vtkActor>::New();
    cubeActor->SetMapper(cubeMapper);
    cubeActor->AddObserver(vtkCallbackCommand::ModifiedEvent, callback);

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(sphereActor);
    renderer->AddActor(cubeActor);

    auto renderWin = vtkSmartPointer<vtkRenderWindow>::New();
    renderWin->AddRenderer(renderer);

    auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWin);

    renderWin->Render();
    interactor->Initialize();
    interactor->Start();
    interactor->Render();


    return 0;
}