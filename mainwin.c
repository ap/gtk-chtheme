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

static void about_clicked(GtkWidget *widget, gpointer u)
{
	show_about_dialog();
}

static void apply_clicked(GtkWidget *widget, gpointer u)
{
	apply_new_look(FALSE);
}

static void ok_clicked(GtkWidget *widget, gpointer u)
{
	apply_new_look(FALSE);
	gtk_main_quit();
}

GtkWidget* create_mainwin(void)
{
	GtkWidget *mainwin = gtk_dialog_new();
	gtk_widget_realize(mainwin);
	gtk_window_set_title(GTK_WINDOW(mainwin), PROJNAME);
	gtk_window_set_resizable(GTK_WINDOW(mainwin), TRUE);
	gtk_window_set_default_size(GTK_WINDOW(mainwin), 360, 450);
	g_signal_connect(G_OBJECT(mainwin), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->vbox), create_preview_pane(), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->vbox), create_theme_sel(), TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->vbox), gtk_hseparator_new(), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->vbox), create_font_sel(), FALSE, FALSE, 0);

	GtkWidget* about_button = gtk_button_new_from_stock(GTK_STOCK_ABOUT);
	g_signal_connect(G_OBJECT(about_button), "clicked", G_CALLBACK(about_clicked), NULL);
	gtk_box_pack_end(GTK_BOX(GTK_DIALOG(mainwin)->action_area), about_button, TRUE, TRUE, 0);

	GtkWidget* apply_button = gtk_button_new_from_stock(GTK_STOCK_APPLY);
	g_signal_connect(G_OBJECT(apply_button), "clicked", G_CALLBACK(apply_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->action_area), apply_button, TRUE, TRUE, 0);
	await_activation(apply_button);

	GtkWidget* ok_button = gtk_button_new_from_stock(GTK_STOCK_OK);
	g_signal_connect(G_OBJECT(ok_button), "clicked", G_CALLBACK(ok_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->action_area), ok_button, TRUE, TRUE, 0);
	await_activation(ok_button);

	GtkWidget *cancel_button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	g_signal_connect(G_OBJECT(cancel_button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->action_area), cancel_button, TRUE, TRUE, 0);

	return mainwin;
}
