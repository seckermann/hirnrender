#include"setCamera.h"
void setCamera(vtkCamera* cam, pos position){
	const double unten[] = {79.5,99.5,659.58};
	const double oben[] = {79.5,99.5,-659.58};
	const double hinten[] = {79.5,600,0};
	const double vorn[] = {79.5,-600,0};
	const double links[] = {600,99.5,0};
	const double rechts[] = {-600,99.5,0};
	switch(position){
		case u:
			cam->SetPosition(unten);
			cam->SetRoll(180);
			break;
		case o:
			cam->SetPosition(oben);
			break;
		case l:
			cam->SetPosition(links);
			cam->SetRoll(90);
			break;
		case r:
			cam->SetPosition(rechts);
			cam->SetRoll(-90);
			break;
		case v:
			cam->SetPosition(vorn);
			break;
		case h:
			cam->SetPosition(hinten);
			cam->SetRoll(180);
			break;

	}
}
