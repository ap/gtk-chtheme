#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <glib.h>
#include <gtk/gtk.h>

#define PRGNAME "Gtk+ 2.0 Change Theme"
#define LPRGNAME "<b>" PRGNAME "</b>"

#define ABOUT \
	"<big><big><span weight=\"heavy\">" LPRGNAME " " VERSION "</span></big></big>\n" \
	"by Aristotle Pagaltzis <tt>&lt;pagaltzis@gmx.de&gt;</tt>\n\n" \
	"<tt><small><u>http://plasmasturm.org/</u></small></tt>"

#define ABOUT_ORIGINAL \
	"<small><small>based on</small>\n" \
	"<span weight=\"heavy\">Gtk Theme Switch</span>\n" \
	"<small>by Maher Awamy and Aaron Lehmann</small></small>"

#define LICENSE \
	"<span size=\"x-small\">" \
	LPRGNAME " is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2, or (at your option) any later version.\n\n" \
	LPRGNAME " is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." \
	"See the GNU General Public License for more details.\n\n" \
	"You should have received a copy of the GNU General Public License along with " LPRGNAME "; see the file COPYING." \
	"If not, write to the Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA." \
	"</span>"

GHashTable *hash;
GSList *awaiting_activation = NULL;
GtkWidget *font_entry;
gchar
	*homedir        = NULL,
	*newfont        = NULL,
	*selected_theme = NULL,
	*tmp_rc         = NULL;

static void write_rc_file(gchar *include_file, FILE *gtkrc)
{
	fprintf(gtkrc, "# -- THEME AUTO-WRITTEN DO NOT EDIT\ninclude \"%s\"\n\n", include_file);

	if (newfont)
		fprintf(
			gtkrc,
			"style \"user-font\"\n"
			"{\n"
			"\tfont_name=\"%s\"\n"
			"}\n"
			"widget_class \"*\" style \"user-font\"\n\n",
			newfont
		);
	
	fprintf(
		gtkrc,
		"include \"%s/.gtkrc.mine\"\n\n"
		"# -- THEME AUTO-WRITTEN DO NOT EDIT\n",
		homedir
	);

	fclose(gtkrc);
}

static void apply_new_look(void)
{
	if(!selected_theme) return;

	gchar *dir;
	dir = g_hash_table_lookup(hash, selected_theme);
	dir = g_strconcat(dir, "/", NULL);

	gchar* theme_rc;
	theme_rc = g_strconcat(dir, selected_theme, NULL);
	theme_rc = g_strconcat(theme_rc, "/gtk-2.0/gtkrc", NULL);

	if(tmp_rc)
	{
		unlink(tmp_rc);
		g_free(tmp_rc);
	}
		
	write_rc_file(theme_rc, fdopen(g_file_open_tmp("gtkrc.preview-XXXXXXXX", &tmp_rc, NULL), "w+"));

	gchar *default_files[] = { tmp_rc, NULL };
	gtk_rc_set_default_files(default_files);
	gtk_rc_reparse_all_for_settings(gtk_settings_get_default(), TRUE);

	g_free(theme_rc);
}

static void clear_font(void)
{
	if(newfont) {
		g_free(newfont);
		newfont = NULL;
		gtk_entry_set_text(GTK_ENTRY(font_entry), "");
	}
}


static void send_refresh_event(void)
{
	GdkEventClient event = { GDK_CLIENT_EVENT, NULL, TRUE, gdk_atom_intern("_GTK_READ_RCFILES", FALSE), 8 };
	gdk_event_send_clientmessage_toall((GdkEvent*) &event);

	/* make sure we get the ClientEvent ourselves */
	while (gtk_events_pending())
		gtk_main_iteration();
}

void get_dirs(GtkListStore *store, gchar *dirname)
{
	GDir *dir = g_dir_open(dirname, 0, NULL);
	const gchar *entry;
	gchar *rc_file = g_strdup("");
	GtkTreeIter iter;

	if (dir) {
		while ((entry = g_dir_read_name(dir))) {
			if (g_file_test(entry, G_FILE_TEST_IS_DIR))
				continue;

			g_free(rc_file);
			rc_file = g_strdup_printf("%s/%s/gtk-2.0/gtkrc", dirname, entry);

			if (!g_file_test(rc_file, G_FILE_TEST_IS_REGULAR))
				continue;

			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter, 0, g_strdup(entry), -1);
			g_hash_table_insert(hash, g_strdup(entry), dirname);
		}
	}

	g_free(rc_file);
}

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

static void ok_clicked(GtkWidget * button, gpointer data)
{
	if(!selected_theme) return;

	gchar *dir = g_hash_table_lookup(hash, selected_theme);
	gchar *include_rc = g_strdup_printf("%s/%s/gtk-2.0/gtkrc", dir, selected_theme);
	gchar *path_to_gtkrc = g_strdup_printf("%s/.gtkrc-2.0", homedir);

	write_rc_file(include_rc, fopen(path_to_gtkrc, "w"));

	g_free(path_to_gtkrc);
	g_free(include_rc);

	send_refresh_event();

	gtk_main_quit();
}

static void font_dialog_ok_clicked(GtkWidget * w, GtkWidget * dialog)
{
	if (newfont)
		g_free(newfont);
	newfont =
		gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG
		(dialog));
	gtk_entry_set_text(GTK_ENTRY(font_entry), newfont);
	gtk_widget_destroy(dialog);

	apply_new_look();
}

static void font_browse_clicked(GtkWidget *w, gpointer data)
{
	GtkWidget *font_dialog;

	font_dialog = gtk_font_selection_dialog_new("Select Font");
	gtk_font_selection_dialog_set_preview_text(GTK_FONT_SELECTION_DIALOG
		(font_dialog), "Gtk Theme Switch");
	if(newfont)
		gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG(font_dialog), newfont);
	g_signal_connect(G_OBJECT(GTK_FONT_SELECTION_DIALOG(font_dialog)->
			ok_button), "clicked", G_CALLBACK(font_dialog_ok_clicked),
		(gpointer) font_dialog);
	g_signal_connect_swapped(G_OBJECT(GTK_FONT_SELECTION_DIALOG(font_dialog)->
			cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy),
		(gpointer) font_dialog);

	gtk_widget_show(font_dialog);
}

static void font_clear_clicked(GtkWidget *w, gpointer data)
{
	clear_font();
	apply_new_look();
}

gboolean clist_itemselected(GtkTreeSelection * selection, GtkTreeModel * model,
	GtkTreePath * path, gboolean path_currently_selected, gpointer userdata)
{
	GtkTreeIter iter;

	if (!path_currently_selected && gtk_tree_model_get_iter(model, &iter, path)) {
		if(selected_theme)
			g_free(selected_theme);

		gtk_tree_model_get(model, &iter, 0, &selected_theme, -1);
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

int main(int argc, char *argv[])
{
	GtkWidget *mainwin;

	newfont = NULL;
	homedir = getenv("HOME");
	hash = g_hash_table_new(g_str_hash, g_str_equal);

	gtk_init (&argc, &argv);

	GtkWidget *clist, *button;

	mainwin = gtk_dialog_new();
	gtk_widget_realize(mainwin);
	gtk_window_set_title(GTK_WINDOW(mainwin), PRGNAME);
	gtk_window_set_resizable(GTK_WINDOW(mainwin), TRUE);
	gtk_window_set_default_size(GTK_WINDOW(mainwin), 360, 450);
	g_signal_connect(G_OBJECT(mainwin), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	/***************************************************************/

	button = gtk_button_new_from_stock(GTK_STOCK_OK);
	gtk_widget_set_sensitive(button, FALSE);
	awaiting_activation = g_slist_append(awaiting_activation, button);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(ok_clicked), NULL);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->action_area), button, TRUE, TRUE, 0);

	button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(mainwin)->action_area), button, TRUE, TRUE, 0);

	GtkWidget *notebook = gtk_notebook_new();
	gtk_container_set_border_width(GTK_CONTAINER(notebook), 2);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(mainwin)->vbox), notebook);

	/***************************************************************/

	{
		GtkWidget* box = gtk_vbox_new(FALSE, 0);
		gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box, gtk_label_new("Theme"));

		GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
		gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(store), 0, GTK_SORT_ASCENDING);

		// the dirname strings are not freed because they get stored in a hash table
		get_dirs(store, g_strconcat(homedir, "/.themes", NULL));
		get_dirs(store, gtk_rc_get_theme_dir());

		clist = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
		gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(clist), TRUE);
		gtk_tree_view_set_search_column(GTK_TREE_VIEW(clist), 0);

		GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(clist));
		gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);
		gtk_tree_selection_set_select_function(selection, clist_itemselected, NULL, NULL);

		g_object_unref(G_OBJECT(store));

		GtkTreeViewColumn *column =
			gtk_tree_view_column_new_with_attributes("Theme", gtk_cell_renderer_text_new(), "text", 0, NULL);

		gtk_tree_view_column_set_sort_column_id(column, 0);
		gtk_tree_view_append_column(GTK_TREE_VIEW(clist), column);

		GtkWidget *sw = gtk_scrolled_window_new(NULL, NULL);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
		gtk_container_add(GTK_CONTAINER(sw), clist);
		gtk_box_pack_start(GTK_BOX(box), sw, TRUE, TRUE, 0);

		gtk_box_pack_start(GTK_BOX(box), gtk_hseparator_new(), FALSE, FALSE, 2);

		{
			GtkWidget *hbox = gtk_hbox_new(FALSE, 0);

			button = gtk_button_new_from_stock(GTK_STOCK_SELECT_FONT);
			gtk_widget_set_sensitive(button, FALSE);
			awaiting_activation = g_slist_append(awaiting_activation, button);
			g_signal_connect(G_OBJECT(button), "clicked",
				G_CALLBACK(font_browse_clicked), NULL);
			gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

			font_entry = gtk_entry_new();
			gtk_editable_set_editable(GTK_EDITABLE(font_entry), FALSE);
			gtk_box_pack_start(GTK_BOX(hbox), font_entry, TRUE, TRUE, 3);

			button = gtk_button_new_from_stock(GTK_STOCK_CLEAR);
			g_signal_connect(G_OBJECT(button), "clicked",
				G_CALLBACK(font_clear_clicked), NULL);
			gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);

			gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 0);
		}
	}

	/***************************************************************/

	{
		GtkWidget* box = gtk_vbox_new(FALSE, 5);
		gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box, gtk_label_new("Demo"));

	/*	GtkWidget *popup;
		GtkWidget *item;*/

	/*
		popup = gtk_menu_new ();
		gtk_widget_show (popup);
		item = gtk_menu_item_new_with_label ("Menu Entry 1");
		gtk_widget_show (item);
		gtk_menu_append(GTK_MENU(popup), item);
		item = gtk_menu_item_new_with_label ("Menu Entry 2");
		gtk_widget_show (item);
		gtk_menu_append(GTK_MENU(popup), item);
		item = gtk_menu_item_new_with_label ("Menu Entry 3");
		gtk_widget_show (item);
		gtk_menu_append(GTK_MENU(popup), item);
		gtk_signal_connect(GTK_OBJECT(text), "button_press_event", GTK_SIGNAL_FUNC(rightclick), popup);
	 */

		GtkWidget* label = gtk_label_new("Label");
		gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

		label = gtk_label_new("Insensitive Label");
		gtk_widget_set_sensitive(label, FALSE);
		gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

		gtk_box_pack_start(GTK_BOX(box), gtk_hseparator_new(), FALSE, FALSE, 0);

		{
			GtkWidget *hbox = gtk_hbox_new(FALSE, 0);

			gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new(""), FALSE, FALSE, 5);

			button = gtk_check_button_new_with_label("Chk 1");
			gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 3);

			button = gtk_check_button_new_with_label("Chk 2");
			gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 3);

			gtk_box_pack_start(GTK_BOX(hbox), gtk_vseparator_new(), TRUE, FALSE, 5);

			gtk_box_pack_end(GTK_BOX(hbox), gtk_label_new(""), FALSE, FALSE, 5);

			button = gtk_radio_button_new_with_label(NULL, "Radio 3");
			gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 3);

			button = gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(button)), "Radio 2");
			gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 3);

			button = gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(button)), "Radio 1");
			gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 3);

			gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 0);
		}

		GtkTextBuffer *buff = gtk_text_buffer_new(NULL);
		gtk_text_buffer_set_text(
			buff, "Editable textarea demo.\n" "Right click for popup menu demo.", -1
		);

		GtkTextView *text = GTK_TEXT_VIEW(gtk_text_view_new());
		gtk_text_view_set_left_margin(text, 5);
		gtk_text_view_set_right_margin(text, 5);
		gtk_text_view_set_buffer(text, buff);

		GtkFrame* frame = GTK_FRAME(gtk_frame_new("Textarea"));
		gtk_container_set_border_width(GTK_CONTAINER(frame), 2);
		gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(text));
		gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(frame), TRUE, TRUE, 0);

		{
			GtkWidget *hbox = gtk_hbox_new(FALSE, 5);

			GtkAdjustment* adj = GTK_ADJUSTMENT(gtk_adjustment_new(50.0, 0.0, 100.0, 1.0, 5.0, 0.0));

			GtkHScale* scale = GTK_HSCALE(gtk_hscale_new(adj));
			gtk_scale_set_draw_value(GTK_SCALE(scale), FALSE);
			gtk_box_pack_start(GTK_BOX(hbox), GTK_WIDGET(scale), TRUE, TRUE, 0);

			gtk_box_pack_start(GTK_BOX(hbox), gtk_hscrollbar_new(adj), TRUE, TRUE, 0);

			gtk_box_pack_start(GTK_BOX(hbox), gtk_spin_button_new(adj, 1.0, 0), FALSE, FALSE, 0);

			gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 0);
		}

		{
			GtkWidget *hbox = gtk_hbox_new(FALSE, 0);

			button = gtk_button_new_from_stock(GTK_STOCK_EXECUTE);
			gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

			gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new(""), FALSE, FALSE, 10);

			button = gtk_toggle_button_new_with_label(GTK_STOCK_BOLD);
			gtk_button_set_use_stock(GTK_BUTTON(button), TRUE);
			gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

			button = gtk_toggle_button_new_with_label(GTK_STOCK_ITALIC);
			gtk_button_set_use_stock(GTK_BUTTON(button), TRUE);
			gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

			button = gtk_toggle_button_new_with_label(GTK_STOCK_UNDERLINE);
			gtk_button_set_use_stock(GTK_BUTTON(button), TRUE);
			gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

			gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new(""), FALSE, FALSE, 10);

			button = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
			gtk_widget_set_sensitive(button, 0);
			gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

			gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 0);
		}
	}

	/***************************************************************/

	{
		GtkWidget* box = gtk_vbox_new(FALSE, 0);
		gtk_container_set_border_width(GTK_CONTAINER(box), 20);
		gtk_notebook_append_page(GTK_NOTEBOOK(notebook), box, gtk_label_new("About"));

		gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(gtk_label_new_from_pango_markup((ABOUT), GTK_JUSTIFY_CENTER)), TRUE, FALSE, 10);
		
		gtk_box_pack_start(GTK_BOX(box), gtk_hseparator_new(), FALSE, FALSE, 0);

		gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(gtk_label_new_from_pango_markup((ABOUT_ORIGINAL), GTK_JUSTIFY_CENTER)), TRUE, FALSE, 10);
		
		gtk_box_pack_start(GTK_BOX(box), gtk_hseparator_new(), FALSE, FALSE, 0);

		gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(gtk_label_new_from_pango_markup((LICENSE), GTK_JUSTIFY_FILL)), TRUE, FALSE, 10);
	}

	/***************************************************************/

	gtk_widget_show_all(mainwin);

	gtk_main();

	if(tmp_rc)
	{
		unlink(tmp_rc);
		g_free(tmp_rc);
	}

	return 0;
}
