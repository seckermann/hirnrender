#include <Python.h>

static PyObject *chiffre_caesar(PyObject *self, PyObject *args);

static PyMethodDef ChiffreMethods[] = 	{
	{"caesar", chiffre_caesar,METH_VARARGS,	"Perform Caesar cipher encryption."},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initchiffre(void){
	Py_InitModule("chiffre", ChiffreMethods);
}

static PyObject *chiffre_caesar(PyObject *self, PyObject *args){
	char *text, *c, *e;
	PyObject *ergebnis;
	int cipher;

	if(!PyArg_ParseTuple(args, "si", &text, &cipher))
		return NULL;

	ergebnis = PyString_FromStringAndSize(NULL, strlen(text));
	e = PyString_AsString(ergebnis);

	for(c = text; *c; c++, e++) {
		if((*c < 'A') || (*c > 'Z')) {
			PyErr_SetString(PyExc_ValueError,"Character out of range");
			Py_DECREF(ergebnis);
			return NULL;
		}
		*e = ((*c - 'A' + cipher) % 26) + 'A';
	}
	return ergebnis;
}


