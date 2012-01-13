#include <vtkImageData.h>
#include <DataStorage/io_application.hpp>
#include <DataStorage/image.hpp>

using namespace isis;

vtkImageData* getImageData(data::Image image){
  vtkImageData* id;
  int dims[3];
  int size;
  double* mem;
  int counter=0;
  dims[0]=image.getNrOfColumns();
  dims[1]=image.getNrOfRows();
  dims[2]=image.getNrOfSlices();
  size = dims[0]*dims[1]*dims[2];
  mem = new double[size];
  image.copyToMem(mem,size);  

  id = vtkImageData::New();
  id->SetDimensions(dims);
  const util::fvector4 spacing = image.getPropertyAs<util::fvector4>( "voxelSize" );
  id->SetSpacing(spacing[0],spacing[1],spacing[2]);
  const util::fvector4 origin = image.getPropertyAs<util::fvector4>( "indexOrigin" );
  id->SetOrigin(origin[0]-spacing[0]/2,origin[1]-spacing[0]/2,origin[2]-spacing[0]/2);

  for (int z=0; z<dims[2]; z++){
    for (int y=0; y<dims[1]; y++){
      for (int x=0; x<dims[0]; x++){
        id->SetScalarComponentFromDouble(x,y,z,0,mem[counter]);
        counter++;
      }
    }
  }
  return id;
}

