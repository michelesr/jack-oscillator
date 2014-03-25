/*  shell_ui.c implementation of textual user interface
 
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
#include <string.h>
#include "synth.h"
#include "data.h" 

/* function declaration */

void shell_loop(char *);
void ui_set_waveform();
void ui_set_fi();
void print_help_message();
void ui_set_amplitude();
void ui_set_channel();
void ui_set_attack();
void ui_set_decay();
void ui_set_sustain();
void ui_set_relase();

/* function definition */

void ui_set_channel() {
  unsigned short c;
  printf("select channel from 1 to 16: ");
  do {
    scanf("%d", &c);
  }
  while (!set_channel(c));
}

void ui_set_attack() {
  int at;
  sample_t aa;
  do { 
    printf("attack time (ms): ");
    scanf("%d", &at);
  } 
  while(!set_attack_time(at));

  do {
    printf("attack amplitude: ");
    scanf("%f", &aa);
  } 
  while(!set_attack_amplitude(aa));
}

void ui_set_decay() {
  int d;
  do {
    printf("decay time(ms): ");
    scanf("%d", &d);
  }
  while(!set_decay(d));
}

void ui_set_sustain() {
  sample_t s;
  do {
    printf("sustain: ");
    scanf("%f", &s);
  }
  while(!set_sustain(s));
}

void ui_set_release() {
  int r;
  do {
    printf("release time(ms): ");
    scanf("%d", &r);
  }
  while(!set_release(r));
}

void ui_set_waveform() {
  char c;

  while(getchar() != '\n');

  printf("Select waveform: \n"
         "a) sine\n"
         "b) square\n"
         "c) sawtooth\n"
         "d) triangle\n"
         "waveform: ");

  c = getchar();
  while (!set_waveform(c)) {
    while(getchar() != '\n');
    printf("waveform: ");
    scanf("%c", &c);
  }
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
          ui_set_amplitude();
          break;
        case 'a':
          ui_set_attack();
          break;
        case 'd':
          ui_set_decay();
          break;
        case 's':
          ui_set_sustain();
          break;
        case 'r':
          ui_set_release();
          break;
        case 'W':
          ui_set_waveform();
          break;
        case 'c':
          ui_set_channel();
          break;
        case 'h':
          print_help_message();
          break;
        case 'i':
          ui_set_fi();
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

void ui_set_fi() {
  int f;
  do { 
    printf("N° of harmonics: ");
    scanf("%d", &f);
  }  
  while (!set_fi(f)); 
}

void ui_set_amplitude() {
  int a;
  do {
    printf("Set new max amplitude: ");
    scanf("%f", &a);
  }
  while(!set_amplitude(a));
}

void print_help_message() {
  printf("h -> help\n"
         "c -> change channel\n"
         "W -> change waveform\n" 
         "a -> set attack time and peak\n"
         "d -> set decay time\n"
         "s -> set sustain\n"
         "r -> set release time\n"
         "A -> set amplitude (volume)\n"
         "i -> set number of harmonics\n"
         "The number of harmonics is related to the precision of the waveform... more is higher, and more cpu power is needed... you can set at around 20 or higher if you want (will your ear notice the difference?).\n"); 
}
