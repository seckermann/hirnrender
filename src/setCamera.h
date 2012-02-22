#include<vtkCamera.h>

#ifndef _SETCAMERA_H_
#define _SETCAMERA_H_
enum pos {u,o,h,v,r,l};
void setCamera(vtkCamera* cam, enum pos);
#endif
