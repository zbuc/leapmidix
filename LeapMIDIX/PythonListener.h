//
//  PythonListener.h
//  LeapMIDIX
//
//  Created by Chris Czub on 2/17/13.
//  Copyright (c) 2013 DBA int80. All rights reserved.
//

#ifndef __LeapMIDIX__PythonListener__
#define __LeapMIDIX__PythonListener__

#include <iostream>
#include "Visualizer.h"
#include "Device.h"
#include "Leap.h"
#include "LeapMIDI.h"
#include "FingerControlProgram.h"
#include "FingerNoteProgram.h"

//#define LMX_VISUALIZER_ENABLED 1

namespace leapmidi {
    
    class Visualizer;
    
    class PythonListener : public leapmidi::Listener {
    public:
        PythonListener();
        virtual ~PythonListener();
        
        void init(Leap::Controller *controller);
        
        // run forever, drawing frames
        void drawLoop();
        
        virtual void onGestureRecognized(const Leap::Controller &controller, leapmidi::GesturePtr gesture);
        virtual void onControlUpdated(const Leap::Controller &controller, leapmidi::GesturePtr gesture, leapmidi::ControlPtr control);
        virtual void onNoteUpdated(const Leap::Controller &controller, leapmidi::GesturePtr gesture, leapmidi::NotePtr note);
        
    protected:
        void processFrameRaw(const Leap::Frame &frame);
        void processFrameTools(const Leap::Frame &frame);
        
        Device *device;
        Visualizer *viz;
    };
    
}

#endif /* defined(__LeapMIDIX__PythonListener__) */
