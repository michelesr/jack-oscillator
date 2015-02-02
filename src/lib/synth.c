/*  synth.c implementation of wave synthesizer
 
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

#include <math.h>
#include <jack/jack.h>
#include "data.h"
#include "notes.h"

/* frequence of A4 */
#define TUNING 440.0 

/* gain adjustment for waveforms */
#define GAIN_SIN 1.0
#define GAIN_SQR 0.85
#define GAIN_SAW 0.885
#define GAIN_TRI 1.01
#define ADSR_NULL -1.0

/* bending macro */
#define BENDING (bender * frq * (bender < 0 ? 0.5 : 1))

typedef jack_default_audio_sample_t sample_t;

/* global vars */
sample_t ramp = 0.0;
sample_t note_on = 0;
/*unsigned char note = 0, old_note = 0;*/
note_t note = NOTE_ZERO;
note_t old_note = NOTE_ZERO;
sample_t attack, decay, release;

/* function declaration */
void calc_note_frqs(sample_t *, sample_t); 
sample_t sine_w(sample_t);
sample_t square_w(sample_t);
sample_t sawtooth_w(sample_t);
sample_t triangle_w(sample_t);
sample_t generate_wave(sample_t *, jack_nframes_t);
sample_t adsr_envelope(sample_t *, jack_nframes_t);
void set_note(note_t);
void set_old_note(unsigned char);
void set_note_on();
void set_note_off();
void adsr_init();
void adsr_reset();

/* function definition */

void set_note(note_t n) {
  note = n;
}

void set_old_note(unsigned char id) {
  old_note.id = id;
  old_note.vel = note.vel;
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

sample_t adsr_envelope(sample_t *note_frqs, jack_nframes_t sr) {

  sample_t out, frq;

  if (note_on) {
    frq = note_frqs[note.id];
    ramp += frq + BENDING;
    ramp = (ramp > 1.0) ? ramp - 2.0 : ramp;
    if (attack < attack_amplitude) {
      attack += (attack_amplitude/(sr*attack_time/1000));
      out = attack;
    }
    else if ((decay > sustain) && (sustain <= attack_amplitude)) {
      decay -= (attack_amplitude - sustain)/(sr*decay_time/1000);
      out = decay;
    }
    else
      out = sustain;
    out *= note.vel / 127.0;
  }

  else if (release > 0) {
    frq = note_frqs[old_note.id];
    ramp += frq + BENDING;
    ramp = (ramp > 1.0) ? ramp - 2.0 : ramp;
    release -= (sustain/(sr*release_time/1000)); 
    out = release;
    out *= old_note.vel / 127.0;
  }

  else
    out = (sample_t) ADSR_NULL;

  return out;
}

sample_t generate_wave(sample_t *note_frqs, jack_nframes_t sr) {
  sample_t envelope, out;

  if((envelope = adsr_envelope(note_frqs, sr)) != ADSR_NULL) {
    switch(waveform) {
      case 0:
        out = sine_w(ramp);
        break;
      case 1:
        out = square_w(ramp);
        break;
      case 2:
        out = sawtooth_w(ramp); 
        break;
      case 3:
        out = triangle_w(ramp);
        break;
    }
  }
  else 
    out = (sample_t)0.0;
  return (envelope * out);

}

/* natural sine */
sample_t sine_w(sample_t ramp) {
  return (volume * max_amplitude * GAIN_SIN * sin(M_PI*ramp));
}


/* we define square wave using if-else */
sample_t square_w(sample_t ramp) {
  sample_t y;

  if (ramp < 0)
    y = -1.0;
  else if (ramp > 0)
    y = 1.0;
  else
    y = 0;

  return (GAIN_SQR * volume * max_amplitude * y);
}

/* ramp generate itself a sawtooth wave so we just scale */
sample_t sawtooth_w(sample_t ramp) {
  return(GAIN_SAW * volume * max_amplitude * ramp);
}

/* additive synthesis */
sample_t triangle_w(sample_t ramp) {
  return (GAIN_TRI * volume * max_amplitude * (1 - 2*fabs(ramp)));
}

void calc_note_frqs(sample_t *note_frqs, sample_t srate) {
  int i;
  for(i=0; i<128; i++)
    note_frqs[i] = (TUNING / 16.0) * pow(2, (((sample_t)i - 9.0) / 12.0)) / srate;
}
