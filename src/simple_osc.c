/*
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
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <jack/jack.h>
#include <jack/midiport.h>

#define FORMS 4 /* number of implemented waveforms */
#define TUNING 440.0 

/* global vars */

typedef jack_default_audio_sample_t sample_t;

jack_port_t *in_p;
jack_port_t *out_p;

sample_t ramp = 0.0;
sample_t note_on;
sample_t note_frqs[128];
double max_amplitude = 0.5;
unsigned char note = 0; 
char waveform = 'a';
int fi = 20; /* fourier iterations */

/* function declaration */

int check_waveform(char);
int process(jack_nframes_t, void*);
int srate(jack_nframes_t, void*);
void set_waveform();
void set_fi();
void print_help_message();
void calc_note_frqs(sample_t); 
void jack_shutdown(void *); 
void shell_loop(char *);

/* function definition */

int check_waveform(char c) {
  char valid_forms[FORMS];
  int i, v;
  for (i=0; i < FORMS; i++) 
    valid_forms[i] = (i + 'a');
  for (i=0; i < FORMS && c != valid_forms[i]; i++); 
  return (i < FORMS); 
}

void set_waveform() {
  char name[10];
  char c;

  while(getchar() != '\n');

  printf("Select waveform: \n"
         "a) sine\n"
         "b) square\n"
         "c) sawtooth\n"
         "d) triangle\n"
         "waveform: ");

  c = getchar();
  while (!check_waveform(c)) {
    printf("invalid waveform\n");
    while(getchar() != '\n');
    printf("waveform: ");
    scanf("%c", &c);
  }

  waveform = c;
  switch(c) {
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
}

void set_fi() {
  do { 
    printf("N° of fourier iterations: ");
    scanf("%d", &fi);
  }  
  while (fi < 1); 
}

void set_amplitude() {
  printf("Set new max amplitude: ");
  scanf("%lf", &max_amplitude);
  printf("New amplitude = %.2lf\n",  max_amplitude);
  if (max_amplitude < 0)
    printf("A < 0 => Inverted Phase\n"); 
}

void print_help_message() {
  printf("h -> help\n"
         "W -> change waveform\n" 
         "A -> set amplitude (volume)\n"
         "i -> set number of fourier iterations\n"
         "The number of fourier iterations is related to the precision of the waveform... more is higher, and more cpu power is needed... you can set at around 20 or higher if you want (will your ear notice the difference?).\n"); 
}

void shell_loop(char *name) {
  char c;
  printf("Hi! if you need help, type h\n"
         "To close the synth, type q or ^D\n%s: ", name);

  /* shell loop */
  while((c = getchar()) != 'q' && c != EOF) 
  {
      switch(c)
      {
        case 'A':
          set_amplitude();
          break;
        case 'W':
          set_waveform();
          break;
        case 'h':
          print_help_message();
          break;
        case 'i':
          set_fi();
        case '\n':
        case ' ':
        case '\t':
          break;
        default:
          printf("invalid input\n");
          break;
      }
      if (c != '\n' && c != '\t' && c != ' ')
        printf("%s: ", name);
  }
}

void calc_note_frqs(sample_t srate) {
  int i;
  for(i=0; i<128; i++)
    note_frqs[i] = (TUNING / 16.0) * pow(2, (((sample_t)i - 9.0) / 12.0)) / srate;
}

int process(jack_nframes_t nframes, void *arg) {
  int i;
  void* port_buf = jack_port_get_buffer(in_p, nframes);
  sample_t *out = (sample_t *) jack_port_get_buffer (out_p, nframes);
  jack_midi_event_t in_event;
  jack_nframes_t event_index = 0;
  jack_nframes_t event_count = jack_midi_get_event_count(port_buf);
  
  /*if(event_count > 1)
  {
     printf(" midisine: have %d events\n", event_count); 
    for(i=0; i<event_count; i++)
    {
      jack_midi_event_get(&in_event, port_buf, i);
       printf("    event %d time is %d. 1st byte is 0x%x\n", i, in_event.time, *(in_event.buffer)); 
    }
    printf("1st byte of 1st event addr is %p\n", in_events[0].buffer);
  } */ 

  jack_midi_event_get(&in_event, port_buf, 0);
  for(i=0; i<nframes; i++) {
    if((in_event.time == i) && (event_index < event_count)) {
      if( ((*(in_event.buffer) & 0xf0)) == 0x90 ) {
        /* note on */
        note = *(in_event.buffer + 1);
        note_on = 1;
      }
      else if( ((*(in_event.buffer)) & 0xf0) == 0x80 ) {
        /* note off */
        note = *(in_event.buffer + 1);
        note_on = 0;
      }
      event_index++;
      if(event_index < event_count)
        jack_midi_event_get(&in_event, port_buf, event_index);
    }
    if (note_on) {
      ramp += note_frqs[note];
      ramp = (ramp > 1.0) ? ramp - 2.0 : ramp;
      double x = 0;
      int k;
      switch(waveform) {
        case 'a':
          out[i] = max_amplitude * note_on*sin(2*M_PI*ramp);
          break;
        case 'b':
          for (k=1; k <= fi; k++) 
            x += (sin(2*M_PI*ramp*(2*k -1))/(2*k -1));
          out[i] = max_amplitude * note_on*4*x/M_PI;
          break;
        case 'c':
          for (k=1; k < fi; k++)
            x += pow(-1,k)/k * sin(2*M_PI*ramp*k); 
          out[i] = note_on * max_amplitude * x *2/M_PI;
        case 'd':
          for (k=1; k < fi; k++)
            x += pow(-1,k)/k * sin(2*M_PI*ramp*(2*k+1))/(2*k+1)/(2*k+1); 
          out[i] = note_on * max_amplitude * 8/M_PI/M_PI * 60 * x;
      }
    }
    else 
      out[i] = note_on;
  }
  return 0;      
}

int srate(jack_nframes_t nframes, void *arg) {
  printf("Sample Rate = %" PRIu32 "/sec\n", nframes);
  calc_note_frqs((sample_t)nframes);
  return 0;
}

void jack_shutdown(void *arg) {
  exit(1);
}

int main(int argc, char **argv) {

  jack_client_t *client;
  char c, name[11];

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
  
  calc_note_frqs(jack_get_sample_rate(client));
  jack_set_process_callback(client, process, 0);
  jack_set_sample_rate_callback(client, srate, 0);
  jack_on_shutdown(client, jack_shutdown, 0);

  in_p = jack_port_register(client, "Midi IN", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
  out_p = jack_port_register(client, "Audio OUT", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

  if (jack_activate(client)) {
    fprintf(stderr, "cannot activate client");
    return 1;
  }

  shell_loop(name);
  jack_client_close(client);
  printf("Bye!\n");
  return 0;
}
