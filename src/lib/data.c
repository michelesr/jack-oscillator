/*  data.c implementation of types and data
 
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
#include <string.h> 
#include <stdio.h>
#define false 0 
#define true 1

/* number of implemented waveforms */
#define FORMS 4 

typedef jack_default_audio_sample_t sample_t;
typedef char bool_t;

/* global vars */
sample_t max_amplitude = 0.5, volume = 1, attack_amplitude = 1, sustain = 1, bender = 0.0;
int fi = 20; /* number of harmonics (not used anymore) */
int attack_time = 1, decay_time = 1, release_time = 1;
unsigned short channel = 1;
char waveform = 0;
bool_t set_waveform(const char);
bool_t set_fi(const int );
bool_t set_amplitude(const sample_t);
bool_t set_volume(const sample_t);
bool_t set_bender(const sample_t);
bool_t set_channel(const unsigned short);
bool_t set_attack_time(const int );
bool_t set_attack_amplitude(const sample_t);
bool_t set_decay(const int);
bool_t set_sustain(const sample_t);
bool_t set_release(const int);

bool_t set_waveform(const char wf) {
  char name[10];
  if (wf >= 0 && wf < FORMS) {
    waveform = wf;
    switch(waveform) {
      case 0:
        strcpy(name, "sine");
        break;
      case 1:
        strcpy(name, "square");
        break;
      case 2:
        strcpy(name, "sawtooth");
        break;
      case 3:
        strcpy(name, "triangle");
        break;
    }
    printf("Changed waveform: %s\n", name);
    return true;
  }
  else {
    printf("invalid waveform\n");
    return false;
  }
}

bool_t set_fi(const int f) {
  if (f > 0) {
    fi = f;
    return true;
  }
  else 
    return false;
}

bool_t set_amplitude(const sample_t a) {
  if(a < 0 || a > 5) {
    printf("invalid amplitude\n");
    return false;
  }
  else {
    max_amplitude = a;
    printf("New amplitude = %.2lf\n",  max_amplitude);
    return true;
  }
}

bool_t set_bender(const sample_t b) {
  bender = b;
  return true;
}

bool_t set_channel(const unsigned short c) {
  if (c < 1 || c > 16) {
    printf("Invalid channel number\n");
    return false;
  }
  else {
    channel = c;
    printf("Changed midi channel to %d\n", channel);
    return true;
  }
}

bool_t set_attack_time(const int at) {
  if (at < 1) {
    printf("attack time must be >= 1 ms\n");
    return false;
  }
  else {
    attack_time = at;
    printf("attack time = %d ms\n", attack_time);
    return true;
  }
}

bool_t set_attack_amplitude(const sample_t aa) {
  if (aa < 0 || aa > 5) {
    printf("invalid attack amplitude peak!\n");
    return false;
  }
  else {
    attack_amplitude = aa;
    printf("attack amplitude peak = %f\n", attack_amplitude);
    return true;
  }
}

bool_t set_decay(const int d) {
  if (d < 1) {
    printf("decay time must be >= 1 ms\n"); 
    return false;
  }
  else {
    decay_time = d;
    printf("decay time (ms): %d ms\n", decay_time);
    return true;
  }
}

bool_t set_sustain(const sample_t s) {
  if (s < 0 || s > 5) {
    printf("invalid sustain!\n");
    return false;
  }
  else {
    sustain = s;
    printf("sustain = %f\n", sustain);
    return true;
  }
}

bool_t set_release(const int r) {
  if (r < 1) {
    printf("release time must be >= 1 ms\n"); 
    return false;
  }
  else {
    release_time = r;
    printf("release time (ms): %d ms\n", release_time);
    return true;
  }
}

bool_t set_volume(const sample_t v) {
  if (v < 0 || v > 1) {
    printf("invalid volume\n");
    return false;
  } 
  else { 
    volume = v;
    printf("Volume: %f\n", volume);
    return true;
  }
}
