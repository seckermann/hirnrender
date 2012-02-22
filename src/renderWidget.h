#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <QVTKWidget.h>

#ifndef _RENDERWIDGET_H_
#define _RENDERWIDGET_H_
class RenderWidget : public QVTKWidget{
	    Q_OBJECT
	private:
		vtkRenderWindow* renWin;
		vtkRenderWindow* renderImage(char* image, char *activity);
	public:
		vtkCamera* cam;
		RenderWidget(char* image, char *activity);
		//void setCamera(vtkCamera* cam, enum pos);
	public slots:
		void setCameraFront();
		void setCameraBack();
//		virtual void paintEvent( QPaintEvent *);

};
#endif
