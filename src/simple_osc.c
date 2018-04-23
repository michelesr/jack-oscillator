/*  simple_osc.c main() implementation source file

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

sample_t note_frqs[128];

/* function declaration */

int process(jack_nframes_t, void *);
int srate(jack_nframes_t, void *);
void jack_shutdown(void *);

/* function definition */

int process(jack_nframes_t nframes, void *arg) {

  int i;
  note_t n = NOTE_ZERO;
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

      switch (*(in_event.buffer) & 0xf0) {
        /* note on */
        case 0x90:
          /* get note from jack buffer */
          n.id = (*(in_event.buffer + 1));
          /* get velocity */
          n.vel = (*(in_event.buffer + 2));
          /* add to our buffer */
          add_active_note(n);
          /* play highest note in buffer */
          set_note(search_highest_active_note());
          set_note_on();
          break;
        /* note off */
        case 0x80:
          /* get note from jack buffer */
          n.id = (*(in_event.buffer + 1));
          /* get note velocity and remove from buffer */
          n.vel = del_active_note(n.id);
          if (active_notes_is_empty()) {
            set_note_off();
            adsr_reset();
            set_old_note(n);
          }
          else
            set_note(search_highest_active_note());
          break;
        /* controller midi */
        case 0xb0:
          handle_midi_control(in_event);
          break;
        /* program/bank change */
        case 0xc0:
          handle_midi_program_change(in_event);
          break;
        /* pitchbend */
        case 0xe0:
          handle_midi_bending(in_event);
          break;
      }
    }

    event_index++;
    if(event_index < event_count)
      jack_midi_event_get(&in_event, port_buf, event_index);

    out[i] = generate_wave(note_frqs,sr);

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
  char name[11];

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

  active_notes_init();
  adsr_init();

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
  return 0;
}

