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

#include <glib.h>
#include <gtk/gtk.h>
#include "main.h"
#include "stock.h"
#include "theme_sel.h"
#include "font_sel.h"
#include "preview_pane.h"
#include "about_dialog.h"

static void response(GtkWidget *widget, gint response, gpointer u)
{
	switch(response) {
		case GTK_RESPONSE_HELP:
			show_about_dialog();
			break;
		case GTK_RESPONSE_APPLY:
			apply_new_look(FALSE);
			break;
		case GTK_RESPONSE_OK:
			apply_new_look(FALSE);
		case GTK_RESPONSE_CANCEL:
		case GTK_RESPONSE_NONE:
			gtk_main_quit();
	}
}

GtkWidget* create_mainwin(void)
{
	GtkWidget *mainwin = gtk_dialog_new_with_buttons(
		PROJNAME, NULL, 0,
		GTK_STOCK_ABOUT, GTK_RESPONSE_HELP,
		GTK_STOCK_APPLY, GTK_RESPONSE_APPLY,
		GTK_STOCK_OK, GTK_RESPONSE_OK,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		NULL
	);

	gtk_widget_realize(mainwin);
	gtk_window_set_resizable(GTK_WINDOW(mainwin), TRUE);
	gtk_window_set_default_size(GTK_WINDOW(mainwin), 360, 450);
	g_signal_connect(G_OBJECT(mainwin), "response", G_CALLBACK(response), NULL);
	g_signal_connect(G_OBJECT(mainwin), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->vbox), create_preview_pane(), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->vbox), create_theme_sel(), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->vbox), create_font_sel(), FALSE, FALSE, 0);

	return mainwin;
}
