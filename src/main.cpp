#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <QApplication>
#include <QVTKWidget.h>
#include "renderWidget.h" 
#include <QPushButton>
#include <QMainWindow>


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


	QPushButton front("vorn");
	QObject::connect( &front, SIGNAL( clicked() ),
			widget, SLOT( setCameraFront() ) );
	front.show();

	QPushButton back("hinten");
	QObject::connect( &back, SIGNAL( clicked() ),
			widget, SLOT( setCameraBack() ) );
	back.show();

	QPushButton right("rechts");
	QObject::connect( &right, SIGNAL( clicked() ),
			widget, SLOT( setCameraRight() ) );
	right.show();

	QPushButton left("links");
	QObject::connect( &left, SIGNAL( clicked() ),
			widget, SLOT( setCameraLeft() ) );
	left.show();

	QPushButton top("oben");
	QObject::connect( &top, SIGNAL( clicked() ),
			widget, SLOT( setCameraTop() ) );
	top.show();

	QPushButton bottom("unten");
	QObject::connect( &bottom, SIGNAL( clicked() ),
			widget, SLOT( setCameraBottom() ) );
	bottom.show();

	widget->show();	
	return app.exec();
}
