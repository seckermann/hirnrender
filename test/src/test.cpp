#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <DataStorage/io_application.hpp>
#include <iostream>
#include <vtkPoints.h>
#include <vtkVoxel.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkImageData.h"
#include "vtkProperty.h"
#include "vtkVolumeProperty.h"
#include "vtkRenderer.h"
#include "vtkVolumeMapper.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkBoxWidget.h>


#include "DataStorage/image.hpp"
#include "CoreUtils/log.hpp"

//external includes
#include<boost/shared_ptr.hpp>


using namespace isis;
int main(int argc, char **argv) {
	int dims[3];
	vtkImageData *id;
	
	data::IOApplication app( "vtkadapter", true, false );
	// initialise the application - will automatically load the input data
	app.init( argc, argv, true); // if there is a problem, we just get no images and exit cleanly
	// get the first image from the loaded input data
	data::Image img=app.fetchImage();

        

	//load image into vtkimageData
	dims[0]=img.getNrOfColumns();
	dims[1]=img.getNrOfRows();
	dims[2]=img.getNrOfSlices();
	int size = dims[0]*dims[1]*dims[2];
	double* mem = new double[size];
	img.copyToMem(mem,size);
	id = vtkImageData::New();
	id->SetDimensions(dims);
	int counter =0;
	for (int z=0; z<dims[2]; z++){
		for (int y=0; y<dims[1]; y++){
			for (int x=0; x<dims[0]; x++){
				id->SetScalarComponentFromDouble(x,y,z,0,mem[counter]);
				counter++;
			}
		}
	}
	const util::fvector4 origin = img.getPropertyAs<util::fvector4>( "indexOrigin" );
	id->SetOrigin(origin[0],origin[1],origin[2]);
	//id->SetSpacing(0.3,0.3,0.3);
	
	vtkVolume * volume = vtkVolume::New();
	
	vtkSmartVolumeMapper* mapper = vtkSmartVolumeMapper::New();
	mapper->SetInput(id);
	vtkVolumeProperty *propertyBrain = vtkVolumeProperty::New();

	vtkColorTransferFunction* colorFun =vtkColorTransferFunction::New();
	propertyBrain->SetColor( colorFun );
	colorFun->AddRGBPoint(	 10, .0, .0, .0);
	colorFun->AddRGBPoint(	 50, .2, .2, .2);
	colorFun->AddRGBPoint(	 100, .4, .4, .4);
	colorFun->AddRGBPoint(	200, .6,.6,.6);
	colorFun->AddRGBPoint(	300, .8,.8,.8);
	colorFun->AddRGBPoint(	 0, 0, 0, 0);



	vtkPiecewiseFunction* opacityFun = vtkPiecewiseFunction::New();
	propertyBrain->SetScalarOpacity( opacityFun );
	opacityFun->AddPoint(0,0.00);
	opacityFun->AddPoint(90,1);	
	propertyBrain->SetIndependentComponents(true);



	propertyBrain->SetInterpolationTypeToLinear();
	//propertyBrain->SetInterpolationTypeToNearest();
	//mapper->SetInterpolationModeToCubic();
	volume->SetProperty( propertyBrain );
	volume->SetMapper(mapper);



	


	vtkRenderer* renderer = vtkRenderer::New();
	renderer->AddVolume(volume);
	renderer->SetBackground(0.1, 0.2, 0.4);
	renderer->ResetCamera();
	double* bounds = volume->GetBounds();
	for(int i= 0 ; i<6 ; i++ ){
		printf("%f\n ", bounds[i]);
	}
	//mapper->SetCroppingRegionPlanes (bounds[0],(bounds[1]-bounds[0])/2 + bounds[0],bounds[2],(bounds[3]-bounds[2])/2 + bounds[2],bounds[4],(bounds[5]-bounds[4])/2 + bounds[4]);
	mapper->SetCroppingRegionPlanes(bounds[0],(bounds[1]-bounds[0])/2 + bounds[0]+20, bounds[2],bounds[3],bounds[4],bounds[5]);
	//mapper->SetCroppingRegionFlagsToFence();
	mapper->CroppingOn();


	vtkRenderWindow* renWin = vtkRenderWindow::New();
	renWin->AddRenderer(renderer);
	renWin->SetSize(800, 600);

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);

	iren->Initialize();
	iren->Start(); 



}

