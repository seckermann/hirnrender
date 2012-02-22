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

using namespace isis;

vtkRenderWindow* renderImage(char* image, char *activity);

int main(int argc, char **argv) {
	char * image = NULL;
	char * activity = NULL;
	switch(argc){
		case 3:
			if (strcmp(argv[1],"-in")==0){
				image= argv[2];
				activity = NULL;
			}else{
				perror("Wrong parameters");
				exit(-1);
			}
			break;
		case 5:
			if (strcmp(argv[1],"-in")==0 && strcmp(argv[3],"-z")==0){
				image= argv[2];
				activity = argv[4];
			}else{
				if (strcmp(argv[1],"-z")==0 && strcmp(argv[3],"-in")==0){
					activity = argv[2];
					image= argv[4];
				}else{
					perror("Wrong parameters");
					exit(-1);
				}
			}
			break;
		default:
			perror("Wrong parameters");
			exit(-1);
			break;

	}

	QVTKWidget *widget = new QVTKWidget();
	widget->SetRenderWindow( renderImage(image, activity) );
   	widget->show();	

}



vtkRenderWindow* renderImage(char* image, char *activity){
	Volume* iad = NULL;

	if(activity==NULL){
		std::list<isis::data::Image> images = isis::data::IOFactory::load(image);
		iad = new Volume(&images.front());

	}else{
		std::list<isis::data::Image> images = isis::data::IOFactory::load(image);
		std::list<isis::data::Image> activities = isis::data::IOFactory::load(activity);
		iad = new Volume(&images.front(),&activities.front());
	}

	double* bounds= iad->getBounds();

	vtkRenderer* renderer = vtkRenderer::New();
	vtkVolumeProperty *propertyBrain;
	vtkFixedPointVolumeRayCastMapper* mapper;
	vtkColorTransferFunction* colorFun1;  
	vtkColorTransferFunction* colorFun2;  
	vtkPiecewiseFunction* opacityFun1;
	vtkPiecewiseFunction* opacityFun2;

	vtkVolume * volume = vtkVolume::New();
	//bild 1


	mapper = vtkFixedPointVolumeRayCastMapper::New();
	//mapper->SetImageSampleDistance(0.5);
	//mapper->SetSampleDistance(0.1);
	mapper->SetImageSampleDistance(1.5);
	mapper->SetSampleDistance(0.3);
	mapper->SetInput(iad->GetOutput());
	propertyBrain = vtkVolumeProperty::New();




	colorFun1 =vtkColorTransferFunction::New();
	colorFun1->AddRGBPoint( 10,	0.0, 	0.0, 	0.0);
	colorFun1->AddRGBPoint( 50,	0.2, 	0.2, 	0.2);
	colorFun1->AddRGBPoint(100,	0.4,	0.4, 	0.4);
	colorFun1->AddRGBPoint(200,	0.6,	0.6,	0.6);
	colorFun1->AddRGBPoint(300,	0.8,	0.8,	0.8);
	colorFun1->AddRGBPoint(  0,	0,	0, 	0);
	propertyBrain->SetColor(0, colorFun1 );
	opacityFun1 = vtkPiecewiseFunction::New();
	opacityFun1->AddPoint( 0, 0);
	opacityFun1->AddPoint(90, 1);	
	propertyBrain->SetScalarOpacity(0, opacityFun1 );



	colorFun2 =vtkColorTransferFunction::New();
	colorFun2->AddRGBPoint(-4,	 1, 	1, 	1);
	colorFun2->AddRGBPoint(-0.2,	 0, 	0, 	1);
	colorFun2->AddRGBPoint(-0.19,	 0, 	0, 	0);
	colorFun2->AddRGBPoint( 0,	 0,	0, 	0);
	colorFun2->AddRGBPoint( 0.19,	 0, 	0, 	0);
	colorFun2->AddRGBPoint( 0.2,	 1, 	0, 	0);
	colorFun2->AddRGBPoint( 4, 	 1, 	1, 	1);
	propertyBrain->SetColor(1,colorFun2 );
	opacityFun2 = vtkPiecewiseFunction::New();
	opacityFun2->AddPoint(-4, 	1);	
	opacityFun2->AddPoint(-0.2,	1);	
	opacityFun2->AddPoint(-0.19,	0);	
	opacityFun2->AddPoint( 0,	0);	
	opacityFun2->AddPoint( 0.19,	0);	
	opacityFun2->AddPoint( 0.2,	1);	
	propertyBrain->SetScalarOpacity(1,opacityFun2 );



	propertyBrain->SetInterpolationTypeToLinear();
	volume->SetProperty( propertyBrain );
	volume->SetMapper(mapper);
	renderer->AddVolume(volume);


//  vtkPlane* plane1 =vtkPlane::New(); 
//  plane1->SetNormal(1,0,0);
// plane1->SetOrigin(bounds[0],0,0);
//  mapper->AddClippingPlane(plane1);
	mapper->CroppingOn();
	mapper->SetCroppingRegionFlagsToFence();
	mapper->SetCroppingRegionPlanes(bounds[0],bounds[1]/2,bounds[2],bounds[3]/2,bounds[4],bounds[5]/2);
	


	cout<<"Cropping an?  "<<mapper->GetCropping()<<endl;
	double* plane = mapper->GetCroppingRegionPlanes();
	cout<<"CroppingPlanes?  "<<plane[0]<<" "<<plane[1]<<" "<<plane[2]<<" "<<plane[3]<<" "<<plane[4]<<" "<<plane[5]<<" "<<endl;
	cout<<"CroppingFlags?  "<<mapper->GetCroppingRegionFlags()<<endl;

	renderer->SetBackground(0.1, 0.2, 0.4);
	renderer->ResetCamera();
	pos position = h;
	
	setCamera(renderer->GetActiveCamera(),position); 

	vtkRenderWindow* renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);
	renWin->SetSize(800, 600);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();

	iren->SetInteractorStyle(style);




	vtkSmartPointer<vtkSliderWidget> opacitySliderWidget;
	vtkSmartPointer<vtkSliderWidget> blueSliderWidget;
	vtkSmartPointer<vtkSliderWidget> redSliderWidget;
	vtkSliderCallback* redCallback;
	vtkSliderCallback* opacityCallback;
	vtkSliderCallback* blueCallback;


	if(iad->hasActivity()){
		// Here we describe the representation of the widget.
		vtkSmartPointer<vtkSliderRepresentation2D> opacityRep = vtkSmartPointer<vtkSliderRepresentation2D>::New();
		opacityRep->SetMinimumValue(0.0);
		opacityRep->SetMaximumValue(1.0);
		opacityRep->SetValue(1);
		opacityRep->SetTitleText("Opacity");

		vtkSmartPointer<vtkSliderRepresentation2D> redRep = vtkSmartPointer<vtkSliderRepresentation2D>::New();
		redRep->SetMinimumValue(0.2);
		redRep->SetMaximumValue(iad->getRange()[1]);
		redRep->SetValue(0.2);
		redRep->SetTitleText("red");

		vtkSmartPointer<vtkSliderRepresentation2D> blueRep = vtkSmartPointer<vtkSliderRepresentation2D>::New();
		blueRep->SetMinimumValue(iad->getRange()[0]);
		blueRep->SetMaximumValue(-0.2);
		blueRep->SetValue(-0.2);
		blueRep->SetTitleText("blue");


		// Here we use normalized display coordinates (0,1) so that the
		// slider will stay in the same proportionate location if the window
		// is resized.

		vtkSliderCallback::range = iad->getRange();
		// Opacity
		opacityRep->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
		opacityRep->GetPoint1Coordinate()->SetValue(0.7 ,1);
		opacityRep->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
		opacityRep->GetPoint2Coordinate()->SetValue(0.7, .1);

		opacityCallback = new vtkSliderCallback();
		opacityCallback->OpacityFun = opacityFun2;
		opacityCallback->opacity = 1;
		opacityCallback->option = 1;

		opacitySliderWidget = vtkSmartPointer<vtkSliderWidget>::New();
		opacitySliderWidget->SetInteractor(iren);
		opacitySliderWidget->SetRepresentation(opacityRep);
		opacitySliderWidget->SetAnimationModeToAnimate();
		opacitySliderWidget->EnabledOn();

		//red
		redRep->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
		redRep->GetPoint1Coordinate()->SetValue(0.8 ,1);
		redRep->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
		redRep->GetPoint2Coordinate()->SetValue(0.8, .1);

		redCallback = new vtkSliderCallback();
		redCallback->ColorFun = colorFun2;
		redCallback->red = 0.2;
		redCallback->option = 2;

		redSliderWidget = vtkSmartPointer<vtkSliderWidget>::New();
		redSliderWidget->SetInteractor(iren);
		redSliderWidget->SetRepresentation(redRep);
		redSliderWidget->SetAnimationModeToAnimate();
		redSliderWidget->EnabledOn();

		//blue
		blueRep->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
		blueRep->GetPoint1Coordinate()->SetValue(0.9 ,1);
		blueRep->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
		blueRep->GetPoint2Coordinate()->SetValue(0.9, .1);

		blueCallback = new vtkSliderCallback();
		blueCallback->ColorFun = colorFun2;
		blueCallback->blue = -0.2;
		blueCallback->option = 3;

		blueSliderWidget = vtkSmartPointer<vtkSliderWidget>::New();
		blueSliderWidget->SetInteractor(iren);
		blueSliderWidget->SetRepresentation(blueRep);
		blueSliderWidget->SetAnimationModeToAnimate();
		blueSliderWidget->EnabledOn();

	}

	//clipping1
	//
	vtkSmartPointer<vtkSliderRepresentation2D> c1Rep = vtkSmartPointer<vtkSliderRepresentation2D>::New();
	c1Rep->SetMinimumValue(bounds[0]);
	c1Rep->SetMaximumValue(bounds[1]);
	c1Rep->SetValue(bounds[0]);
	c1Rep->SetTitleText("c1");


	c1Rep->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
	c1Rep->GetPoint1Coordinate()->SetValue(0.6 ,1);
	c1Rep->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
	c1Rep->GetPoint2Coordinate()->SetValue(0.6, .1);

	vtkSliderCallback* c1Callback = new vtkSliderCallback();
  c1Callback->mapper = mapper;
	c1Callback->option = 4;

	vtkSmartPointer<vtkSliderWidget> c1SliderWidget = vtkSmartPointer<vtkSliderWidget>::New();
	c1SliderWidget->SetInteractor(iren);
	c1SliderWidget->SetRepresentation(c1Rep);
	c1SliderWidget->SetAnimationModeToAnimate();
	c1SliderWidget->EnabledOn();




	// Observe the interaction events of the widget. If the computation
	// in the callback is time consuming, observe the
	// EndInteractionEvent instead.
	if(iad->hasActivity()){

		opacitySliderWidget->AddObserver(vtkCommand::InteractionEvent,opacityCallback);

		redSliderWidget->AddObserver(vtkCommand::InteractionEvent,redCallback);


		blueSliderWidget->AddObserver(vtkCommand::InteractionEvent,blueCallback);
	}
	c1SliderWidget->AddObserver(vtkCommand::InteractionEvent,c1Callback);
	return renWin;
}

