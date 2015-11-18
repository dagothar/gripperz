/* 
 * File:   RbfOptimizer.cpp
 * Author: dagothar
 * 
 * Created on November 17, 2015, 10:29 PM
 */

#include "RbfOptimizer.hpp"
#include <stdexcept>
#include <rw/common/macros.hpp>

#define DEBUG rw::common::Log::infoLog()
#define INFO rw::common::Log::infoLog()

using namespace gripperz::optimization;
using namespace gripperz::math;
using namespace std;
using namespace rw::common;

RbfOptimizer::RbfOptimizer(const Configuration& config) :
_configuration(config) {
}

RbfOptimizer::~RbfOptimizer() {
}

Vector RbfOptimizer::minimize(ObjectiveFunction::Ptr function, const Vector& initialGuess) {
    int dimensions = initialGuess.size();

    string gripperz_root = getenv("GRIPPERZ_ROOT");
    string rbf_root = gripperz_root + "/ext/rbfopt/src";
    setenv("PYTHONPATH", rbf_root.c_str(), 1);
    Py_Initialize();

    char* arg[1] = {(char*) ""};
    PySys_SetArgv(1, arg);

    PyObject* module_name = PyString_FromString("wrapper");
    PyObject* module = PyImport_Import(module_name);
    Py_DECREF(module_name);
    if (!module) {
        RW_THROW("Unable to load module wrapper!");
    }

    PyObject* optimize = PyObject_GetAttrString(module, "optimize");
    if (!optimize || !PyCallable_Check(optimize)) {
        RW_THROW("Unable to find method 'optimize'!");
    }

    PyMethodDef cb_desc = {"callback", callback, METH_VARARGS, "optimizer callback"};
    PyObject* cb_name = PyString_FromString(cb_desc.ml_name);
    PyObject* cb = PyCFunction_NewEx(&cb_desc, NULL, cb_name);
    Py_DECREF(cb_name);

    PyObject* optimize_args = PyTuple_New(5);
    PyTuple_SetItem(optimize_args, 0, PyCObject_FromVoidPtr(function.get(), NULL));
    PyTuple_SetItem(optimize_args, 1, cb);
    PyTuple_SetItem(optimize_args, 2, PyInt_FromLong(dimensions));
    PyTuple_SetItem(optimize_args, 3, PyInt_FromLong(_configuration.maxNOfIterations));
    PyTuple_SetItem(optimize_args, 4, PyInt_FromLong(_configuration.maxNOfEvaluations));
    PyObject* result_tuple = PyObject_CallObject(optimize, optimize_args);
    if (!result_tuple) {
        RW_THROW("Call to rbfopt.optimize failed!");
    }

    float value = PyFloat_AsDouble(PyTuple_GetItem(result_tuple, 0));
    DEBUG << "Value at the minimum: " << value << endl;
    
    PyObject* result_list = PyTuple_GetItem(result_tuple, 1);
    if (!result_list) {
        RW_THROW("No rbfopt result list!");
    }
    int n = PyList_Size(result_list);
    vector<double> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = PyFloat_AsDouble(PyList_GetItem(result_list, i));
    }

    Py_XDECREF(result_list);
    Py_XDECREF(result_tuple);
    Py_XDECREF(optimize_args);
    Py_XDECREF(cb);
    Py_XDECREF(optimize);
    Py_XDECREF(module);
    Py_Finalize();

    return result;
}

PyObject* RbfOptimizer::callback(PyObject* module, PyObject* args) {
    /* extract arguments */
    PyObject* arg1 = PyTuple_GetItem(args, 0); // objective
    PyObject* arg2 = PyTuple_GetItem(args, 1); // x

    ObjectiveFunction* objective = (ObjectiveFunction*) PyCObject_AsVoidPtr(arg1);

    int n = PyList_Size(arg2);
    vector<double> x(n);
    for (int i = 0; i < n; ++i) {
        x[i] = PyFloat_AsDouble(PyList_GetItem(arg2, i));
    }

    double result = objective->evaluate(x);

    return Py_BuildValue("f", result);
}
