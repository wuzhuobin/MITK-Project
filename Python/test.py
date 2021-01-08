# %%
# conda install jupyter simpleitk vtk numpy
import SimpleITK as sitk
# %%
image = sitk.ReadImage("../Data/CTA 1.0 CE.nrrd")
print(image)
# %%
label = sitk.ReadImage("../Data/label.nii.gz")
print(label)

#%%
# label_array = sitk.GetArrayFromImage(label)
size = label.GetSize()
print("size: ", size)

# %%
seeds = []
for i in range(82, 426, 1):
  print(len(seeds))
  for j in range(220, 349, 1):
    for k in range(1147, size[2], 1):
      if (label[i, j, k] == 1):
        seeds.append([i, j, k])

          
# %%
confidenceConnected = sitk.ConfidenceConnectedImageFilter()
confidenceConnected.SetSeedList(seeds)
confidenceConnected.SetNumberOfIterations(1)
confidenceConnected.SetMultiplier(1)
confidenceConnected.SetInitialNeighborhoodRadius(1)
confidenceConnected.SetReplaceValue(2)
output = confidenceConnected.Execute(image)
# %%
sitk.WriteImage(output, "../Data/output.nii.gz")

# %%