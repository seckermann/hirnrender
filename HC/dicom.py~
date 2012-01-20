#!/usr/bin/env python
# -*- coding: utf-8 -*-

import vtk


opacityLevel = 2048
opacityWindow = 4096

#DICOM image Reader
dicomReader = vtk.vtkDICOMImageReader()
dicomReader.SetDirectoryName("data/S3")
dicomReader.Update()
inputBrain = dicomReader.GetOutput()
reader = dicomReader


#Volume & Mapper
volume = vtk.vtkVolume()

mapper = vtk.vtkSmartVolumeMapper()

#mapper verbinden
mapper.SetInputConnection(reader.GetOutputPort())

mapper.CroppingOn()
#SetCropppingRegionPlanes((xmin,xmax,ymin,ymax,zmin,zmax)), wobei x == breite, y == höhe, z == tiefe von der entfernten, unteren, linken Ecke aus.
# 0.0,0.0,0.0 ist also die entfernte linke untere Ecke. positive x-werte gehen nach rechts, positve y-Werte nach oben und positive z-Werte auf den Betrachter zu.
#mapper.SetCroppingRegionPlanes((0.0, 150.0, 0.0, 150.0, 0.0, 300.0))

colorFun = vtk.vtkColorTransferFunction()
opacityFun = vtk.vtkPiecewiseFunction()
  
# Create the property and attach the transfer functions
propertyBrain = vtk.vtkVolumeProperty()
propertyBrain.SetIndependentComponents(True)
propertyBrain.SetInterpolationTypeToLinear()
propertyBrain.SetAmbient(0.1);
propertyBrain.SetDiffuse(0.9);
propertyBrain.SetSpecular(0.2);
propertyBrain.SetSpecularPower(10.0);
propertyBrain.SetScalarOpacityUnitDistance(0.8919);

#propertyBrain.SetColor( colorFun )
propertyBrain.SetScalarOpacity( opacityFun )



#connect up the volume to the property and the mapper
volume.SetProperty( propertyBrain )
volume.SetMapper(mapper)

#colorFun.AddRGBSegment(opacityLevel - 0.5*opacityWindow, 0.0,0.0,0.0,opacityLevel + 0.5*opacityWindow, 1.0, 1.0, 1.0)
#opacityFun.AddSegment(opacityLevel - 0.5*opacityWindow, 0.0, opacityLevel + 0.5*opacityWindow, 1.0)



#opacityFun.AddPoint(-3024, 0, 0.5, 0.0 );
#opacityFun.AddPoint(-1000, 0, 0.5, 0.0 );
opacityFun.AddPoint(0.0,0.0)
opacityFun.AddPoint(149.9,0.0)
opacityFun.AddPoint(150.0,1.0,0.5,0.5)
#opacityFun.AddPoint(1000,1.0)
#opacityFun.AddPoint(-500, 1.0, 0.33, 0.45 );
#opacityFun.AddPoint(3071, 1.0, 0.5, 0.0);
print opacityFun.GetRange()
print colorFun.GetRange()
 
mapper.SetBlendModeToComposite()


#renderer
renderer = vtk.vtkRenderer()
renderer.AddVolume(volume)
renderer.SetBackground(0.1, 0.2, 0.4)

renderer.ResetCamera()


# Finally we create the render window which will show up on the screen
# We put our renderer into the render window using AddRenderer. We
# also set the size to be 300 pixels by 300.
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(renderer)
renWin.SetSize(800, 600)

# The vtkRenderWindowInteractor class watches for events (e.g., keypress,
# mouse) in the vtkRenderWindow. These events are translated into
# event invocations that VTK understands (see VTK/Common/vtkCommand.h
# for all events that VTK processes). Then observers of these VTK
# events can process them as appropriate.
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

# By default the vtkRenderWindowInteractor instantiates an instance
# of vtkInteractorStyle. vtkInteractorStyle translates a set of events
# it observes into operations on the camera, actors, and/or properties
# in the vtkRenderWindow associated with the vtkRenderWinodwInteractor. 
# Here we specify a particular interactor style.
style = vtk.vtkInteractorStyleTrackballCamera()
iren.SetInteractorStyle(style)

# Here we use a vtkBoxWidget to transform the underlying coneActor (by
# manipulating its transformation matrix). Many other types of widgets
# are available for use, see the documentation for more details.
#
# The SetInteractor method is how 3D widgets are associated with the render
# window interactor. Internally, SetInteractor sets up a bunch of callbacks
# using the Command/Observer mechanism (AddObserver()). The place factor 
# controls the initial size of the widget with respect to the bounding box
# of the input to the widget.
#boxWidget = vtk.vtkBoxWidget()
#boxWidget.SetInteractor(iren)
#boxWidget.SetPlaceFactor(1.25)

#CropWidget
cropWidget = vtk.vtkImageCroppingRegionsWidget()
cropWidget.SetInteractor(iren)
cropWidget.SetVolumeMapper(mapper)
cropWidget.SetProp3D(volume)
pos = volume.GetBounds()
cropWidget.SetCroppingRegionFlags(0x2ebfeba)
bounds = volume.GetBounds()
cropWidget.SetPlanePositions(bounds[0],bounds[1]/2,bounds[2],bounds[3]/2,bounds[4],bounds[5]/2)
mapper.SetCroppingRegionFlags(0x2ebfeba)
cropWidget.SetSlice(1)








# Place the interactor initially. The input to a 3D widget is used to
# initially position and scale the widget. The EndInteractionEvent is
# observed which invokes the SelectPolygons callback.
#boxWidget.SetProp3D(volume)
#boxWidget.PlaceWidget()

# Similar to Step2/Python/Cone2.py, we define a callback for
# interaction.  As can be seen the callback takes two arguments.  The
# first being the object that generates the event and the second
# argument the event name (which is a string).
#def myCallback(widget, event_string):
#    t = vtk.vtkTransform()
#    boxWidget.GetTransform(t)
#    boxWidget.GetProp3D().SetUserTransform(t)

def myCallback(widget, event_string):
	x = cropWidget.GetSliceOrientation()
	if x == 0:
		cropWidget.SetSliceOrientation(1)
	elif x == 1:
		cropWidget.SetSliceOrientation(2)
	elif x == 2:
		cropWidget.SetSliceOrientation(0)


# Now for every interaction event that is generated by the boxWidget,
# call our callback function.
#boxWidget.AddObserver("InteractionEvent", myCallback)
iren.AddObserver("UserEvent", myCallback)

# Normally the user presses the "i" key to bring a 3D widget to
# life. Here we will manually enable it so it appears with the cone.
#boxWidget.On()
#cropWidget.SetEnabled(1)

# Start the event loop.
iren.Initialize()
iren.Start()

# There is no explicit need to free any objects at this point.
# Once Python exits, memory is automatically freed.
