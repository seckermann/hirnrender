#include <iostream>
//vtk
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


class vtkSliderCallback : public vtkCommand{
  public:
  static double red;
  static double blue;
  static double opacity;
  static vtkPiecewiseFunction* OpacityFun;
  static vtkColorTransferFunction* ColorFun;
    static vtkSliderCallback *New(){
      return new vtkSliderCallback;
    }
    virtual void Execute(vtkObject *caller, unsigned long, void*){
      vtkSliderWidget *sliderWidget = reinterpret_cast<vtkSliderWidget*>(caller);
      double value = static_cast<double>(static_cast<vtkSliderRepresentation *>(sliderWidget->GetRepresentation())->GetValue());

      switch(option){
        case 1:
          this->opacity=value;

          this->OpacityFun->AddSegment(-4,value,this->blue, value);
          this->OpacityFun->AddSegment(this->blue+0.1,0,this->red-0.1, 0);
          this->OpacityFun->AddSegment(this->red,value,4, value);
          break;
        case 2:
          this->red=value;
          this->ColorFun->AddRGBSegment (0,0,0,0,4,0,0,0);
          this->ColorFun->AddRGBSegment (value-0.1,0,0,0,4,0,0,0);
          this->ColorFun->AddRGBSegment (value,1,0,0,4,1,1,1);
          this->OpacityFun->AddSegment(0,0,value-0.1,0);	
          this->OpacityFun->AddSegment(value,this->opacity,4,this->opacity);	
          break;
        case 3:
          this->blue=value;
          this->ColorFun->AddRGBSegment (-4,0,0,0,0,0,0,0);
          this->ColorFun->AddRGBSegment (-4,1,1,1,value,0,0,1);
          this->OpacityFun->AddSegment(-4,this->opacity,value,this->opacity);	
          this->OpacityFun->AddSegment(value+0.1,0,0,0);	
          break;
      }
    }
//    vtkSliderCallback():OpacityFun(0) {}
    int option;
    
};
double vtkSliderCallback::opacity=1;
double  vtkSliderCallback::red=0.2;
double  vtkSliderCallback::blue=-0.2;
vtkPiecewiseFunction* vtkSliderCallback::OpacityFun;
vtkColorTransferFunction* vtkSliderCallback::ColorFun;

using namespace isis;

vtkImageAppendComponents* getAppendComponents(vtkImageData* image, data::Image activity);
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
  vtkImageAppendComponents* iad;
  //Image
  std::list<data::Image> images = data::IOFactory::load(image);
  for (std::list<data::Image>::const_iterator image = images.begin(); image != images.end(); ++image){
    ids.push_back(getImageData(*image));
  }
  id = ids.front();

  //Activity
  if(activity!= NULL){
    std::list<data::Image> activities = data::IOFactory::load(activity);
    iad= getAppendComponents(id, activities.front());
  }else{
    iad = vtkImageAppendComponents::New();
    iad->AddInput(id);

  }



  vtkRenderer* renderer = vtkRenderer::New();
  vtkVolumeProperty *propertyBrain;
  vtkFixedPointVolumeRayCastMapper* mapper;
  vtkColorTransferFunction* colorFun1;  
  vtkColorTransferFunction* colorFun2;  
  vtkPiecewiseFunction* opacityFun1;
  vtkPiecewiseFunction* opacityFun2;

  vtkVolume * volume = vtkVolume::New();
  double* bounds;
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
  bounds = volume->GetBounds();
  double* cropping = new double[6];
  cropping[0] = bounds[0];
  cropping[1] = (bounds[1]-bounds[0])/2+bounds[0];
  cropping[2] = bounds[2];
  cropping[3] = bounds[3];
  cropping[4] = bounds[4];
  cropping[5] = bounds[5];
  mapper->SetCroppingRegionPlanes(cropping[0],cropping[1], cropping[2],cropping[3],cropping[4],cropping[5]);
  mapper->CroppingOn();


  renderer->SetBackground(0.1, 0.2, 0.4);
  renderer->ResetCamera();


  vtkRenderWindow* renWin = vtkRenderWindow::New();
  renWin->AddRenderer(renderer);
  renWin->SetSize(800, 600);

  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);
  vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
  iren->SetInteractorStyle(style);










  // Here we describe the representation of the widget.
  vtkSmartPointer<vtkSliderRepresentation2D> opacityRep = vtkSmartPointer<vtkSliderRepresentation2D>::New();
  opacityRep->SetMinimumValue(0.0);
  opacityRep->SetMaximumValue(1.0);
  opacityRep->SetValue(1);
  opacityRep->SetTitleText("Opacity");

  vtkSmartPointer<vtkSliderRepresentation2D> redRep = vtkSmartPointer<vtkSliderRepresentation2D>::New();
  redRep->SetMinimumValue(0.2);
  redRep->SetMaximumValue(4.0);
  redRep->SetValue(0.2);
  redRep->SetTitleText("red");

  vtkSmartPointer<vtkSliderRepresentation2D> blueRep = vtkSmartPointer<vtkSliderRepresentation2D>::New();
  blueRep->SetMinimumValue(-4.0);
  blueRep->SetMaximumValue(-0.2);
  blueRep->SetValue(-0.2);
  blueRep->SetTitleText("blue");


  // Here we use normalized display coordinates (0,1) so that the
  // slider will stay in the same proportionate location if the window
  // is resized.

  // Opacity
  opacityRep->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
  opacityRep->GetPoint1Coordinate()->SetValue(0.7 ,1);
  opacityRep->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
  opacityRep->GetPoint2Coordinate()->SetValue(0.7, .1);

  vtkSmartPointer<vtkSliderCallback> opacityCallback = vtkSmartPointer<vtkSliderCallback>::New();
  opacityCallback->OpacityFun = opacityFun2;
    opacityCallback->opacity = 1;
  opacityCallback->option = 1;

  vtkSmartPointer<vtkSliderWidget> opacitySliderWidget = vtkSmartPointer<vtkSliderWidget>::New();
  opacitySliderWidget->SetInteractor(iren);
  opacitySliderWidget->SetRepresentation(opacityRep);
  opacitySliderWidget->SetAnimationModeToAnimate();
  opacitySliderWidget->EnabledOn();

  //red
  redRep->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
  redRep->GetPoint1Coordinate()->SetValue(0.8 ,1);
  redRep->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
  redRep->GetPoint2Coordinate()->SetValue(0.8, .1);

  vtkSmartPointer<vtkSliderCallback> redCallback = vtkSmartPointer<vtkSliderCallback>::New();
  redCallback->ColorFun = colorFun2;
  //redCallback->red = 0.2;
  redCallback->option = 2;

  vtkSmartPointer<vtkSliderWidget> redSliderWidget = vtkSmartPointer<vtkSliderWidget>::New();
  redSliderWidget->SetInteractor(iren);
  redSliderWidget->SetRepresentation(redRep);
  redSliderWidget->SetAnimationModeToAnimate();
  redSliderWidget->EnabledOn();

  //blue
  blueRep->GetPoint1Coordinate()->SetCoordinateSystemToNormalizedDisplay();
  blueRep->GetPoint1Coordinate()->SetValue(0.9 ,1);
  blueRep->GetPoint2Coordinate()->SetCoordinateSystemToNormalizedDisplay();
  blueRep->GetPoint2Coordinate()->SetValue(0.9, .1);

  vtkSmartPointer<vtkSliderCallback> blueCallback = vtkSmartPointer<vtkSliderCallback>::New();
  blueCallback->ColorFun = colorFun2;
 // blueCallback->blue = -0.2;
  blueCallback->option = 3;

  vtkSmartPointer<vtkSliderWidget> blueSliderWidget = vtkSmartPointer<vtkSliderWidget>::New();
  blueSliderWidget->SetInteractor(iren);
  blueSliderWidget->SetRepresentation(blueRep);
  blueSliderWidget->SetAnimationModeToAnimate();
  blueSliderWidget->EnabledOn();
  // Observe the interaction events of the widget. If the computation
  // in the callback is time consuming, observe the
  // EndInteractionEvent instead.
  opacitySliderWidget->AddObserver(vtkCommand::InteractionEvent,opacityCallback);

  redSliderWidget->AddObserver(vtkCommand::InteractionEvent,redCallback);


  blueSliderWidget->AddObserver(vtkCommand::InteractionEvent,blueCallback);











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
  id->SetOrigin(origin[0],origin[1],origin[2]);
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

vtkImageAppendComponents* getAppendComponents(vtkImageData* image, data::Image activity){
  vtkImageAppendComponents* iad = vtkImageAppendComponents::New();
  iad->AddInput(image);

  vtkImageData* ad = vtkImageData::New();

  int dimensions[3];
  int size;
  double* mem;
  int counter=0;
  dimensions[0]=activity.getNrOfColumns();
  dimensions[1]=activity.getNrOfRows();
  dimensions[2]=activity.getNrOfSlices();
  size = dimensions[0]*dimensions[1]*dimensions[2];
  mem = new double[size];
  activity.copyToMem(mem,size);  

  ad->SetDimensions(image->GetDimensions());
  ad->SetSpacing(image->GetSpacing());
  const util::fvector4 origin = activity.getPropertyAs<util::fvector4>( "indexOrigin" );
  ad->SetOrigin(origin[0],origin[1],origin[2]);
  ad->SetOrigin(image->GetOrigin());
  const util::fvector4 spacing = activity.getPropertyAs<util::fvector4>( "voxelSize" );

  int *dims=image->GetDimensions();
  int c = 0;
  for (int z=0; z<dims[2]-spacing[2]; z=z+spacing[2]){
    for (int y=0; y<dims[1]-spacing[1]; y=y+spacing[1]){
      for (int x=0; x<dims[0]-spacing[0]; x=x+spacing[0]){

        for(int zz=0; zz<spacing[2]; zz++){
          for(int yy=0; yy<spacing[1]; yy++){
            for(int xx=0; xx<spacing[0];xx++){
              if(counter<size){
                ad->SetScalarComponentFromDouble(x+xx,y+yy,z+zz,0,mem[counter]);
              }else{
                ad->SetScalarComponentFromDouble(x+xx,y+yy,z+zz,0,0);
              }
              c++;            
            }
          }
        }
        counter++;
      }
    }
  }
  iad->AddInput(ad);
  return iad;
}
