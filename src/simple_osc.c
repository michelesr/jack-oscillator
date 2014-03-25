/*  simple_osc.c main() implementation source file
 
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

/* standard libs */
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* jack libs */
#include <jack/jack.h>
#include <jack/midiport.h>

/* program libs */
#include "lib/shell_ui.h"
#include "lib/synth.h"
#include "lib/controller.h"

/* global vars */

jack_port_t *in_p;
jack_port_t *out_p;
jack_nframes_t sr;

sample_t ramp = 0.0;
sample_t note_on = 0;
sample_t note_frqs[128];
sample_t attack = 0, decay, release = 0;
unsigned char note = 0, active_notes[128];

/* function declaration */

int process(jack_nframes_t, void*);
int srate(jack_nframes_t, void*);
int note_is_active(unsigned char);
int search_active_note(unsigned char);
int active_notes_is_empty();
unsigned char search_highest_active_note();
void jack_shutdown(void *); 
void add_active_note(unsigned char );
void del_active_note(unsigned char ); 

/* function definition */

int process(jack_nframes_t nframes, void *arg) {
int i;
unsigned char c = 0;
static unsigned char n = 0;
void* port_buf = jack_port_get_buffer(in_p, nframes);
sample_t *out = (sample_t *) jack_port_get_buffer (out_p, nframes);
jack_midi_event_t in_event;
jack_nframes_t event_index = 0;
jack_nframes_t event_count = jack_midi_get_event_count(port_buf);

jack_midi_event_get(&in_event, port_buf, 0);


for(i=0; i<nframes; i++) {
  
  /* check channel */
  if ((event_index < event_count) && 
      ((*(in_event.buffer) & 0x0f) == (channel-1)))  {
    /* inspect event if is in time (try without checking the time) */
    if(/*(in_event.time == i) &&*/ (event_index < event_count)) {

      /* note on event */
      if( ((*(in_event.buffer) & 0xf0)) == 0x90 ) {

        /* get note from jack buffer */
        c = (*(in_event.buffer + 1));
        /* add to our buffer */
        add_active_note(c);
        /* play highest note in buffer */
        note = search_highest_active_note();
        note_on = 1;
      }

      /* note off event */
      else if( (((*(in_event.buffer)) & 0xf0) == 0x80)) {
        /* get note from jack buffer */
        c = (*(in_event.buffer + 1));
        /* remove from active notes */
        del_active_note(c);
        if (active_notes_is_empty()) {
          note_on = 0;
          attack = 0;
          decay = attack_amplitude;
          release = sustain;
          n = c;
        }
        else
          note = search_highest_active_note();
      }

      /* program change */
      else if( (((*(in_event.buffer)) & 0xf0) == 0xc0)) 
        handle_midi_program_change(in_event);

      /* control event */
      else if( (((*(in_event.buffer)) & 0xf0) == 0xb0)) 
        handle_midi_control(in_event);

    }
  }

    event_index++;
    if(event_index < event_count)
      jack_midi_event_get(&in_event, port_buf, event_index);


  if (note_on) {
    ramp += note_frqs[note];
    ramp = (ramp > 1.0) ? ramp - 2.0 : ramp;
    if (attack_time > 0 && attack < attack_amplitude) 
      attack += (attack_amplitude/(sr*attack_time/1000));
    else if (decay_time > 0 && decay > (sustain/attack_amplitude)) 
      decay -= (sustain/(sr*decay_time/1000));
    switch(waveform) {
      case 0:
        out[i] = attack * decay * sine_w(ramp);
        break;
      case 1:
        out[i] = attack * decay * square_w(ramp);
        break;
      case 2:
        out[i] = attack * decay * sawtooth_w(ramp); 
        break;
      case 3:
        out[i] = attack * decay * triangle_w(ramp);
        break;
    }
  }
  else if (release > 0) {
    ramp += note_frqs[n];
    ramp = (ramp > 1.0) ? ramp - 2.0 : ramp;
    release -= (sustain/(sr*release_time/1000)); 
    switch(waveform) {
      case 0:
        out[i] = release * sine_w(ramp);
        break;
      case 1:
        out[i] = release * square_w(ramp);
        break;
      case 2:
        out[i] = release * sawtooth_w(ramp); 
        break;
      case 3:
        out[i] = release * triangle_w(ramp);
        break;
    }
  }
  else 
    out[i] = note_on;
}
return 0;      
}

int srate(jack_nframes_t nframes, void *arg) {
  printf("Sample Rate = %" PRIu32 "/sec\n", nframes);
  sr = nframes;
  calc_note_frqs(note_frqs, (sample_t)nframes);
  return 0;
}

void jack_shutdown(void *arg) {
  exit(1);
}

int main(int argc, char **argv) {

  jack_client_t *client;
  char c, name[11];
  int i;

  if (argc < 2) {
    printf("Type client name (max 10 char): ");
    scanf("%s", name); 
  }
  else 
    strcpy(name, argv[1]);

  if (!(client = jack_client_open(name, JackNullOption, NULL))) {
    fprintf(stderr, "jack server not running?\n");
    return 1;
  }

  /* initialize array */
  for(i = 0; i <= 128; i++) {
    active_notes[i] = 255;
  }

  decay = attack_amplitude; 

  jack_set_process_callback(client, process, 0);
  jack_set_sample_rate_callback(client, srate, 0);
  jack_on_shutdown(client, jack_shutdown, 0);

  in_p = jack_port_register(client, "in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
  out_p = jack_port_register(client, "out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

  if (jack_activate(client)) {
    fprintf(stderr, "cannot activate client");
    return 1;
  }

  shell_loop(name);
  jack_client_close(client);
  printf("Bye!\n");
  return 0;
}

void add_active_note(unsigned char note)
{
    static int i = 0;
    if (!note_is_active(note)) {
      active_notes[i] = note;
      i = ((i+1)%128);
    }
}

void del_active_note(unsigned char note) {
  active_notes[search_active_note(note)] = 255; 
}

int note_is_active(unsigned char note) {
  return(search_active_note(note) < 128);
}

int search_active_note(unsigned char note) {
  int i;
  for (i=0; (i < 128) && (active_notes[i] != note); i++);
  return (i);
}

unsigned char search_highest_active_note() {
  char c=-1;
  int i;
  for (i=0; i < 128; i++) {
    if ((c < active_notes[i]) && (active_notes[i] != 255))
      c = active_notes[i];
  }
  if (c == -1)
    return ((unsigned char) 255);
  else 
    return ((unsigned char) c);
}

int active_notes_is_empty() {
  return (search_highest_active_note() == 255);
}
