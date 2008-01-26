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

#ifndef ___UTIL_H
#define ___UTIL_H 1

GtkWidget* unfocussable(GtkWidget *w);
GtkWidget* gtk_toggle_button_new_from_stock(const gchar *stock_id);
GtkLabel* gtk_label_new_from_pango_markup(const char *markup_text, GtkJustification jtype);

#endif
