# %%
import vtk
# %%
sphereSource = vtk.vtkSphereSource()
sphereSource.SetCenter(0, 0, 0)
sphereSource.SetRadius(5.0)
# Make the surface smooth.
sphereSource.SetPhiResolution(100)
sphereSource.SetThetaResolution(100)
sphereSource.Update()

sphereMapper = vtk.vtkPolyDataMapper()
sphereMapper.SetInputData(sphereSource.GetOutput())

sphereActor = vtk.vtkActor()
sphereActor.SetMapper(sphereMapper)


cubeSource = vtk.vtkCubeSource()
cubeSource.SetCenter(0, 0, 0)
cubeSource.SetXLength(20)
cubeSource.SetYLength(2)
cubeSource.SetZLength(2)
cubeSource.Update()

cubeMapper = vtk.vtkPolyDataMapper()
cubeMapper.SetInputData(cubeSource.GetOutput())

cubeActor = vtk.vtkActor()
cubeActor.SetMapper(cubeMapper)
cubeActor.GetProperty().SetOpacity(0.2)


renderer = vtk.vtkRenderer()
renderer.AddActor(sphereActor)
renderer.AddActor(cubeActor)

renderWindow = vtk.vtkRenderWindow()
renderWindow.AddRenderer(renderer)
renderWindowInteractor = vtk.vtkRenderWindowInteractor()
renderWindowInteractor.SetRenderWindow(renderWindow)
# renderer.SetBackground(colors.GetColor3d("DarkGreen"))

renderWindow.Render()
renderWindowInteractor.Start()

# %%
plane1 = vtk.vtkPlane()
plane1.SetOrigin(0, 0.5, 0.5)
plane1.SetNormal(0, 0, 1)

plane2 = vtk.vtkPlane()
plane2.SetOrigin(0, 0.5, 0.5)
plane2.SetNormal(0, 0.1, -0.8)

planes = vtk.vtkPlaneCollection()
planes.AddItem(plane1)
planes.AddItem(plane2)

clipper = vtk.vtkClipClosedSurface()
clipper.SetInputData(sphereSource.GetOutput())
clipper.SetClippingPlanes(planes)
clipper.Update()
data = clipper.GetOutput()

# %%
mapper = vtk.vtkPolyDataMapper()
mapper.SetInputData(data)
# mapper.SetInputConnection(sphereSource.GetOutputPort())

actor = vtk.vtkActor()
actor.SetMapper(mapper)

mapper1 = vtk.vtkPolyDataMapper()
mapper1.SetInputConnection(sphereSource.GetOutputPort())

actor1 = vtk.vtkActor()
actor1.SetMapper(mapper1)
actor1.GetProperty().SetOpacity(0.2)

renderer = vtk.vtkRenderer()
renderWindow = vtk.vtkRenderWindow()
renderWindow.SetWindowName("Sphere")
renderWindow.AddRenderer(renderer)
renderWindowInteractor = vtk.vtkRenderWindowInteractor()
renderWindowInteractor.SetRenderWindow(renderWindow)

renderer.AddActor(actor)
renderer.AddActor(actor1)
# renderer.SetBackground(colors.GetColor3d("DarkGreen"))

renderWindow.Render()
renderWindowInteractor.Start()
# %%
