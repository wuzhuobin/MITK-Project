# %%
import vtk

innerRadius = 2.0
outerRadius = 5.0
length = 10.0

# %%
diskSource = vtk.vtkDiskSource()
diskSource.SetInnerRadius(innerRadius)
diskSource.SetOuterRadius(outerRadius)
diskSource.SetRadialResolution(5)
diskSource.SetCircumferentialResolution(100)

linearExtrusionFilter = vtk.vtkLinearExtrusionFilter()
linearExtrusionFilter.SetInputConnection(diskSource.GetOutputPort())
linearExtrusionFilter.SetExtrusionTypeToVectorExtrusion()
linearExtrusionFilter.SetVector(0.0, 0.0, 1.0)
linearExtrusionFilter.SetScaleFactor(length)
linearExtrusionFilter.Update()

# %%
transform = vtk.vtkTransform()
transform.Translate(0.0, 0.0, - length / 2)

transformPolyData = vtk.vtkTransformPolyDataFilter()
transformPolyData.SetInputConnection(linearExtrusionFilter.GetOutputPort())
transformPolyData.SetTransform(transform)
transformPolyData.Update()


# %%

polyDataWriter = vtk.vtkPolyDataWriter()
polyDataWriter.SetInputConnection(transformPolyData.GetOutputPort())
polyDataWriter.SetFileName("../Data/sleeve.vtk")
polyDataWriter.Write()

# # %%


# %%
