#include"vtkSliderCallback.h"
#include<vtkSliderWidget.h>
#include<vtkSliderRepresentation.h>

vtkSliderCallback::vtkSliderCallback(){
}



void vtkSliderCallback::Execute(vtkObject *caller, unsigned long, void*){
  vtkSliderWidget *sliderWidget = reinterpret_cast<vtkSliderWidget*>(caller);
  double value = static_cast<double>(static_cast<vtkSliderRepresentation *>(sliderWidget->GetRepresentation())->GetValue());

  switch(option){
    case 1:
      this->opacity=value;

      this->OpacityFun->AddSegment(range[0],value,this->blue, value);
      this->OpacityFun->AddSegment(this->blue+0.001,0,this->red-0.001, 0);
      this->OpacityFun->AddSegment(this->red,value,range[1], value);
      break;
    case 2:
      this->red=value;
      this->ColorFun->AddRGBSegment (0,0,0,0,range[1],0,0,0);
      this->ColorFun->AddRGBSegment (0,0,0,0,value-0.001,0,0,0);
      this->ColorFun->AddRGBSegment (value,1,0,0,range[1],1,1,1);
      this->OpacityFun->AddSegment(0,0,value-0.001,0);	
      this->OpacityFun->AddSegment(value,this->opacity,range[1],this->opacity);	
      break;
    case 3:
      this->blue=value;
      this->ColorFun->AddRGBSegment (range[0],0,0,0,0,0,0,0);
      this->ColorFun->AddRGBSegment (range[0],1,1,1,value,0,0,1);
      this->OpacityFun->AddSegment(range[0],this->opacity,value,this->opacity);	
      this->OpacityFun->AddSegment(value+0.001,0,0,0);	
      break;
    case 4:
      this->Plane1->SetOrigin(value,0,0);
      break;
  }
}

double* vtkSliderCallback::range;
double vtkSliderCallback::red;
double vtkSliderCallback::blue;
double vtkSliderCallback::opacity;
vtkPiecewiseFunction* vtkSliderCallback::OpacityFun;
vtkColorTransferFunction* vtkSliderCallback::ColorFun;
vtkPlane* vtkSliderCallback::Plane1;
