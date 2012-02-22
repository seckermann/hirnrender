#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <QApplication>
#include <QVTKWidget.h>
#include "renderImage.h" 

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

	QApplication app(argc, argv);
	QVTKWidget *widget = new QVTKWidget();
	widget->SetRenderWindow( renderImage(image, activity) );

	widget->show();	
	return app.exec();
}
