/*  synth.c implementation of wave synthesizer
 
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

#include <math.h>
#include <jack/jack.h>
#include "data.h"

/* frequence of A4 */
#define TUNING 440.0 

/* gain adjustment for waveforms */
#define GAIN_SIN 1.0
#define GAIN_SQR 0.85
#define GAIN_SAW 0.885
#define GAIN_TRI 1.01

typedef jack_default_audio_sample_t sample_t;

/* global vars */
sample_t ramp = 0.0;
sample_t note_on = 0;
unsigned char note = 0, old_note = 0;
sample_t attack,decay,release;

/* function declaration */
void calc_note_frqs(sample_t *, sample_t); 
sample_t sine_w(sample_t);
sample_t square_w(sample_t);
sample_t sawtooth_w(sample_t);
sample_t triangle_w(sample_t);
sample_t generate_wave(sample_t *, jack_nframes_t);
void set_note(unsigned char);
void set_old_note(unsigned char);
void set_note_on();
void set_note_off();
void adsr_init();
void adsr_reset();

/* function definition */

void set_note(unsigned char n) {
  note = n;
}

void set_old_note(unsigned char n) {
  old_note = n;
}

void set_note_on() {
  note_on = (sample_t)1.0;
}

void set_note_off() {
  note_on = (sample_t)0.0;
}

void adsr_init() {
  attack = 0;
  release = 0;
  decay = attack_amplitude; 
}

void adsr_reset() {
  attack = 0;
  decay = attack_amplitude;
  release = sustain;
}

sample_t generate_wave(sample_t *note_frqs, jack_nframes_t sr) {
  if (note_on) {
    ramp += note_frqs[note];
    ramp = (ramp > 1.0) ? ramp - 2.0 : ramp;
    if (attack_time > 0 && attack < attack_amplitude) 
      attack += (attack_amplitude/(sr*attack_time/1000));
    else if (decay_time > 0 && decay > (sustain/attack_amplitude)) 
      decay -= (sustain/(sr*decay_time/1000));
    switch(waveform) {
      case 0:
        return(attack * decay * sine_w(ramp));
        break;
      case 1:
        return(attack * decay * square_w(ramp));
        break;
      case 2:
        return(attack * decay * sawtooth_w(ramp)); 
        break;
      case 3:
        return(attack * decay * triangle_w(ramp));
        break;
    }
  }
  else if (release > 0) {
    ramp += note_frqs[old_note];
    ramp = (ramp > 1.0) ? ramp - 2.0 : ramp;
    release -= (sustain/(sr*release_time/1000)); 
    switch(waveform) {
      case 0:
        return(release * sine_w(ramp));
        break;
      case 1:
        return(release * square_w(ramp));
        break;
      case 2:
        return(release * sawtooth_w(ramp)); 
        break;
      case 3:
        return(release * triangle_w(ramp));
        break;
    }
  }
  else 
    return(note_on);

}

sample_t sine_w(sample_t ramp) {
  return (volume * max_amplitude * GAIN_SIN * sin(2*M_PI*ramp));
}

sample_t square_w(sample_t ramp) {
  int k;
  sample_t x = 0;

  for (k=1; k <= fi; k++) 
    x += (sin(2*M_PI*ramp*(2*k -1))/(2*k -1));

  return(volume * max_amplitude * GAIN_SQR * 4*x/M_PI);
}

sample_t sawtooth_w(sample_t ramp) {
  int k;
  sample_t x = 0;

  for (k=1; k <= fi; k++)
    x += pow(-1,k)/k * sin(2*M_PI*ramp*k); 

  return(GAIN_SAW * volume * max_amplitude * x *(-2)/M_PI);
}

sample_t triangle_w(sample_t ramp) {
  int k;
  sample_t x = 0;

  for (k=0; k < fi; k++)
    x += pow(-1,k) * sin(2*M_PI*ramp*(2*k+1))/(2*k+1)/(2*k+1); 

  return(GAIN_TRI * volume * max_amplitude * x * 8 / M_PI / M_PI);
}

void calc_note_frqs(sample_t *note_frqs, sample_t srate) {
  int i;
  for(i=0; i<128; i++)
    note_frqs[i] = (TUNING / 32.0) * pow(2, (((sample_t)i - 9.0) / 12.0)) / srate;
}
