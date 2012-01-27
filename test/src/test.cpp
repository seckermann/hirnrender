//vtk
#include <vtkInteractorStyleTrackballCamera.h>
#include "vtkImageData.h"
#include "vtkVolumeProperty.h"
#include "vtkRenderer.h"
#include "vtkOpenGLRenderer.h"
#include "vtkSmartVolumeMapper.h"
#include <vtkFixedPointVolumeRayCastMapper.h>
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>

#include <iostream>

#include <DataStorage/io_factory.hpp>
#include "DataStorage/image.hpp"

#include "CoreUtils/log.hpp"

//external includes
#include<boost/shared_ptr.hpp>


using namespace isis;

vtkImageData* getImageData(data::Image image);
void renderImage(char* image, char *activity);




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
  renderImage(image, activity); 

}


void renderImage(char* image, char *activity){
  std::list<vtkImageData*> ids;
  std::list<vtkImageData*> ads;
  vtkImageData* ad;
  vtkImageData* id;
  //Image
  std::list<data::Image> images = data::IOFactory::load(image);
  for (std::list<data::Image>::const_iterator image = images.begin(); image != images.end(); ++image){
    ids.push_back(getImageData(*image));
  }
  id = ids.front();


  //Activity
  if(activity!= NULL){
    std::list<data::Image> activities = data::IOFactory::load(activity);
    for (std::list<data::Image>::const_iterator activity = activities.begin(); activity != activities.end(); ++activity){
      ads.push_back(getImageData(*activity));
    }
    ad = ads.front();
  }



  vtkRenderer* renderer1 = vtkOpenGLRenderer::New();
//  vtkRenderer* renderer2 = vtkOpenGLRenderer::New();
  vtkVolumeProperty *propertyBrain;
  vtkFixedPointVolumeRayCastMapper* mapper;
  vtkColorTransferFunction* colorFun;  
  vtkPiecewiseFunction* opacityFun;

  vtkVolume * volume = vtkVolume::New();
  double* bounds;
  //bild 1

  mapper = vtkFixedPointVolumeRayCastMapper::New();
  mapper->SetInput(id);
 
  propertyBrain = vtkVolumeProperty::New();

  colorFun =vtkColorTransferFunction::New();
  //propertyBrain->SetColor( colorFun );
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
  
  propertyBrain->DisableGradientOpacityOn();
  //propertyBrain->SetIndependentComponents(true);


  propertyBrain->SetInterpolationTypeToLinear();
  bounds = volume->GetBounds();
  double* cropping = new double[6];
  cropping[0] = bounds[0];
  cropping[1] = (bounds[1]-bounds[0])/2+bounds[0];
  cropping[2] = bounds[2];
  cropping[3] = bounds[3];
  cropping[4] = bounds[4];
  cropping[5] = bounds[5];
  mapper->SetCroppingRegionPlanes(cropping[0],cropping[1], cropping[2],cropping[3],cropping[4],cropping[5]);
  //mapper->CroppingOn();
  
  volume->SetProperty( propertyBrain );
  volume->SetMapper(mapper);
  renderer1->AddVolume(volume);
  //Activity
  if(activity!=NULL){

    double max = -100;
    double min = +100;
    for (int z=0; z<ad->GetDimensions()[2]; z++){
      for (int y=0; y<ad->GetDimensions()[1]; y++){
        for (int x=0; x<ad->GetDimensions()[0]; x++){
          double temp = ad->GetScalarComponentAsDouble(x,y,z,0);
          if(temp > max){
            max = temp;
          }
          if(temp < min){
            min = temp;
          }
        }
      }
    }
    volume = vtkVolume::New();

    mapper = vtkFixedPointVolumeRayCastMapper::New();
    mapper->SetInput(ad);
    propertyBrain = vtkVolumeProperty::New();

    colorFun =vtkColorTransferFunction::New();
    propertyBrain->SetColor( colorFun );
    colorFun->AddRGBPoint(	 -4,   0,   1, 1);
    colorFun->AddRGBPoint(	 -0.2, 0,   0, 1  );
    colorFun->AddRGBPoint(	  0,   0,   0, 0  );
    colorFun->AddRGBPoint(	  0.2, 1,   0, 0  );
    colorFun->AddRGBPoint(	  4,   1, 1, 0  );

    opacityFun = vtkPiecewiseFunction::New();
    propertyBrain->SetScalarOpacity( opacityFun );

    opacityFun->AddPoint(-4,   1);	
    opacityFun->AddPoint(-0.2 , 1);	
    opacityFun->AddPoint(-0.1, 0);
    opacityFun->AddPoint( 0,   0);
    opacityFun->AddPoint( 0.1, 0);
    opacityFun->AddPoint( 0.2, 1);	
    opacityFun->AddPoint( 4, 1);	

    propertyBrain->SetIndependentComponents(true);

  propertyBrain->DisableGradientOpacityOn();
    //propertyBrain->SetInterpolationTypeToLinear();
    bounds = volume->GetBounds();

    mapper->SetCroppingRegionPlanes(cropping[0],cropping[1], cropping[2],cropping[3],cropping[4],cropping[5]);
  //  mapper->CroppingOn();

    volume->SetProperty( propertyBrain );
    volume->SetMapper(mapper);

    renderer1->AddVolume(volume);


  }



  renderer1->SetBackground(0.1, 0.2, 0.4);
  renderer1->ResetCamera();
	

  renderer1->InteractiveOn();
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  renWin->AddRenderer(renderer1);
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
  id->SetOrigin(origin[0]/*-spacing[0]/2.0*/,origin[1]/*-spacing[1]/2.0*/,origin[2]/*-spacing[2]/2.0*/);

  id->SetOrigin(origin[0]-spacing[0]/2.0,origin[1]-spacing[1]/2.0,origin[2]-spacing[2]/2.0);
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

