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

/* number of implemented waveforms */
#define FORMS 4 

double max_amplitude = 0.5; 
int fi = 20;
char waveform = 'a';

int check_waveform(char);
void shell_loop(char *);
void set_waveform();
void set_fi();
void print_help_message();
void set_amplitude();

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
