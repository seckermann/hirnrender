#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <QApplication>
#include <QVTKWidget.h>
#include "renderWidget.h" 
#include <QPushButton>


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
	RenderWidget *widget = new RenderWidget(image, activity);

	/*
		QPushButton hello("Ende");
		QObject::connect( &hello, SIGNAL( clicked() ),
				&widget, SLOT( quit() ) );





hello->show();*/
	widget->show();	
	return app.exec();
}
