from vtk.qt.QVTKRenderWindowInteractor import QVTKRenderWindowInteractor
import vtk
from RenderingType import Technique
from constants import *

iren = QVTKRenderWindowInteractor()
surfaceExtractor = vtk.vtkContourFilter()
volumeColor = vtk.vtkColorTransferFunction()
volumeScalarOpacity = vtk.vtkPiecewiseFunction()

def update_iso(val: int, current_rendering_technique: Technique) -> None:
    if current_rendering_technique == Technique.SurfaceRendering.value:
        surfaceExtractor.SetValue(0, val)
        iren.update()

def update_rgb(node: int, R: float, G: float, B: float) -> None:
    volumeColor.RemovePoint(node)
    volumeColor.AddRGBPoint(node, R, G, B)
    iren.update()

def update_opacity(node: int, opacity: float) -> None:
    volumeColor.RemovePoint(node)
    volumeScalarOpacity.AddPoint(node, opacity)
    iren.update()

def vtk_rendering(dataDir: str, current_rendering_technique: str) -> None:
    renWin = iren.GetRenderWindow()
    aRenderer = vtk.vtkRenderer()
    renWin.AddRenderer(aRenderer)
    

    reader = vtk.vtkDICOMImageReader()
    reader.SetDirectoryName(dataDir)
    reader.Update()

    if (current_rendering_technique == Technique.SurfaceRendering.value):
        # An isosurface, or contour value of 500 is known to correspond to the
        surfaceExtractor.SetInputConnection(reader.GetOutputPort())
        surfaceExtractor.SetValue(0, DEFAULT_ISO)

        surfaceNormals = vtk.vtkPolyDataNormals()
        surfaceNormals.SetInputConnection(surfaceExtractor.GetOutputPort())
        surfaceNormals.SetFeatureAngle(60.0)
        
        surfaceMapper = vtk.vtkPolyDataMapper()
        surfaceMapper.SetInputConnection(surfaceNormals.GetOutputPort())
        surfaceMapper.ScalarVisibilityOff()
        
        surface = vtk.vtkActor()
        surface.SetMapper(surfaceMapper)

        aRenderer.AddActor(surface)

    elif(current_rendering_technique == Technique.RayCastingRendering.value):
        volumeMapper = vtk.vtkGPUVolumeRayCastMapper()
        volumeMapper.SetInputConnection(reader.GetOutputPort())
        volumeMapper.SetBlendModeToComposite()
        
        volumeColor.AddRGBPoint(SKIN,    *DEFAULT_SKIN)
        volumeColor.AddRGBPoint(MUSCLE,  *DEFAULT_MUSCLE)
        volumeColor.AddRGBPoint(BONES, *DEFAULT_BONES)
        volumeColor.AddRGBPoint(DEEP_BONES, *DEFAULT_DEEP_BONES)
        
        volumeScalarOpacity.AddPoint(SKIN,    DEFAULT_SKIN_OPACITY)
        volumeScalarOpacity.AddPoint(MUSCLE,  DEFAULT_MUSCLE_OPACITY)
        volumeScalarOpacity.AddPoint(BONES, DEFAULT_BONES_OPACITY)
        volumeScalarOpacity.AddPoint(DEEP_BONES, DEFAULT_DEEP_BONES_OPACITY)

        volumeGradientOpacity = vtk.vtkPiecewiseFunction()
        volumeGradientOpacity.AddPoint(0,   0.0)
        volumeGradientOpacity.AddPoint(90,  0.5)
        volumeGradientOpacity.AddPoint(100, 1.0)

        volumeProperty = vtk.vtkVolumeProperty()
        volumeProperty.SetColor(volumeColor)
        volumeProperty.SetScalarOpacity(volumeScalarOpacity)
        volumeProperty.SetGradientOpacity(volumeGradientOpacity)
        volumeProperty.SetInterpolationTypeToLinear()
        volumeProperty.ShadeOn()
        volumeProperty.SetAmbient(0.4)
        volumeProperty.SetDiffuse(0.6)
        volumeProperty.SetSpecular(0.2)

        volume = vtk.vtkVolume()
        volume.SetMapper(volumeMapper)
        volume.SetProperty(volumeProperty)

        aRenderer.AddViewProp(volume)
    
    aCamera = vtk.vtkCamera()
    aCamera.SetViewUp(0, 0, -1)
    aCamera.SetPosition(0, 1, 0)
    aCamera.SetFocalPoint(0, 0, 0)
    aCamera.ComputeViewPlaneNormal()
    
    # aRenderer.AddActor(surface)
    aRenderer.SetActiveCamera(aCamera)
    aRenderer.ResetCamera()
    
    aRenderer.SetBackground(0, 0, 0)
    
    aRenderer.ResetCameraClippingRange()
    
    # Interact with the data.
    iren.Initialize()
    renWin.Render()
    iren.Start()
    iren.show()