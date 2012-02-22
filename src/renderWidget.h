#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <QVTKWidget.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#ifndef _RENDERWIDGET_H_
#define _RENDERWIDGET_H_
class RenderWidget : public QVTKWidget{
	    Q_OBJECT
	private:
		double* cropping;
		vtkRenderWindow* renWin;
		vtkRenderer* renderer;
		vtkFixedPointVolumeRayCastMapper* mapper;
		vtkRenderWindow* renderImage(char* image, char *activity);
	public:
		vtkCamera* cam;
		RenderWidget(char* image, char *activity);
		//void setCamera(vtkCamera* cam, enum pos);
	public slots:
		void setCameraFront();
		void setCameraBack();
		void setCameraBottom();
		void setCameraRight();
		void setCameraLeft();
		void setCameraTop();
		void setCropping();
		void setCroppingX(double* x);
		void setCroppingY(double* y);
		void setCroppingZ(double* z);

};
#endif
