# %%
import vtk
# %%
polyDataReader = vtk.vtkPolyDataReader()
polyDataReader.SetFileName("../Data/extracted_1.vtk")
polyDataReader.Update()
cup = polyDataReader.GetOutput()
print(cup)
# %%
polyDataReader = vtk.vtkXMLPolyDataReader()
polyDataReader.SetFileName("../Data/pelvis.vtp")
polyDataReader.Update()
pelvis = polyDataReader.GetOutput()
print(pelvis)
# %%
selectEnclosedPoints = vtk.vtkSelectEnclosedPoints()
selectEnclosedPoints.SetInputData(pelvis)
selectEnclosedPoints.SetSurfaceData(cup)
selectEnclosedPoints.Update()
selected = selectEnclosedPoints.GetOutput()
print(selected)
# %%
selectedPoints = selected.GetPointData().GetArray("SelectedPoints")
print(selectedPoints)
# %%
color = vtk.vtkUnsignedCharArray()
color.SetNumberOfComponents(3)
color.SetName("Colors")
for i in range(0, selectedPoints.GetSize(), 1):
  if selectedPoints.GetValue(i) == 0:
    color.InsertNextTypedTuple((255, 255, 255))
  else:
    color.InsertNextTypedTuple((0, 255, 0))
print(color)
selected.GetPointData().RemoveArray("SelectedPoints")
selected.GetPointData().SetScalars(color)
# %%
polyDataWriter = vtk.vtkPolyDataWriter()
polyDataWriter.SetInputData(selected)
polyDataWriter.SetFileName("../Data/selected.vtk")
polyDataWriter.Write()

# %%
mapper = vtk.vtkPolyDataMapper()
mapper.SetInputData(selected)

actor = vtk.vtkActor()
actor.SetMapper(mapper)

renderer = vtk.vtkRenderer()
renderer.AddActor(actor)

renderWindow = vtk.vtkRenderWindow()
renderWindow.AddRenderer(renderer)

interactor = vtk.vtkRenderWindowInteractor()
interactor.SetRenderWindow(renderWindow)
interactor.Start()
# %%
