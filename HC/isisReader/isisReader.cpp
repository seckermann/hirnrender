#include <Python.h>
#include <vtkImageData.h>
#include <DataStorage/io_application.hpp>
#include <DataStorage/image.hpp>

using namespace isis;

static PyObject *isisReader_getImageData(PyObject *self, PyObject *args);
static vtkImageData *getImageData(data::Image image);

static PyMethodDef IsisReaderMethods[] = 	{
	{"getImageData", isisReader_getImageData,METH_VARARGS,	"Simple parser for Vista Images for use with the VTK toolkit"},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initisisReader(void){
	Py_InitModule("isisReader", IsisReaderMethods);
}

static PyObject *isisReader_getImageData(PyObject *self, PyObject *args){
	PyObject *vtkImage;
	
	if(!PyArg_ParseTuple(args, "O", &vtkImage))
		return NULL;
	
	return vtkImage;
}

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


