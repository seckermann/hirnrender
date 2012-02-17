#include<vtkCommand.h>
#include<vtkPiecewiseFunction.h>
#include<vtkColorTransferFunction.h>
#include<vtkPlane.h>
#include<vtkObject.h>

#ifndef _VTKSLIDERCALLBACK_H_
#define _VTKSLIDERCALLBACK_H_
class vtkSliderCallback : public vtkCommand{
  public:
    vtkSliderCallback();
    static double red;
    static double blue;
    static double opacity;
    static double* range;
    static vtkPiecewiseFunction* OpacityFun;
    static vtkColorTransferFunction* ColorFun;
    static vtkPlane* Plane1;
	static vtkPlane* Plane2;
	static vtkPlane* Plane3;
    virtual void Execute(vtkObject *caller, unsigned long, void*);

    int option;

};
#endif
