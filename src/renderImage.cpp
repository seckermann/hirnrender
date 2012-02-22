#include "renderImage.h" 
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
	return renWin;
}
