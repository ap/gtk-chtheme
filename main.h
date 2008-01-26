/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef ___MAIN_H
#define ___MAIN_H 1

void read_theme_list(gchar * dirname);
void populate_themelist(GHFunc func, gpointer user_data);

void await_activation (GtkWidget* w);
void set_font(gchar * newfont, gboolean is_preview);
gchar* get_font(void);
void set_theme(gchar * newthemename, gboolean is_preview);
gchar* get_theme(void);
void apply_new_look(gboolean is_preview);

#endif
