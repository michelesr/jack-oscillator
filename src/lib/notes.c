/*  notes.c implementation of active notes buffer

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

#include "data.h"
#include "notes.h"

note_t active_notes[128];

bool_t note_is_active(unsigned char);
bool_t active_notes_is_empty();
void add_active_note(note_t);
unsigned char del_active_note(unsigned char );
void active_notes_init();
note_t get_note_by_id(unsigned char);
note_t search_highest_active_note();

void active_notes_init() {
  int i;
  /* initialize array */
  for(i = 0; i < 128; i++) {
    active_notes[i].id = i;
    active_notes[i].vel = 0;
  }
}

void add_active_note(note_t n)
{
    if (!note_is_active(n.id)) {
      active_notes[n.id].vel = n.vel;
    }
}

unsigned char del_active_note(unsigned char id) {
  unsigned char old_vel = active_notes[id].vel;
  active_notes[id].vel = 0;
  return old_vel;
}

bool_t note_is_active(unsigned char id) {
  return(active_notes[id].vel > 0? true: false);
}

note_t get_note_by_id(unsigned char id) {
  return active_notes[id];
}

note_t search_highest_active_note() {
  int i;
  note_t out = NOTE_ZERO;

  // start from the end (highest note) and return the first active note
  for (i=127; i >= 0 && active_notes[i].vel == 0; i--);

  if (i > -1)
    out = get_note_by_id(i);

  return out;
}

bool_t active_notes_is_empty() {
  return ((search_highest_active_note().id == 255)? true: false);
}
