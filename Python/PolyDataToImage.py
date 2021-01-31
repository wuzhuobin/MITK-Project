# %%
import vtk
import math
# %%

polyDataReader = vtk.vtkPolyDataReader()
polyDataReader.SetFileName("../Data/extracted_1.vtk")
polyDataReader.Update()
cup = polyDataReader.GetOutput()
print(cup)
# %%
print("bounds: ", cup.GetBounds())
# extent = (
#   math.floor(cup.GetBounds()[0]), math.ceil(cup.GetBounds()[1]),
#   math.floor(cup.GetBounds()[2]), math.ceil(cup.GetBounds()[3]),
#   math.floor(cup.GetBounds()[4]), math.ceil(cup.GetBounds()[5]),
# )
extent = (
  0, math.ceil(cup.GetBounds()[1] - cup.GetBounds()[0]),
  0, math.ceil(cup.GetBounds()[3] - cup.GetBounds()[2]),
  0, math.ceil(cup.GetBounds()[5] - cup.GetBounds()[4]),
)
print("extent: ", extent)
# %%
whiteImage = vtk.vtkImageData()
# whiteImage.SetOrigin(extent[0], extent[2], extent[4])
whiteImage.SetOrigin(cup.GetBounds()[0], cup.GetBounds()[2], cup.GetBounds()[4])
whiteImage.SetExtent(extent)
whiteImage.AllocateScalars(vtk.VTK_UNSIGNED_CHAR, 1)
count = whiteImage.GetNumberOfPoints()
# for (vtkIdType i = 0 i < count ++i)
for i in range(count):
    whiteImage.GetPointData().GetScalars().SetTuple1(i, 255)
print(whiteImage)
# %%
# niftiWriter = vtk.vtkNIFTIImageWriter()
# niftiWriter.SetInputData(whiteImage)
# niftiWriter.SetFileName("../Data/white_image.nii.gz")
# niftiWriter.Write()

imageWriter = vtk.vtkMetaImageWriter()
imageWriter.SetFileName("../Data/white_image.mhd")
imageWriter.SetInputData(whiteImage)
imageWriter.Write()
# %%
# polygonal data -. image stencil:
pol2stenc = vtk.vtkPolyDataToImageStencil()
# pol2stenc.SetTolerance(0)  # important if extruder.SetVector(0, 0, 1) !!!
pol2stenc.SetInputData(cup)
pol2stenc.SetOutputOrigin(cup.GetBounds()[0], cup.GetBounds()[2], cup.GetBounds()[4])
pol2stenc.SetOutputSpacing((1, 1, 1))
pol2stenc.SetOutputWholeExtent(whiteImage.GetExtent())
pol2stenc.Update()

# cut the corresponding white image and set the background:
imgstenc = vtk.vtkImageStencil()
imgstenc.SetInputData(whiteImage)
imgstenc.SetStencilConnection(pol2stenc.GetOutputPort())
imgstenc.ReverseStencilOff()
imgstenc.SetBackgroundValue(0)
# imgstenc.SetBackgroundValue(1)
imgstenc.Update()
print(imgstenc.GetOutput())

# %%
imageWriter = vtk.vtkMetaImageWriter()
imageWriter.SetFileName("../Data/cup.mhd")
imageWriter.SetInputData(imgstenc.GetOutput())
imageWriter.Write()
# niftiWriter = vtk.vtkNIFTIImageWriter()
# niftiWriter.SetInputData(imgstenc.GetOutput())
# niftiWriter.SetFileName("../Data/cup.nii.gz")
# niftiWriter.Write()
