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

#include "stock.h"
#include "themeutil.h"
#include "theme.h"
#include "demo.h"
#include "about.h"

static void ok_clicked(GtkWidget *widget, gpointer u)
{
	if(!get_selected_theme()) return;
	write_rc_file(get_selected_theme(), get_newfont(), FALSE);
	GdkEventClient event = { GDK_CLIENT_EVENT, NULL, TRUE, gdk_atom_intern("_GTK_READ_RCFILES", FALSE), 8 };
	gdk_event_send_clientmessage_toall((GdkEvent*) &event);
	gtk_main_quit();
}

int main(int argc, char *argv[])
{
	gtk_init (&argc, &argv);

	init_theme_list(g_strconcat(homedir(), "/.themes", NULL));
	init_theme_list(gtk_rc_get_theme_dir());

	init_new_stock_items();

	GtkWidget *mainwin = gtk_dialog_new();
	gtk_widget_realize(mainwin);
	gtk_window_set_title(GTK_WINDOW(mainwin), PROJNAME);
	gtk_window_set_resizable(GTK_WINDOW(mainwin), TRUE);
	gtk_window_set_default_size(GTK_WINDOW(mainwin), 360, 450);
	g_signal_connect(G_OBJECT(mainwin), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget *notebook = gtk_notebook_new();
	gtk_container_set_border_width(GTK_CONTAINER(notebook), 2);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), theme_page(), gtk_label_new("Theme"));
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), demo_page(),  gtk_label_new("Demo"));
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), about_page(), gtk_label_new("About"));
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(mainwin)->vbox), notebook);

	GtkWidget *ok_button = gtk_button_new_from_stock(GTK_STOCK_OK);
	g_signal_connect(G_OBJECT(ok_button), "clicked", G_CALLBACK(ok_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->action_area), ok_button, TRUE, TRUE, 0);
	gtk_widget_set_sensitive(ok_button, FALSE);
	await_activation(ok_button);

	GtkWidget *cancel_button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	g_signal_connect(G_OBJECT(cancel_button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->action_area), cancel_button, TRUE, TRUE, 0);

	gtk_widget_show_all(mainwin);

	gtk_main();

	cleanup_temporary();

	return 0;
}
