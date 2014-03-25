/*  data.c header of types and data
 
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

#include <jack/jack.h>
#define false 0
#define true 1

typedef jack_default_audio_sample_t sample_t;
typedef short int bool_t;

extern sample_t max_amplitude, volume, attack_amplitude, sustain;
extern unsigned short channel;
extern int fi, attack_time, decay_time, release_time;
extern char waveform;
extern bool_t set_waveform(const char);
extern bool_t set_fi(const int );
extern bool_t set_amplitude(const int);
extern bool_t set_volume(const sample_t);
extern bool_t set_channel(const unsigned short);
extern bool_t set_attack_time(const int );
extern bool_t set_attack_amplitude(const sample_t);
extern bool_t set_decay(const int);
extern bool_t set_sustain(const sample_t);
extern bool_t set_release(const int);
extern bool_t check_waveform(const char);
