#include "renderWidget.h" 
#include <vtkRenderWindow.h>


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//vtk
#include <vtkPlane.h>
#include <vtkCamera.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>
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
#include "volume.h"
#include "vtkSliderCallback.h"
/*
   void RenderWidget::paintEvent( QPaintEvent * ) {
   std::cout << "paintEvent" << std::cout << std::flush;
   }
   */
RenderWidget::RenderWidget(char* image, char *activity){
	renWin = renderImage(image, activity); 
	this->SetRenderWindow(renWin);
}

vtkRenderWindow* RenderWidget::renderImage(char* image, char *activity){
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
	this->cropping = iad->getBounds();

	this->renderer = vtkRenderer::New();

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
	this->mapper = mapper;
	setCropping();




	renderer->SetBackground(0.1, 0.2, 0.4);
	renderer->ResetCamera();
	this->cam=renderer->GetActiveCamera();
	setCameraBack(); 

	vtkRenderWindow* renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);
	return renWin;
}


void RenderWidget::setCameraRight(){
	const double rechts[] = {-600,99.5,0};
	cam->SetPosition(0,0,0);
	cam->SetPosition(rechts);
	cam->SetRoll(-90);
	this->update();
}
void RenderWidget::setCameraFront(){
	const double vorn[] = {79.5,-600,0};
	cam->SetRoll(0);
	cam->SetPosition(0,0,0);
	cam->SetPosition(vorn);
	cam->SetRoll(0);
	this->update();
}


void RenderWidget::setCameraBack(){
	const double hinten[] = {79.5,600,0};
	cam->SetPosition(0,0,0);
	cam->SetPosition(hinten);
	cam->SetRoll(180);
	this->update();
}

void RenderWidget::setCameraTop(){
	const double oben[] = {79.5,99.5,-659.58};
	cam->SetPosition(0,0,0);
	cam->SetRoll(0);

	cam->SetPosition(oben);
	cam->SetRoll(0);
	this->update();
}

void RenderWidget::setCameraLeft(){
	const double links[] = {600,99.5,0};
	cam->SetPosition(0,0,0);
	cam->SetRoll(0);
	cam->SetPosition(links);
	cam->SetRoll(90);
	this->update();
}


void RenderWidget::setCameraBottom(){
	const double unten[] = {79.5,99.5,659.58};
	cam->SetPosition(0,0,0);
	cam->SetPosition(unten);
	cam->SetRoll(180);
	this->update();
}
void RenderWidget::setCropping(){
	mapper->SetCroppingRegionPlanes(cropping);
	this->update();
}

void RenderWidget::setCroppingX(double* x){
	this->cropping[0]=x[0];
	this->cropping[1]=x[1];
	this->setCropping();
}
void RenderWidget::setCroppingY(double* y){
	this->cropping[2]=y[0];
	this->cropping[3]=y[1];
	setCropping();
}
void RenderWidget::setCroppingZ(double* z){
	this->cropping[4]=z[0];
	this->cropping[5]=z[1];
	setCropping();
}
