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
#include <string.h>
#include "theme_sel.h"
#include "main.h"

GtkWidget *theme_list = NULL;

static gboolean theme_list_focus(GtkWidget *w)
{
	gtk_widget_grab_focus(w);
	return FALSE;
}

static gboolean theme_list_itemselected(GtkTreeSelection * sel,
	GtkTreeModel * model, GtkTreePath * path, gboolean is_selected, gpointer u)
{
	GtkTreeIter iter;
	gchar *newtheme;

	if (!is_selected && gtk_tree_model_get_iter(model, &iter, path))
	{
		gtk_tree_model_get(model, &iter, 0, &newtheme, -1);
		set_theme(newtheme, TRUE);
	}

	return TRUE;
}

static void insert_key_in_liststore(gchar * key, gchar * value,
	GtkListStore * store)
{
	GtkTreeIter iter;

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, key, -1);
}

GtkWidget *create_theme_sel(void)
{
	GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);

	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(store), 0,
		GTK_SORT_ASCENDING);

	populate_themelist((GHFunc) insert_key_in_liststore, store);

	theme_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(theme_list), TRUE);
	gtk_tree_view_set_search_column(GTK_TREE_VIEW(theme_list), 0);

	GtkTreeSelection *selection =
		gtk_tree_view_get_selection(GTK_TREE_VIEW(theme_list));

	gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);
	gtk_tree_selection_set_select_function(selection,
		(GtkTreeSelectionFunc) theme_list_itemselected, NULL, NULL);

	if (get_theme())
	{
		GtkTreeIter iter;
		gchar *newtheme;

		gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter);

		do
		{
			gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &newtheme, -1);
			if (strcmp(newtheme, get_theme()) != 0)
				continue;

			GtkTreePath *p = gtk_tree_model_get_path(GTK_TREE_MODEL(store), &iter);

			gtk_tree_view_set_cursor(GTK_TREE_VIEW(theme_list), p, NULL, FALSE);
			gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(theme_list), p, NULL, TRUE, 0.5, 0);
			break;

		}
		while (gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter));
	}

	g_object_unref(G_OBJECT(store));

	GtkTreeViewColumn *column =
		gtk_tree_view_column_new_with_attributes("Theme",
		gtk_cell_renderer_text_new(), "text", 0, NULL);

	gtk_tree_view_column_set_sort_column_id(column, 0);
	gtk_tree_view_append_column(GTK_TREE_VIEW(theme_list), column);

	GtkWidget *sw = gtk_scrolled_window_new(NULL, NULL);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(sw), theme_list);

	gtk_timeout_add(0, (GtkFunction)theme_list_focus, theme_list);

	return sw;
}
