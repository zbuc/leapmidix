//
//  main.cpp
//  LeapMIDIX
//
//  Created by Mischa Spiegelmock on 12/1/12.
//  Copyright (c) 2012 DBA int80. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <time.h>
#include "LMXListener.h"
#include <Python/Python.h>

void sendNote();

static int numargs=0;

/* Return the number of arguments of the application command line */
static PyObject*
leapmidi_numargs(PyObject *self, PyObject *args)
{
    if(!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return Py_BuildValue("i", numargs);
}

static PyMethodDef leapmidiMethods[] = {
    {"numargs", leapmidi_numargs, METH_VARARGS,
        "Return the number of arguments received by the process."},
    {NULL, NULL, 0, NULL}
};

int main(int argc, char * argv[]) {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pValue;
    
    string pythonProgram = "programs.demo";
    string pythonEntryFxn = "foo";
    
    printf("Setting program name to %s\n", argv[0]);
    Py_SetProgramName(argv[0]);
    Py_Initialize();
    
    numargs = argc;
    Py_InitModule("leapmidi", leapmidiMethods);
    
    PySys_SetArgv(argc, argv);
    pName = PyString_FromString(pythonProgram.c_str());
    /* Error checking of pName left out */
    
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    
    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, pythonEntryFxn.c_str());  
        if (pFunc && PyCallable_Check(pFunc)) {
            pValue = PyObject_CallObject(pFunc, NULL);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
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
            fprintf(stderr, "Cannot find function \"%s\"\n", pythonEntryFxn.c_str());
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", pythonProgram.c_str());
        return 1;
    }
    
    // start listening for events
    leapmidi::LMXListener listener;
    Leap::Controller controller;
    
    listener.init(&controller);
    controller.addListener(listener);
    
    // run forever
    try {
        listener.drawLoop();
    } catch (const std::exception &ex) {
        std::cerr << "Caught exception: " << ex.what() << std::endl;
        exit(1);
    }

    Py_Finalize();

    return 0;
}
