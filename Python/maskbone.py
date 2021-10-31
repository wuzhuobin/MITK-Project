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
sitk.WriteImage(label, "../Data/label.nii.gz")
print("OrImageFilter")

# %%
binaryThreshold = sitk.BinaryThresholdImageFilter()
binaryThreshold.SetInsideValue(1)
binaryThreshold.SetUpperThreshold(10)
binaryThreshold.SetLowerThreshold(1)
binaryThreshold.SetOutsideValue(0)
label = binaryThreshold.Execute(label)
sitk.WriteImage(label, "../Data/label.nii.gz")
print("BinaryThresholdImageFilter")
# %%
maskImage = sitk.MaskImageFilter()
maskImage.SetOutsideValue(-1024)
mask = maskImage.Execute(image, label)
sitk.WriteImage(mask, "../Data/mask.nii.gz")
print("MaskImageFilter")
# %%

threshold = sitk.ThresholdImageFilter()
threshold.SetOutsideValue(-1024)
threshold.SetUpper(1)
mask = threshold.Execute(mask)
sitk.WriteImage(mask, "../Data/mask.nii.gz")
print("ThresholdImageFilter")
# %%
