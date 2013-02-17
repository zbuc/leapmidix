//
//  PythonListener.cpp
//  LeapMIDIX
//
//  Created by Chris Czub on 2/17/13.
//  Copyright (c) 2013 DBA int80. All rights reserved.
//

#include <memory>
#include <map>

#include "PythonListener.h"

namespace leapmidi {
    
    PythonListener::PythonListener() {
        viz = NULL;
        device = NULL;
    }
    
    void PythonListener::init(Leap::Controller *controller) {
        std::cout << "Leap MIDI device initalized" << std::endl;
        
        // create virtual midi source
        device = new Device();
        device->init();
        
        viz = NULL;
#ifdef LMX_VISUALIZER_ENABLED
        // initialize visualizer
        viz = new Visualizer();
        viz->init(this);
#endif
        
        // create default program
        /*    FingerControlPtr controlProgram = make_shared<FingerControl>();
         controlProgram->initGestures();
         setProgram(controlProgram);
         */
        FingerNotePtr noteProgram = make_shared<FingerNote>();
        noteProgram->initGestures();
        setProgram(noteProgram);
    }
    
    PythonListener::~PythonListener() {
        if (viz)
            delete viz;
        if (device)
            delete device;
    }
    
    void PythonListener::onGestureRecognized(const Leap::Controller &controller, GesturePtr gesture) {
        leapmidi::Listener::onGestureRecognized(controller, gesture);
    }
    
    void PythonListener::onControlUpdated(const Leap::Controller &controller, GesturePtr gesture, ControlPtr control) {
        // call superclass method
        leapmidi::Listener::onControlUpdated(controller, gesture, control);
        
        // draw gesture and control output
        // ...
        
        // control
        leapmidi::midi_control_index controlIndex = control->controlIndex();
        // control value
        leapmidi::midi_control_value val = control->mappedValue();
        
        if (0) {
            cout << "recognized control index " << controlIndex
            << " (" << control->description() << ")"
            << ", raw value: "
            << control->rawValue() << " mapped value: " << val << endl;
        }
        
        device->addControlMessage(controlIndex, val);
    }
    
    void PythonListener::onNoteUpdated(const Leap::Controller &controller, GesturePtr gesture, NotePtr note) {
        // call superclass method
        leapmidi::Listener::onNoteUpdated(controller, gesture, note);
        
        // draw gesture and note output
        // ...
        
        // note
        leapmidi::midi_note_index noteIndex = note->noteIndexOff();
        // control value
        leapmidi::midi_note_value val = note->mappedValue();
        
        if (note->note_state == NOTE_ON) {
            noteIndex = note->noteIndexOn();
            device->addNoteOnMessage(noteIndex, val);
        } else {
            device->addNoteOffMessage(noteIndex, val);
        }
        
        if (0) {
            cout << "recognized note index " << noteIndex
            << " (" << note->description() << ")"
            << ", raw value: "
            << note->rawValue() << " mapped value: " << val << endl;
        }
        
    }
    
    
    void PythonListener::drawLoop() {
#ifdef LMX_VISUALIZER_ENABLED
        viz->drawLoop();
#else
        std::cin.get();
#endif
    }
    
} // namespace leapmidi