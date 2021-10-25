#%%
import SimpleITK as sitk;
import vtk
import numpy as np

# %%
knee = sitk.ReadImage(fileName="../Data/knee.nrrd")
print(knee)
# %%
otsuFilter = sitk.OtsuMultipleThresholdsImageFilter()
otsuFilter.SetNumberOfThresholds(2)
kneeSeg = otsuFilter.Execute(knee)
sitk.WriteImage(kneeSeg, fileName="../Data/knee-seg.nrrd")
# %%
kneeSegThresholds = []
for i in range(1, 3):
	thresholdFilter = sitk.ThresholdImageFilter()
	thresholdFilter.SetUpper(i)
	thresholdFilter.SetLower(i)
	kneeSegThreshold = thresholdFilter.Execute(kneeSeg)
	kneeSegThresholds.append(kneeSegThreshold)
	sitk.WriteImage(kneeSegThreshold, "../Data/knee-seg-threshold-" + str(i) + ".nrrd")
# %%
connectedComponentFilter = sitk.ConnectedComponentImageFilter()
connectedComponentFilter.SetFullyConnected(False)
kneeSegThresholdConnected = connectedComponentFilter.Execute(kneeSegThresholds[1])
sitk.WriteImage(kneeSegThresholdConnected, "../Data/knee-seg-threshold-2-connected.nrrd")
# %%
binaryThreshold = sitk.BinaryThresholdImageFilter()
binaryThreshold.SetInsideValue(1)
binaryThreshold.SetOutsideValue(0)
binaryThreshold.SetUpperThreshold(1)
binaryThreshold.SetLowerThreshold(1)
kneeSegThresholdConnectedThreshold = binaryThreshold.Execute(kneeSegThresholdConnected)
sitk.WriteImage(kneeSegThresholdConnectedThreshold, "../Data/knee-seg-threshold-2-connected-threshold.nrrd")
# %%
regionOfInterest = sitk.RegionOfInterestImageFilter()
index = sitk.VectorInt32([173, 176, 4])
regionOfInterest.SetIndex(index)
size = sitk.VectorUInt32([336 - 173, 248 - 176, 160 - 4])
regionOfInterest.SetSize(size)
regionOfInterest.SetRegionOfInterest()
kneeRegion = regionOfInterest.Execute(knee)
sitk.WriteImage(kneeRegion, "../Data/knee-region.nrrd")
kneeRegionOtsu = otsuFilter.Execute(kneeRegion)
sitk.WriteImage(kneeRegionOtsu, "../Data/knee-region-otsu.nrrd")
# %%
