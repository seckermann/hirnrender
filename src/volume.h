#include<vtkImageAppendComponents.h>
#include<DataStorage/image.hpp>
#ifndef _VOLUME_H_
#define _VOLUME_H_
class Volume : public vtkImageAppendComponents{
  private:
    bool withActivity;
    int size1;
    int size2;
    int dims[3];
    int dims1[3];
    int dims2[3];
    vtkImageData* id;
    vtkImageData* ad;
    isis::data::Image* image;
    isis::data::Image* activity;
    double* bounds;
    double* range;
    void setImageData(isis::data::Image* image );
  public:
    Volume(isis::data::Image* image, isis::data::Image* activity=NULL);
    double* getBounds();
    double* getRange();
    bool hasActivity();
};
#endif
