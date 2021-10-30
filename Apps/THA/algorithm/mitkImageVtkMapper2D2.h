#ifndef MITK_IMAGE_VTK_MAPPER_2D_2_H
#define MITK_IMAGE_VTK_MAPPER_2D_2_H

// mitk
#include <mitkImageVtkMapper2D.h>

// MITK
#include <mitkAbstractTransformGeometry.h>
#include <mitkDataNode.h>
#include <mitkImageSliceSelector.h>
#include <mitkLevelWindowProperty.h>
#include <mitkLookupTableProperty.h>
#include <mitkPixelType.h>
#include <mitkPlaneGeometry.h>
#include <mitkProperties.h>
#include <mitkPropertyNameHelper.h>
#include <mitkResliceMethodProperty.h>
#include <mitkVtkResliceInterpolationProperty.h>

//#include <mitkTransferFunction.h>
#include <mitkTransferFunctionProperty.h>

#include "mitkImageStatisticsHolder.h"
#include "mitkPlaneClipping.h"

// MITK Rendering
#include "mitkImageVtkMapper2D.h"
#include "vtkMitkLevelWindowFilter.h"
#include "vtkMitkThickSlicesFilter.h"
#include "vtkNeverTranslucentTexture.h"

// VTK
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkColorTransferFunction.h>
#include <vtkGeneralTransform.h>
#include <vtkImageChangeInformation.h>
#include <vtkImageData.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageReslice.h>
#include <vtkLookupTable.h>
#include <vtkMatrix4x4.h>
#include <vtkPlaneSource.h>
#include <vtkPoints.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkTransform.h>

// ITK
#include <itkRGBAPixel.h>
#include <mitkRenderingModeProperty.h>

namespace mitk
{
class ImageVtkMapper2D2 : public ImageVtkMapper2D
{
public:
    /** Standard class typedefs. */
    mitkClassMacro(ImageVtkMapper2D2, ImageVtkMapper2D);

    /** Method for creation through the object factory. */
    itkFactorylessNewMacro(Self);
    itkCloneMacro(Self);

protected:
    void GenerateDataForRenderer(mitk::BaseRenderer* renderer) override
    {
        LocalStorage* localStorage = m_LSH.GetLocalStorage(renderer);

        auto* image = const_cast<mitk::Image*>(this->GetInput());
        mitk::DataNode* datanode = this->GetDataNode();
        if (nullptr == image || !image->IsInitialized())
        {
            this->SetToInvalidState(localStorage);
            return;
        }

        // check if there is a valid worldGeometry
        const PlaneGeometry* worldGeometry = renderer->GetCurrentWorldPlaneGeometry();
        if (nullptr == worldGeometry || !worldGeometry->IsValid() || !worldGeometry->HasReferenceGeometry())
        {
            this->SetToInvalidState(localStorage);
            return;
        }

        image->Update();

        localStorage->m_PublicActors = localStorage->m_Actors.Get();

        // early out if there is no intersection of the current rendering geometry
        // and the geometry of the image that is to be rendered.
        if (!RenderingGeometryIntersectsImage(worldGeometry, image->GetSlicedGeometry()))
        {
            this->SetToInvalidState(localStorage);
            return;
        }

        // set main input for ExtractSliceFilter
        localStorage->m_Reslicer->SetInput(image);
        localStorage->m_Reslicer->SetWorldGeometry(worldGeometry);
        localStorage->m_Reslicer->SetTimeStep(this->GetTimestep());

        // set the transformation of the image to adapt reslice axis
        localStorage->m_Reslicer->SetResliceTransformByGeometry(
            image->GetTimeGeometry()->GetGeometryForTimeStep(this->GetTimestep()));

        // is the geometry of the slice based on the input image or the worldgeometry?
        bool inPlaneResampleExtentByGeometry = false;
        datanode->GetBoolProperty("in plane resample extent by geometry", inPlaneResampleExtentByGeometry, renderer);
        localStorage->m_Reslicer->SetInPlaneResampleExtentByGeometry(inPlaneResampleExtentByGeometry);

        // Initialize the interpolation mode for resampling; switch to nearest
        // neighbor if the input image is too small.
        if ((image->GetDimension() >= 3) && (image->GetDimension(2) > 1))
        {
            VtkResliceInterpolationProperty* resliceInterpolationProperty;
            datanode->GetProperty(resliceInterpolationProperty, "reslice interpolation", renderer);

            int interpolationMode = VTK_RESLICE_NEAREST;
            if (resliceInterpolationProperty != nullptr)
            {
                interpolationMode = resliceInterpolationProperty->GetInterpolation();
            }

            switch (interpolationMode)
            {
                case VTK_RESLICE_NEAREST:
                    localStorage->m_Reslicer->SetInterpolationMode(ExtractSliceFilter::RESLICE_NEAREST);
                    break;
                case VTK_RESLICE_LINEAR:
                    localStorage->m_Reslicer->SetInterpolationMode(ExtractSliceFilter::RESLICE_LINEAR);
                    break;
                case VTK_RESLICE_CUBIC:
                    localStorage->m_Reslicer->SetInterpolationMode(ExtractSliceFilter::RESLICE_CUBIC);
                    break;
            }
        }
        else
        {
            localStorage->m_Reslicer->SetInterpolationMode(ExtractSliceFilter::RESLICE_NEAREST);
        }

        // set the vtk output property to true, makes sure that no unneeded mitk image convertion
        // is done.
        localStorage->m_Reslicer->SetVtkOutputRequest(true);

        // Thickslicing
        int thickSlicesMode = 0;
        int thickSlicesNum = 1;
        // Thick slices parameters
        if (image->GetPixelType().GetNumberOfComponents() == 1)  // for now only single component are allowed
        {
            DataNode* dn = renderer->GetCurrentWorldPlaneGeometryNode();
            if (dn)
            {
                ResliceMethodProperty* resliceMethodEnumProperty = nullptr;

                if (dn->GetProperty(resliceMethodEnumProperty, "reslice.thickslices", renderer) &&
                    resliceMethodEnumProperty)
                    thickSlicesMode = resliceMethodEnumProperty->GetValueAsId();

                IntProperty* intProperty = nullptr;
                if (dn->GetProperty(intProperty, "reslice.thickslices.num", renderer) && intProperty)
                {
                    thickSlicesNum = intProperty->GetValue();
                    if (thickSlicesNum < 1)
                        thickSlicesNum = 1;
                }
            }
            else
            {
                MITK_WARN << "no associated widget plane data tree node found";
            }
        }

        const auto* planeGeometry = dynamic_cast<const PlaneGeometry*>(worldGeometry);

        if (thickSlicesMode > 0)
        {
            double dataZSpacing = 1.0;

            Vector3D normInIndex, normal;

            const auto* abstractGeometry = dynamic_cast<const AbstractTransformGeometry*>(worldGeometry);
            if (abstractGeometry != nullptr)
                normal = abstractGeometry->GetPlane()->GetNormal();
            else
            {
                if (planeGeometry != nullptr)
                {
                    normal = planeGeometry->GetNormal();
                }
                else
                    return;  // no fitting geometry set
            }
            normal.Normalize();

            image->GetTimeGeometry()->GetGeometryForTimeStep(this->GetTimestep())->WorldToIndex(normal, normInIndex);

            dataZSpacing = 1.0 / normInIndex.GetNorm();

            localStorage->m_Reslicer->SetOutputDimensionality(3);
            localStorage->m_Reslicer->SetOutputSpacingZDirection(dataZSpacing);
            localStorage->m_Reslicer->SetOutputExtentZDirection(-thickSlicesNum, 0 + thickSlicesNum);

            // Do the reslicing. Modified() is called to make sure that the reslicer is
            // executed even though the input geometry information did not change; this
            // is necessary when the input /em data, but not the /em geometry changes.
            localStorage->m_TSFilter->SetThickSliceMode(thickSlicesMode - 1);
            localStorage->m_TSFilter->SetInputData(localStorage->m_Reslicer->GetVtkOutput());

            // vtkFilter=>mitkFilter=>vtkFilter update mechanism will fail without calling manually
            localStorage->m_Reslicer->Modified();
            localStorage->m_Reslicer->Update();

            localStorage->m_TSFilter->Modified();
            localStorage->m_TSFilter->Update();
            localStorage->m_ReslicedImage = localStorage->m_TSFilter->GetOutput();
        }
        else
        {
            // this is needed when thick mode was enable bevore. These variable have to be reset to default values
            localStorage->m_Reslicer->SetOutputDimensionality(2);
            localStorage->m_Reslicer->SetOutputSpacingZDirection(1.0);
            localStorage->m_Reslicer->SetOutputExtentZDirection(0, 0);

            localStorage->m_Reslicer->Modified();
            // start the pipeline with updating the largest possible, needed if the geometry of the input has changed
            localStorage->m_Reslicer->UpdateLargestPossibleRegion();
            localStorage->m_ReslicedImage = localStorage->m_Reslicer->GetVtkOutput();
        }

        // Bounds information for reslicing (only reuqired if reference geometry
        // is present)
        // this used for generating a vtkPLaneSource with the right size
        double sliceBounds[6];
        for (auto& sliceBound : sliceBounds)
        {
            sliceBound = 0.0;
        }
        localStorage->m_Reslicer->GetClippedPlaneBounds(sliceBounds);

        // get the spacing of the slice
        localStorage->m_mmPerPixel = localStorage->m_Reslicer->GetOutputSpacing();

        // calculate minimum bounding rect of IMAGE in texture
        {
            double textureClippingBounds[6];
            for (auto& textureClippingBound : textureClippingBounds)
            {
                textureClippingBound = 0.0;
            }
            // Calculate the actual bounds of the transformed plane clipped by the
            // dataset bounding box; this is required for drawing the texture at the
            // correct position during 3D mapping.
            mitk::PlaneClipping::CalculateClippedPlaneBounds(
                image->GetGeometry(), planeGeometry, textureClippingBounds);

            textureClippingBounds[0] = static_cast<int>(textureClippingBounds[0] / localStorage->m_mmPerPixel[0] + 0.5);
            textureClippingBounds[1] = static_cast<int>(textureClippingBounds[1] / localStorage->m_mmPerPixel[0] + 0.5);
            textureClippingBounds[2] = static_cast<int>(textureClippingBounds[2] / localStorage->m_mmPerPixel[1] + 0.5);
            textureClippingBounds[3] = static_cast<int>(textureClippingBounds[3] / localStorage->m_mmPerPixel[1] + 0.5);

            // clipping bounds for cutting the image
            localStorage->m_LevelWindowFilter->SetClippingBounds(textureClippingBounds);
        }
    }
    //     ImageVtkMapper2D2() = default;
};
}  // namespace mitk

#endif  //! MITK_IMAGE_VTK_MAPPER_2D_2_H