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
#include "about.h"
#include "stock.h"

#define LPROJNAME "<b>" PROJNAME "</b>"

#define ABOUT \
	"<big><big><span weight=\"heavy\">" LPROJNAME " " VERSION "</span></big></big>\n" \
	"by Aristotle Pagaltzis <tt>&lt;pagaltzis@gmx.de&gt;</tt>\n\n" \
	"<tt><small><u>http://plasmasturm.org/</u></small></tt>"

#define ABOUT_ORIGINAL \
	"<small><small>based on</small>\n" \
	"<span weight=\"heavy\">Gtk Theme Switch</span>\n" \
	"<small>by Maher Awamy and Aaron Lehmann</small></small>"

#define LICENSE \
	"<span size=\"x-small\">" \
	LPROJNAME " is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2, or (at your option) any later version.\n\n" \
	LPROJNAME " is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." \
	"See the GNU General Public License for more details.\n\n" \
	"You should have received a copy of the GNU General Public License along with " LPROJNAME "; see the file COPYING." \
	"If not, write to the Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA." \
	"</span>"

GtkLabel* gtk_label_new_from_pango_markup(const char *markup_text, GtkJustification jtype)
{
	PangoAttrList *attrs;
	gchar* parsed_text;
	
	pango_parse_markup(markup_text, -1, 0, &attrs, &parsed_text, NULL, NULL);

	GtkLabel* label = GTK_LABEL(gtk_label_new(parsed_text));
	gtk_label_set_attributes(label, attrs);
	gtk_label_set_line_wrap(label, TRUE);
	gtk_label_set_justify(label, jtype);

	return label;
}

GtkWidget* about_page(void) {
	GtkWidget* box = gtk_vbox_new(FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(box), 20);

	gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(gtk_label_new_from_pango_markup((ABOUT), GTK_JUSTIFY_CENTER)), TRUE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(box), gtk_hseparator_new(), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(gtk_label_new_from_pango_markup((ABOUT_ORIGINAL), GTK_JUSTIFY_CENTER)), TRUE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(box), gtk_hseparator_new(), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(gtk_label_new_from_pango_markup((LICENSE), GTK_JUSTIFY_FILL)), TRUE, FALSE, 10);

	return box;
}
