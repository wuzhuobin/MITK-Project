# %%
import vtk
# %%
stlReader = vtk.vtkSTLReader()
stlReader.SetFileName("../Data/implant.stl")
stlReader.Update()
implant = stlReader.GetOutput()
print(implant)

# %%
bounds = implant.GetBounds()
center = (
  (bounds[1] + bounds[0]) * 0.5, 
  (bounds[3] - bounds[2]) * 0.5, 
  (bounds[5] - bounds[4]) * 0.5
)
print("center: ", center)

# %%
connectivityFilter = vtk.vtkPolyDataConnectivityFilter()
connectivityFilter.SetInputData(implant)
connectivityFilter.SetExtractionModeToAllRegions()
connectivityFilter.Update()
regions = connectivityFilter.GetNumberOfExtractedRegions()
print("regions: ", regions)

# %%
  transform = vtk.vtkTransform()
  transform.Identity()
  transform.PostMultiply()
#   transform.Translate(-center[0], -center[1], -center[2])
  transform.RotateY(0)
  transform.RotateX(90)
  transform.RotateZ(180)
  transform.Translate(
    -98.54936906586104,
    14.204643754424644,
    1578.9678664012258
  )
#   transform.Translate(center[0], center[1], center[2])

# %%
connectivityFilter.SetExtractionModeToSpecifiedRegions()
for i in range(0, regions, 1):
  print("region: ", i)
  connectivityFilter.InitializeSpecifiedRegionList()
  connectivityFilter.AddSpecifiedRegion(i)
  connectivityFilter.Update()
  extracted = connectivityFilter.GetOutput()

  transformPolyData = vtk.vtkTransformPolyDataFilter()
  transformPolyData.SetInputData(extracted)
  transformPolyData.SetTransform(transform)
  transformPolyData.Update()
  extracted = transformPolyData.GetOutput()

  polyDataWriter = vtk.vtkPolyDataWriter()
  polyDataWriter.SetInputData(extracted)
  polyDataWriter.SetFileName("../Data/extracted_" + str(i) + ".vtk")
  polyDataWriter.Write()
# %%

polyDataReader = vtk.vtkPolyDataReader()
polyDataReader.SetFileName("../Data/extracted_2.vtk")
polyDataReader.Update()
extracted_2 = polyDataReader.GetOutput()
rotatoinCenter = (
  (extracted_2.GetBounds()[0] + extracted_2.GetBounds()[1]) * 0.5,
  (extracted_2.GetBounds()[2] + extracted_2.GetBounds()[3]) * 0.5,
  (extracted_2.GetBounds()[4] + extracted_2.GetBounds()[5]) * 0.5,
)
print("rotationCenter: ", rotatoinCenter)
# %%
for i in range(0, 1, 1):
  polyDataReader.SetFileName("../Data/extracted_" + str(i) + ".vtk")
  polyDataReader.Update()
  
  transform = vtk.vtkTransform()
  transform.PostMultiply()
  transform.Identity()
  transform.Translate(
    -rotatoinCenter[0],
    -rotatoinCenter[1],
    -rotatoinCenter[2]
  )
  transform.RotateY(40)
  transform.Translate(
    rotatoinCenter[0],
    rotatoinCenter[1],
    rotatoinCenter[2]
  )
  
  transformPolyData = vtk.vtkTransformPolyDataFilter()
  transformPolyData.SetInputData(polyDataReader.GetOutput())
  transformPolyData.SetTransform(transform)
  transformPolyData.Update()
  extracted_1 = transformPolyData.GetOutput()
  
  polyDataWriter = vtk.vtkPolyDataWriter()
  polyDataWriter.SetInputData(extracted_1)
  polyDataWriter.SetFileName("../Data/transformed_extracted_" + str(i) + ".vtk")
  polyDataWriter.Write()
# %%
