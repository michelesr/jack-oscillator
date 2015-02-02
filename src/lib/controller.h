/*  controller.h header of midi control handling
 
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

#include <jack/jack.h>

extern void handle_midi_control(jack_midi_event_t event);
extern void handle_midi_bending(jack_midi_event_t event);
extern void handle_midi_program_change(jack_midi_event_t event); 
