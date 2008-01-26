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
#include "about_dialog.h"
#include "util.h"
#include "stock.h"

#define LPROJNAME "<b>" PROJNAME "</b>"

#define ABOUT \
	"<big><big><span weight=\"heavy\">" LPROJNAME " " VERSION "</span></big></big>\n" \
	"by Aristotle Pagaltzis <tt>&lt;pagaltzis@gmx.de&gt;</tt>\n\n" \
	"<tt><small><u>http://plasmasturm.org/</u></small></tt>"

#define LICENSE \
	"<span size=\"x-small\">" \
	LPROJNAME " is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2, or (at your option) any later version.\n\n" \
	LPROJNAME " is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." \
	"See the GNU General Public License for more details.\n\n" \
	"You should have received a copy of the GNU General Public License along with " LPROJNAME "; see the file COPYING." \
	"If not, write to the Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA." \
	"</span>"

void show_about_dialog(void) {
	GtkWidget* about_dlg = gtk_dialog_new();
	gtk_window_set_title(GTK_WINDOW(about_dlg), "About");
	gtk_window_set_resizable(GTK_WINDOW(about_dlg), FALSE);
	gtk_dialog_set_has_separator(GTK_DIALOG(about_dlg), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(GTK_DIALOG(about_dlg)->vbox), 20);
	
	GtkWidget* about = GTK_WIDGET(gtk_label_new_from_pango_markup((ABOUT), GTK_JUSTIFY_CENTER));

	GtkWidget* box = gtk_hbox_new(FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(box), 20);
	gtk_box_pack_start(GTK_BOX(box), about, FALSE, FALSE, 10);
	gtk_widget_show_all(box);

	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(about_dlg)->vbox), box, FALSE, FALSE, 10);

	GtkWidget* ok_button = gtk_dialog_add_button(GTK_DIALOG(about_dlg), GTK_STOCK_OK, 0);
	gtk_button_set_use_stock(GTK_BUTTON(ok_button), TRUE);

	gtk_dialog_run(GTK_DIALOG(about_dlg));

	gtk_widget_destroy(about_dlg);
}
