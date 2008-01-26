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
#include "theme.h"
#include "themeutil.h"

GSList *awaiting_activation = NULL;
GtkWidget *font_entry;
gchar
	*newfont        = NULL,
	*selected_theme = NULL;

static void apply_new_look(void)
{
	if(!selected_theme) return;
	gchar* rc = write_rc_file(selected_theme, newfont, TRUE);

	gchar *default_files[] = { rc, NULL };
	gtk_rc_set_default_files(default_files);

	gtk_rc_reparse_all_for_settings(gtk_settings_get_default(), TRUE);
}

static void clear_font(void)
{
	if(newfont) {
		g_free(newfont);
		newfont = NULL;
		gtk_entry_set_text(GTK_ENTRY(font_entry), "");
	}
}

static void font_dialog_ok_clicked(GtkWidget *widget, GtkWidget *dialog)
{
	if (newfont)
		g_free(newfont);

	newfont = gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(dialog));

	gtk_entry_set_text(GTK_ENTRY(font_entry), newfont);
	gtk_widget_destroy(dialog);

	apply_new_look();
}

static void font_browse_clicked (GtkWidget *widget, gpointer u)
{
	GtkWidget *font_dialog = gtk_font_selection_dialog_new("Select Font");

	if(newfont)
		gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(font_dialog), newfont);

	gtk_font_selection_dialog_set_preview_text(GTK_FONT_SELECTION_DIALOG(font_dialog), PROJNAME);

	g_signal_connect(G_OBJECT(GTK_FONT_SELECTION_DIALOG(font_dialog)->ok_button), "clicked", G_CALLBACK(font_dialog_ok_clicked), (gpointer) font_dialog);

 	g_signal_connect_swapped(G_OBJECT(GTK_FONT_SELECTION_DIALOG(font_dialog)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer) font_dialog);

	gtk_widget_show(font_dialog);
}

static void font_clear_clicked (GtkWidget *widget, gpointer u)
{
	clear_font();
	apply_new_look();
}

static gboolean clist_itemselected (GtkTreeSelection* sel, GtkTreeModel* model, GtkTreePath* path, gboolean is_selected, gchar** selected_theme)
{
	GtkTreeIter iter;

	if (!is_selected && gtk_tree_model_get_iter(model, &iter, path)) {
		if(*selected_theme)
			g_free(*selected_theme);

		gtk_tree_model_get(model, &iter, 0, selected_theme, -1);
	}

	if(awaiting_activation)
	{
		g_slist_foreach(awaiting_activation, (GFunc)gtk_widget_set_sensitive, GINT_TO_POINTER(TRUE));
		g_slist_free(awaiting_activation);
		awaiting_activation = NULL;
	}

	clear_font();
	apply_new_look();

	return TRUE;
}

static void insert_key_in_liststore(gchar* key, gchar* value, GtkListStore* store)
{
	GtkTreeIter iter;
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, key, -1);
}

GtkWidget* theme_page(void)
{
	GtkWidget* box = gtk_vbox_new(FALSE, 0);

	GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(store), 0, GTK_SORT_ASCENDING);

	populate_themelist(insert_key_in_liststore, store);

	GtkWidget *clist = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(clist), TRUE);
	gtk_tree_view_set_search_column(GTK_TREE_VIEW(clist), 0);

	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(clist));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);
	gtk_tree_selection_set_select_function(selection, (GtkTreeSelectionFunc)clist_itemselected, &selected_theme, NULL);

	g_object_unref(G_OBJECT(store));

	GtkTreeViewColumn *column =
		gtk_tree_view_column_new_with_attributes("Theme", gtk_cell_renderer_text_new(), "text", 0, NULL);

	gtk_tree_view_column_set_sort_column_id(column, 0);
	gtk_tree_view_append_column(GTK_TREE_VIEW(clist), column);

	GtkWidget *sw = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(sw), clist);
	gtk_box_pack_start(GTK_BOX(box), sw, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(box), gtk_hseparator_new(), FALSE, FALSE, 2);

	{
		GtkWidget *hbox = gtk_hbox_new(FALSE, 0);

		GtkWidget* button = gtk_button_new_from_stock(GTK_STOCK_SELECT_FONT);
		gtk_widget_set_sensitive(button, FALSE);
		await_activation(button);
		g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(font_browse_clicked), NULL);
		gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

		font_entry = gtk_entry_new();
		gtk_editable_set_editable(GTK_EDITABLE(font_entry), FALSE);
		gtk_box_pack_start(GTK_BOX(hbox), font_entry, TRUE, TRUE, 3);

		button = gtk_button_new_from_stock(GTK_STOCK_CLEAR);
		g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(font_clear_clicked), NULL);
		gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

		gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 0);
	}

	return box;
}

void await_activation (gpointer data)
{ awaiting_activation = g_slist_append(awaiting_activation, data); }

gchar* get_newfont (void)
{ return newfont; }

gchar* get_selected_theme (void)
{ return selected_theme; }
