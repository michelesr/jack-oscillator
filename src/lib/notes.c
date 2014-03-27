/*  notes.c implementation of active notes buffer

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

#include "data.h"

unsigned char active_notes[128];

bool_t note_is_active(unsigned char);
bool_t active_notes_is_empty();
int search_active_note(unsigned char);
void add_active_note(unsigned char );
void del_active_note(unsigned char );
void active_notes_init();
unsigned char search_highest_active_note();

void active_notes_init() {
  int i;
  /* initialize array */
  for(i = 0; i <= 128; i++) {
    active_notes[i] = 255;
  }
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

bool_t note_is_active(unsigned char note) {
  return((search_active_note(note) < 128)? true: false);
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

bool_t active_notes_is_empty() {
  return ((search_highest_active_note() == 255)? true: false);
}
