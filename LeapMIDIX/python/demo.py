#!/usr/bin/env python

import leapmidi

def handle_finger_x(finger_index, finger_x_position):
    if finger_x_position > 50:
        # trigger note on
        leapmidi.addMIDIPacket((0x90, 0x48, 0x7F))
    else:
        leapmidi.addMIDIPacket((0x80, 0x48, 0x00))


def setup():
    program = {"FingerPositionX": handle_finger_x}
    leapmidi.run_program(program)
