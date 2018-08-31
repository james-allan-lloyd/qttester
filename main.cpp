#include "Python.h"
#include "mainwindow.h"
#include <QApplication>

#include <iostream>

/*
 * https://github.com/google/clif
 *
 *
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(argc != 3)
    {
        std::cout << "USAGE: test-app.exe TEST_SCRIPT TEST_FUNC" << std::endl;
        return 1;
    }

    MainWindow w;
    w.show();

    Py_Initialize();
    PyObject* pName = PyUnicode_DecodeFSDefault("simple_test");  // argv[1]);
    // std::cout << "Loading file: " << argv[1] << std::endl;
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        std::cout << "Loaded module." << std::endl;
        PyObject* pFunc = PyObject_GetAttrString(pModule, argv[2]);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject* pArgs = PyTuple_New(argc - 3);
            for (int i = 0; i < argc - 3; ++i) {
                PyObject* pValue = PyLong_FromLong(atoi(argv[i + 3]));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyLong_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }

    int result = a.exec();

    if (Py_FinalizeEx() < 0) {
        return 120;
    }

    return result;
}
