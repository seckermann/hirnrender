#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//vtk
#include <vtkPlane.h>
#include <vtkCamera.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageAppendComponents.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSliderRepresentation2D.h>
//isis
#include <DataStorage/io_factory.hpp>
#include <QApplication>
#include "DataStorage/image.hpp"


//slider?
#include <vtkCommand.h>
#include <vtkPolyData.h>
#include <vtkWidgetEvent.h>
#include <vtkSmartPointer.h>
#include <vtkSliderWidget.h>
#include <vtkPolyDataMapper.h>
#include <vtkCallbackCommand.h>
#include <vtkWidgetEventTranslator.h>
#include <vtkSliderRepresentation3D.h>
#include "setCamera.h"
#include "volume.h"
#include "vtkSliderCallback.h"
#include <QVTKWidget.h>

#include <vtkRenderWindow.h>


#ifndef _RENDERIMAGE_H_
#define _RENDERIMAGE_H_
vtkRenderWindow* renderImage(char* image, char *activity);
#endif
