/*  controller.c implementation of midi control handling  
 
    Copyright (C) 2004 Ian Esten
    Copyright (C) 2014 Michele Sorcinelli 
    
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

void debug_print_midi(jack_midi_event_t event);
void handle_midi_control(jack_midi_event_t event);
void handle_program_change(jack_midi_event_t event);

void debug_print_midi (jack_midi_event_t event) {
  fprintf(stderr, "event: type:%x ch:%d par_a:%d par_b:%d \n", *(event.buffer)&0xf0, (*(event.buffer)&0x0f)+1, *(event.buffer+1), *(event.buffer+2) );
}

void handle_midi_control(jack_midi_event_t event) {
  switch(*(event.buffer +1)) {
    case 7: /* volume */
      set_volume((sample_t) ((*(event.buffer+2)) /127.0));
  }
}

void handle_midi_program_change(jack_midi_event_t event) {
  set_waveform((char) *(event.buffer+1));
}
