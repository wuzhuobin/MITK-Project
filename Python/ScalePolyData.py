# %%
import vtk
import numpy as np
# %%

fileNames = ("acetabular_liner", "acetabular_shell", "femoral_head", "femoral_stem")
scales = (1.1, 1.2, 1.3, 1.4, 1.5)
for fileName in fileNames:
    for scale in scales:
        front = "../Data/"
        end = ".vtp"
        fullName = front + fileName + end
        print(fullName)
        newName = front + fileName + str(scale) + end
        print(newName)

        reader = vtk.vtkXMLPolyDataReader()
        reader.SetFileName(fullName)
        reader.Update()
        # cor = [-83.667, 12.068, 1577.445]
        cor = reader.GetOutput().GetCenter()

        transform = vtk.vtkTransform()
        transform.PostMultiply()
        transform.Translate(-cor[0], -cor[1], -cor[2])
        transform.Scale(scale, scale, scale)
        transform.Translate(cor)
        transformPolyData = vtk.vtkTransformPolyDataFilter()
        transformPolyData.SetInputData(reader.GetOutput())
        transformPolyData.SetTransform(transform)
        transformPolyData.Update()
        writer = vtk.vtkXMLPolyDataWriter()
        writer.SetInputData(transformPolyData.GetOutput())
        writer.SetFileName(newName)
        writer.Write()


# %%
reader = vtk.vtkXMLPolyDataReader()
reader.SetFileName("../Data/acetabular_liner.vtp")
reader.Update()

# cor = [-83.667, 12.068, 1577.445]
cor = reader.GetOutput().GetCenter()
scale = 1.1
transform = vtk.vtkTransform()
transform.PostMultiply()
transform.Translate(-cor[0], -cor[1], -cor[2])
transform.Scale(scale, scale, scale)
transform.Translate(cor)

transformPolyData = vtk.vtkTransformPolyDataFilter()
transformPolyData.SetInputData(reader.GetOutput())
transformPolyData.SetTransform(transform)
transformPolyData.Update()

writer = vtk.vtkXMLPolyDataWriter()
writer.SetInputData(transformPolyData.GetOutput())
writer.SetFileName("../Data/acetabular_liner1.1.vtp")
writer.Write()


# %%
