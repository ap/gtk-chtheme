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
#include "font_sel.h"
#include "main.h"

GtkWidget *font_entry;

static void font_dlg_ok_clicked(GtkWidget *widget, gboolean *flag)
{
	*flag = TRUE;
}

static void font_browse_clicked(GtkWidget *widget, gpointer u)
{
	const gchar *curfont = NULL;
	gboolean is_confirmed = FALSE;

	GtkFontSelectionDialog *font_dlg =
		GTK_FONT_SELECTION_DIALOG(gtk_font_selection_dialog_new("Select Font"));

	gtk_font_selection_dialog_set_preview_text(font_dlg, "Sphinx of Black Quartz, Judge My Vow");

	curfont = gtk_entry_get_text(GTK_ENTRY(font_entry));

	if (g_utf8_strlen(curfont, -1))
		gtk_font_selection_dialog_set_font_name(font_dlg, curfont);

	g_signal_connect(G_OBJECT(font_dlg->ok_button), "clicked",
		G_CALLBACK(font_dlg_ok_clicked), &is_confirmed);

	gtk_dialog_run(GTK_DIALOG(font_dlg));

	if(is_confirmed)
		set_font(gtk_font_selection_dialog_get_font_name(font_dlg), TRUE);

	gtk_widget_destroy(GTK_WIDGET(font_dlg));
}

static void font_clear_clicked(GtkWidget *widget, gpointer u)
{
	set_font(NULL, TRUE);
}

static void font_entry_set_from_style(GtkWidget *w, GtkStyle *prev, gpointer u)
{
	gchar *font = get_font();
	gtk_entry_set_text(GTK_ENTRY(font_entry), font ? font : "");
}

GtkWidget *create_font_sel(void)
{
	GtkWidget *box = gtk_hbox_new(FALSE, 0);
	GtkWidget *button;

	button = gtk_button_new_from_stock(GTK_STOCK_SELECT_FONT);
	await_activation(button);
	g_signal_connect(G_OBJECT(button), "clicked",
		G_CALLBACK(font_browse_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

	font_entry = gtk_entry_new();
	gtk_editable_set_editable(GTK_EDITABLE(font_entry), FALSE);
	gtk_box_pack_start(GTK_BOX(box), font_entry, TRUE, TRUE, 3);

	g_signal_connect(G_OBJECT(font_entry), "style-set",
		G_CALLBACK(font_entry_set_from_style), NULL);

	button = gtk_button_new_from_stock(GTK_STOCK_CLEAR);
	await_activation(button);
	g_signal_connect(G_OBJECT(button), "clicked",
		G_CALLBACK(font_clear_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

	return box;
}
