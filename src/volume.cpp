#include"volume.h"
#include"DataStorage/image.hpp"
#include<vtkImageData.h>
Volume::Volume(isis::data::Image* image, isis::data::Image* activity){
  double* mem1;
  double* mem2;
  isis::util::fvector4 spacing1;
  isis::util::fvector4 spacing2;
  isis::util::fvector4 origin1;
  isis::util::fvector4 origin2;
  this->image = image;
  this->activity = activity;
  withActivity = (activity!=NULL);


  //Image
  spacing1 = image->getPropertyAs<isis::util::fvector4>("voxelSize");
  origin1 = image->getPropertyAs<isis::util::fvector4>("indexOrigin");
  dims1[0]= image->getNrOfColumns();
  dims1[1]= image->getNrOfRows();
  dims1[2]= image->getNrOfSlices();
  dims[0]= dims1[0]*spacing1[0];
  dims[1]= dims1[1]*spacing1[1];
  dims[2]= dims1[2]*spacing1[2];
  size1 = dims1[0]*dims1[1]*dims1[2];
  mem1 = new double[size1];
  image->copyToMem(mem1, size1);
  //Activity
  if(withActivity){
    spacing2 = activity->getPropertyAs<isis::util::fvector4>("voxelSize");
    origin2 = activity->getPropertyAs<isis::util::fvector4>("indexOrigin");
    dims2[0]= activity->getNrOfColumns();
    if(dims2[0]*spacing2[0]>dims[0]){
      cout<<"Dimensions are false"<< endl;
      exit(-1);
    }
    dims2[1]= activity->getNrOfRows();
    if(dims2[1]*spacing2[1]>dims[1]){
      cout<<"Dimensions are false"<< endl;
      exit(-1);
    }
    dims2[2]= activity->getNrOfSlices();
    if(dims2[2]*spacing2[2]>dims[2]){
      cout<<"Dimensions are false"<< endl;
      exit(-1);
    }
    size2 = dims2[0]*dims2[1]*dims2[2];
    mem2 = new double[size2];
    activity->copyToMem(mem2, size2);
  }
  //making id and ad
  id = vtkImageData::New();
  id->SetDimensions(dims);
  id->SetSpacing(1,1,1);
  if(withActivity){
    ad = vtkImageData::New();
    ad->SetDimensions(dims[0], dims[1],dims[2]);
    ad->SetSpacing(1,1,1);
  }

  int counter = 0;
  for(int z=0; z<dims1[2]*spacing1[2]; z=z+spacing1[2]){
    for(int y=0; y<dims1[1]*spacing1[1]; y=y+spacing1[1]){
      for(int x=0; x<dims1[0]*spacing1[0]; x=x+spacing1[0]){

        for(int zz=0; zz<spacing1[2]; zz++){
          for(int yy=0; yy<spacing1[1]; yy++){
            for(int xx=0; xx<spacing1[0]; xx++){
              id->SetScalarComponentFromDouble(x+xx,y+yy,z+zz,0,mem1[counter]); 
            }
          }
        }
        counter++;
      }
    }
  }
  bounds=id->GetBounds();
  AddInput(id);



  if(withActivity){
    counter=0;

    for(int z=0; z<dims2[2]*spacing2[2]; z=z+spacing2[2]){
      for(int y=0; y<dims2[1]*spacing2[1]; y=y+spacing2[1]){
        for(int x=0; x<dims2[0]*spacing2[0]; x=x+spacing2[0]){

          for(int zz=0; zz<spacing2[2]; zz++){
            for(int yy=0; yy<spacing2[1]; yy++){
              for(int xx=0; xx<spacing2[0]; xx++){
                ad->SetScalarComponentFromDouble(x+xx,y+yy,z+zz,0,mem2[counter]); 
              }
            }
          }
          counter++;
        }
      }
    }
    for(int z=dims2[2]*spacing2[2]; z<dims[2]; z++){
      for(int y=dims2[1]*spacing2[1]; y<dims[1]; y++){
        for(int x=dims2[0]*spacing2[0]; x<dims[0];x++){
          ad->SetScalarComponentFromDouble(x,y,z,0,0); 
        }
      }
    }
    range=ad->GetScalarRange();
    AddInput(ad);
	delete [] mem1;
	delete [] mem2;
  }


}

Volume::~Volume(){
	id->Delete();
	ad->Delete();
	this->Delete();
}

bool Volume::hasActivity(){
  return withActivity;
}
double* Volume::getBounds(){
  return bounds;
}
double* Volume::getRange(){
    return range;
}
