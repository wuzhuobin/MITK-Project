# %%
# conda install jupyter simpleitk vtk numpy
import SimpleITK as sitk

# %%
image = sitk.ReadImage("../Data/output.nii.gz")
threahold = sitk.ThresholdImageFilter()
threahold.SetLower(1)
threahold.SetUpper(1)
pelvis = threahold.Execute(image)
sitk.WriteImage(pelvis, "../Data/pelvis.nii.gz")

# %%
threahold.SetLower(2)
threahold.SetUpper(2)
left = threahold.Execute(image)
sitk.WriteImage(left, "../Data/left.nii.gz")

# %%
threahold.SetLower(3)
threahold.SetUpper(3)
right = threahold.Execute(image)
sitk.WriteImage(right, "../Data/right.nii.gz")

# %%
