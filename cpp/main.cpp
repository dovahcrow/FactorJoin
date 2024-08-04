#include <cstddef>
#include <iostream>
#include <format>
#include <Python.h>

#define PY_SSIZE_T_CLEAN

using namespace std;

static PyObject *
test111(PyObject *self, PyObject *args)
{
    cout << "in cpp" << endl;
    Py_RETURN_NONE;
}

int main(int argc, char *argv[]) {
  Py_Initialize();

  // TODO: Change: ..
  PyRun_SimpleString("import sys\nsys.path.append(\"../\")\n");

  PyRun_SimpleString(R"(
import pickle
import time
import numpy as np
)");

  // TODO: Change 
  PyRun_SimpleString(R"(model_path = "../checkpoints/model_stats_greedy_200.pkl")");
  // TODO: Change 
  PyRun_SimpleString(R"(query_file = "../../End-to-End-CardEst-Benchmark/workloads/stats_CEB/sub_plan_queries/stats_CEB_sub_queries.sql")");
  PyRun_SimpleString(R"(
with open(model_path, "rb") as f:
		bound_ensemble = pickle.load(f)
)");
  PyRun_SimpleString(R"(
with open(query_file, "r") as f:
		queries = f.readlines()
)");

  PyObject *main_module = PyImport_AddModule("__main__");
  if (!main_module) {
      fprintf(stderr, "Failed to get __main__ module\n");
      return -1;
  }
  PyObject *main_dict = PyModule_GetDict(main_module);

  static PyMethodDef add_method = {
      "test111", test111, METH_VARARGS, "test111"
  };

  PyObject *py_add_func = PyCFunction_New(&add_method, NULL);

  PyDict_SetItemString(main_dict, "test111", py_add_func);

  PyRun_SimpleString(R"(
bound_ensemble.test111() ## print "in python" because we haven't set yet
bound_ensemble.test111 = test111
)");

  PyRun_SimpleString(R"(
bound_ensemble.get_cardinality_bound_one(queries[0])
)");
}
