#%%
import SimpleITK as sitk
# %%

image = sitk.ReadImage("../Data/CTA 1.0 CE.nrrd")
print(image)
pelvis = sitk.ReadImage("../Data/pelvis.nii.gz")
print(pelvis)
left = sitk.ReadImage("../Data/left.nii.gz")
print(left)
right = sitk.ReadImage("../Data/right.nii.gz")
print(right)

#%%
orImage = sitk.OrImageFilter()
label = orImage.Execute(pelvis, left)
label = orImage.Execute(label, right)
sitk.WriteImage(label, "../Data/label.nrrd")

# %%
binaryThreshold = sitk.BinaryThresholdImageFilter()
binaryThreshold.SetInsideValue(1)
binaryThreshold.SetUpperThreshold(10)
binaryThreshold.SetLowerThreshold(1)
binaryThreshold.SetOutsideValue(0)
label = binaryThreshold.Execute(label)
sitk.WriteImage(label, "../Data/label.nrrd")
# %%

maskImage = sitk.MaskImageFilter()
mask = maskImage.Execute(image, label)
sitk.WriteImage(mask, "../Data/mask.nrrd")
# %%