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
#include "PythonListener.h"
#include <Python/Python.h>

void sendNote();

/* Takes a dictionary describing a LeapMIDI program and executes it */
static PyObject*
leapmidi_run_program(PyObject *self, PyObject *args)
{
    // in: 1 dictionary
    printf("Running program...\n");
}

static PyMethodDef leapmidiMethods[] = {
    {"run_program", leapmidi_run_program, METH_VARARGS,
        "Takes a dictionary describing a LeapMIDI program and executes it"},
    {NULL, NULL, 0, NULL}
};

int main(int argc, char * argv[]) {
    // start listening for events
    leapmidi::PythonListener listener;
    Leap::Controller controller;
    
    listener.init(&controller);
    controller.addListener(listener);
    
    // load python program and execute
    // entry function
    PyObject *pName, *pModule, *pFunc;
    PyObject *pValue;
    
    string pythonProgram = "programs.demo";
    string pythonEntryFxn = "setup";
    
    printf("Setting program name to %s\n", argv[0]);
    Py_SetProgramName(argv[0]);
    Py_Initialize();
    
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
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call to Python program entry function %s failed\n", pythonEntryFxn.c_str());
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
        fprintf(stderr, "Failed to load Python program \"%s\"\n", pythonProgram.c_str());
        return 1;
    }
    
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
