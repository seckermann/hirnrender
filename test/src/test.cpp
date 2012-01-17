//vtk
#include <vtkInteractorStyleTrackballCamera.h>
#include "vtkImageData.h"
#include "vtkVolumeProperty.h"
#include "vtkRenderer.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>

#include <iostream>

#include <DataStorage/io_application.hpp>
#include "DataStorage/image.hpp"

#include "CoreUtils/log.hpp"

//external includes
#include<boost/shared_ptr.hpp>


using namespace isis;

vtkImageData* getImageData(data::Image image);




int main(int argc, char **argv) {

  data::IOApplication app( "vtkadapter", true, false );
  app.init( argc, argv, true); // if there is a problem, we just get no images and exit cleanly
  // get the first image from the loaded input data
  std::list<data::Image> images = app.images;


  std::list<vtkImageData*> ids;

  for (std::list<data::Image>::const_iterator image = images.begin(); image != images.end(); ++image){
    ids.push_back(getImageData(*image));
  }




  vtkRenderer* renderer = vtkRenderer::New();
  vtkVolumeProperty *propertyBrain;
  vtkSmartVolumeMapper* mapper;
  vtkColorTransferFunction* colorFun;  
  vtkPiecewiseFunction* opacityFun;
  
//bild 1
  vtkImageData* id = ids.front();
  vtkVolume * volume = vtkVolume::New();

  mapper = vtkSmartVolumeMapper::New();
  mapper->SetInput(id);
  propertyBrain = vtkVolumeProperty::New();

  colorFun =vtkColorTransferFunction::New();
  propertyBrain->SetColor( colorFun );
  colorFun->AddRGBPoint(	 10, .0, .0, .0);
  colorFun->AddRGBPoint(	 50, .2, .2, .2);
  colorFun->AddRGBPoint(	 100, .4, .4, .4);
  colorFun->AddRGBPoint(	200, .6,.6,.6);
  colorFun->AddRGBPoint(	300, .8,.8,.8);
  colorFun->AddRGBPoint(	 0, 0, 0, 0);

  opacityFun = vtkPiecewiseFunction::New();
  propertyBrain->SetScalarOpacity( opacityFun );
  opacityFun->AddPoint(0,0.00);
  opacityFun->AddPoint(90,1);	
  propertyBrain->SetIndependentComponents(true);

  propertyBrain->SetInterpolationTypeToLinear();
  //propertyBrain->SetInterpolationTypeToNearest();
  //mapper->SetInterpolationModeToCubic();
  volume->SetProperty( propertyBrain );
  volume->SetMapper(mapper);
  renderer->AddVolume(volume);
  double* bounds = volume->GetBounds();
  for(int i= 0 ; i<6 ; i++ ){
  printf("%f\n ", bounds[i]);
  }
  mapper->SetCroppingRegionPlanes(bounds[0],(bounds[1]-bounds[0])/2 + bounds[0], bounds[2],bounds[3],bounds[4],bounds[5]);
  mapper->CroppingOn();

/*bild2**************************/
  id = ids.back();
  volume = vtkVolume::New();

  mapper = vtkSmartVolumeMapper::New();
  mapper->SetInput(id);
  propertyBrain = vtkVolumeProperty::New();

  colorFun =vtkColorTransferFunction::New();
  propertyBrain->SetColor( colorFun );
  colorFun->AddRGBPoint(	 0, .0, .0, .0);
  colorFun->AddRGBPoint(	 4, 1,.0 ,.0 );
  colorFun->AddRGBPoint(	 -4, 0, 0, 1);

  opacityFun = vtkPiecewiseFunction::New();
  propertyBrain->SetScalarOpacity( opacityFun );
  opacityFun->AddPoint(0,0.00);
  opacityFun->AddPoint(0.1,1);	
  propertyBrain->SetIndependentComponents(true);

  propertyBrain->SetInterpolationTypeToLinear();
  //propertyBrain->SetInterpolationTypeToNearest();
  //mapper->SetInterpolationModeToCubic();
  volume->SetProperty( propertyBrain );
  volume->SetMapper(mapper);
  renderer->AddVolume(volume);

  bounds = volume->GetBounds();
  
  mapper->SetCroppingRegionPlanes(bounds[0],(bounds[1]-bounds[0])/2 + bounds[0], bounds[2],bounds[3],bounds[4],bounds[5]);
  mapper->CroppingOn();




/**************************/




  renderer->SetBackground(0.1, 0.2, 0.4);
  renderer->ResetCamera();


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





vtkImageData* getImageData(data::Image image){
  vtkImageData* id;
  int dims[3];
  int size;
  double* mem;
  int counter=0;
  dims[0]=image.getNrOfColumns();
  dims[1]=image.getNrOfRows();
  dims[2]=image.getNrOfSlices();
  size = dims[0]*dims[1]*dims[2];
  mem = new double[size];
  image.copyToMem(mem,size);  

  id = vtkImageData::New();
  id->SetDimensions(dims);
  const util::fvector4 spacing = image.getPropertyAs<util::fvector4>( "voxelSize" );
  id->SetSpacing(spacing[0],spacing[1],spacing[2]);
  const util::fvector4 origin = image.getPropertyAs<util::fvector4>( "indexOrigin" );
  id->SetOrigin(origin[0]+spacing[0]/2.0,origin[1]-spacing[1]/2.0,origin[2]-spacing[2]/2.0);

  for (int z=0; z<dims[2]; z++){
    for (int y=0; y<dims[1]; y++){
      for (int x=0; x<dims[0]; x++){
        id->SetScalarComponentFromDouble(x,y,z,0,mem[counter]);
        counter++;
      }
    }
  }
  return id;
}

