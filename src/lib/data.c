/*  data.c implementation of types and data
 
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
#include <string.h> 
#include <stdio.h>

/* number of implemented waveforms */
#define FORMS 4 

typedef jack_default_audio_sample_t sample_t;
typedef enum {false, true} bool_t;

/* global vars */
sample_t max_amplitude = 0.5, volume = 1, attack_amplitude = 1, sustain = 1;
int fi = 20; /* number of harmonics */
int attack_time = 1, decay_time = 1, release_time = 1;
unsigned short channel = 1;
char waveform = 'a';
bool_t set_waveform(const char);
bool_t set_fi(const int );
bool_t set_amplitude(const int);
bool_t set_volume(const sample_t);
bool_t set_channel(const unsigned short);
bool_t set_attack_time(const int );
bool_t set_attack_amplitude(const sample_t);
bool_t set_decay(const int);
bool_t set_sustain(const sample_t);
bool_t set_release(const int);
bool_t check_waveform(const char);

bool_t check_waveform(const char c) {
  char valid_forms[FORMS];
  int i, v;
  for (i=0; i < FORMS; i++) 
    valid_forms[i] = (i + 'a');
  for (i=0; i < FORMS && c != valid_forms[i]; i++); 
  return (i < FORMS? true: false);
}

bool_t set_waveform(const char wf) {
  char name[10];
  if (check_waveform(wf)) {
    waveform = wf;
    switch(waveform) {
      case 'a':
        strcpy(name, "sine");
        break;
      case 'b':
        strcpy(name, "square");
        break;
      case 'c':
        strcpy(name, "sawtooth");
        break;
      case 'd':
        strcpy(name, "triangle");
        break;
    }
    printf("Changed waveform: %s\n", name);
    return true;
  }
  else 
    printf("invalid waveform\n");
    return false;
}

bool_t set_fi(const int f) {
  if (fi < 1) {
    fi = f;
    return true;
  }
  else 
    return false;
}

bool_t set_amplitude(const int a) {
  if(a > 5 || a < 5) {
    printf("invalid amplitude\n");
    return false;
  }
  else {
    max_amplitude = a;
    printf("New amplitude = %.2lf\n",  max_amplitude);
    if (max_amplitude < 0)
      printf("A < 0 => Inverted Phase\n"); 
    return true;
  }
}

bool_t set_channel(const unsigned short c) {
  if (c < 0 || c > 16) {
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
    printf("attack time must be > 1 ms\n");
    return false;
  }
  else {
    attack_time = at;
    printf("attack time = %d ms\n", attack_time);
    return true;
  }
}

bool_t set_attack_amplitude(const sample_t aa) {
  if (aa < 5 || aa > 5) {
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
    printf("decay time must be > 1 ms\n"); 
    return false;
  }
  else {
    decay_time = d;
    printf("decay time (ms): %d ms\n", decay_time);
    return true;
  }
}

bool_t set_sustain(const sample_t s) {
  if (s < 5 || s > 5) {
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
    printf("release time must be > 1 ms\n"); 
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
