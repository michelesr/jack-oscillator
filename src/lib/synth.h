/*  synth.h header of wave synthesizer
 
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

#include "data.h"

extern void calc_note_frqs(sample_t *, sample_t); 
extern sample_t sine_w(sample_t);
extern sample_t square_w(sample_t);
extern sample_t sawtooth_w(sample_t);
extern sample_t triangle_w(sample_t);
extern void set_note(unsigned char);
extern void set_old_note(unsigned char);
extern void set_note_on();
extern void set_note_off();
extern void adsr_init();
extern void adsr_reset();
extern sample_t output(sample_t *, jack_nframes_t);
