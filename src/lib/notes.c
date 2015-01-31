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
#include "notes.h"

note_t active_notes[128];

bool_t note_is_active(unsigned char);
bool_t active_notes_is_empty();
int search_active_note(unsigned char);
void add_active_note(unsigned char, unsigned char );
void del_active_note(unsigned char );
void active_notes_init();
note_t get_note_by_id(unsigned char);
note_t search_highest_active_note();

void active_notes_init() {
  int i;
  /* initialize array */
  for(i = 0; i < 128; i++) {
    active_notes[i].id = 255;
    active_notes[i].vel = 0;
  }
}

void add_active_note(unsigned char note, unsigned char vel)
{
    static int i = 0;
    if (!note_is_active(note)) {
      active_notes[i].id = note;
      active_notes[i].vel = vel;
      i = ((i+1)%128);
    }
}

void del_active_note(unsigned char note) {
  active_notes[search_active_note(note)].id = 255;
  active_notes[search_active_note(note)].vel = 0;
}

bool_t note_is_active(unsigned char note) {
  return((search_active_note(note) < 128)? true: false);
}

int search_active_note(unsigned char id) {
  int i;
  for (i=0; (i < 128) && (active_notes[i].id != id); i++);
  return (i);
}

note_t get_note_by_id(unsigned char id) {
  return active_notes[search_active_note(id)];
}

note_t search_highest_active_note() {
  int i, c = -1;
  note_t out = NOTE_ZERO;

  for (i=0; i < 128; i++) {
    if ((c < active_notes[i].id) && (active_notes[i].id != 255))
      c = active_notes[i].id;
  }
  
  if (c > -1)
    out = get_note_by_id(c);

  return out;
}

bool_t active_notes_is_empty() {
  return ((search_highest_active_note().id == 255)? true: false);
}
