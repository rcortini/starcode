#include <Python.h>
#include <stdlib.h>
#include "../src/starcode.h"

#define MAX_STR_LENGTH 2048

static char module_docstring[] = 
       "This module is a Python interface to Starcode";
static char starcode_docstring[] = 
       "Starcode invocation";

static PyObject *pystarcode_starcode(PyObject *self, PyObject *args, PyObject *kwargs);

static PyMethodDef module_methods[] = {
  {"starcode", pystarcode_starcode, METH_VARARGS|METH_KEYWORDS, starcode_docstring},
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initpystarcode(void)
{
  PyObject *m = Py_InitModule3("pystarcode", module_methods, module_docstring);
  if (m == NULL)
    return;
}

FILE *py_fopen(const char *fname, const char *mode)
{
  FILE *f = fopen(fname, mode);
  if (f==NULL)
  {
    char error_message[MAX_STR_LENGTH];
    sprintf(error_message, "Cannot open file %s", fname);
    PyErr_SetString(PyExc_IOError, error_message);
    return NULL;
  }
  else
    return f;
}

static PyObject *pystarcode_starcode(PyObject *self, PyObject *args, PyObject *kwargs)
{
  // Parse the input
  char *in_filename, *out_filename;
  int tau, cluster_ratio;
  int clusteralg = 0;
  int verbose = 1;
  int thrmax = 4;

  static char *kwlist[] = {
    "input",
    "output",
    "dist",
    "cluster_ratio",
    "clusteralg",
    "verbose",
    "threads",
    NULL};
  if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ssii|iii", kwlist,
	&in_filename,
	&out_filename,
	&tau,
	&cluster_ratio,
	&clusteralg,
	&verbose,
	&thrmax))
    return NULL;

  // open input and output files
  FILE *inputf1 = py_fopen(in_filename,"r");
  if (inputf1 == NULL) return NULL;
  FILE *inputf2 = NULL;
  FILE *outputf1 = py_fopen(out_filename, "w");
  if (outputf1 == NULL) return NULL;
  FILE *outputf2 = NULL;

  const int showclusters = 1;
  const int showids = 0;
  const int outputt = 0;

  // printf("%s %s %d %d %d %d\n",in_filename,out_filename,tau,cluster_ratio,clusteralg,verbose);

  int out = starcode(
      inputf1,
      inputf2,
      outputf1,
      outputf2,
      tau,
      verbose,
      thrmax,
      clusteralg,
      cluster_ratio,
      showclusters,
      showids,
      outputt
  );

  // int out = 0;

  PyObject *ret = Py_BuildValue("h", out);

  return ret;
}
