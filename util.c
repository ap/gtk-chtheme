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

#include <gtk/gtk.h>
#include "util.h"

GtkWidget* unfocussable(GtkWidget *w)
{
	GTK_WIDGET_UNSET_FLAGS(w, GTK_CAN_FOCUS);
	return w;
}

GtkWidget* gtk_toggle_button_new_from_stock(const gchar *stock_id)
{
	GtkWidget* button = gtk_toggle_button_new_with_label(stock_id);
	gtk_button_set_use_stock(GTK_BUTTON(button), TRUE);
	return button;
}

GtkLabel* gtk_label_new_from_pango_markup(const char *markup_text, GtkJustification jtype)
{
	PangoAttrList *attrs;
	gchar* parsed_text;
	GtkLabel* label;
	
	pango_parse_markup(markup_text, -1, 0, &attrs, &parsed_text, NULL, NULL);

	label = GTK_LABEL(gtk_label_new(parsed_text));
	gtk_label_set_attributes(label, attrs);
	gtk_label_set_line_wrap(label, TRUE);
	gtk_label_set_justify(label, jtype);

	return label;
}
