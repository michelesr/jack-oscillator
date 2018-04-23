/*  controller.c implementation of midi signal handling

    Copyright (C) 2014-2015 Michele Sorcinelli

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <stdio.h>
#include <jack/midiport.h>
#include "data.h"
#include "math.h"

#define BENDER_ZERO 16384

/*
   NOTE: jack_midi_dump command will dump out midi signal
   MIDI TABLES: http://www.midi.org/techspecs/midimessages.php
*/

void handle_midi_control(jack_midi_event_t e);
void handle_midi_bending(jack_midi_event_t e);
void handle_program_change(jack_midi_event_t e);

void handle_midi_control(jack_midi_event_t e) {
  switch(*(e.buffer +1)) {
    case 7: /* volume */
      set_volume((sample_t) ((*(e.buffer+2)) /127.0));
  }
}

void handle_midi_program_change(jack_midi_event_t e) {
  set_waveform((char) *(e.buffer+1));
}

void handle_midi_bending(jack_midi_event_t e) {
  /* 4 hex digits divided in 2 bytes, Less Significant Byte First */
  int v = (*(e.buffer+1) + *(e.buffer+2) * pow(16,2)) - BENDER_ZERO;
  set_bender(1.0*v/BENDER_ZERO);
}
