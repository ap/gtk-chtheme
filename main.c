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
#include <unistd.h>				// unlink()
#include <stdlib.h>				// getenv()
#include <stdio.h>				// fopen() fdopen() fprintf()
#include <string.h>
#include <fcntl.h>
#include "main.h"
#include "stock.h"
#include "mainwin.h"

GHashTable *hash = NULL;
GSList *awaiting_activation = NULL;
gchar *tmp_rc = NULL, *gtkrc = NULL, *font = NULL, *themename = NULL;

// dirname gets freed!
void read_theme_list(gchar * dirname)
{
	GDir *dir = g_dir_open(dirname, 0, NULL);
	const gchar *entry;
	gchar *rc_file;

	if (dir)
	{
		while ((entry = g_dir_read_name(dir)))
		{
			if (g_file_test(entry, G_FILE_TEST_IS_DIR))
				continue;

			rc_file = g_strdup_printf("%s/%s/gtk-2.0/gtkrc", dirname, entry);

			if (!g_file_test(rc_file, G_FILE_TEST_IS_REGULAR))
			{
				g_free(rc_file);
				continue;
			}

			g_hash_table_insert(hash, g_strdup(entry), rc_file);
		}
	}

	g_free(dirname);
}

void populate_themelist(GHFunc func, gpointer user_data)
{
	g_hash_table_foreach(hash, func, user_data);
}

void cleanup_temporary(void)
{
	if (tmp_rc)
	{
		unlink(tmp_rc);
		g_free(tmp_rc);
		tmp_rc = NULL;
	}
}

void apply_new_look(gboolean is_preview)
{
	if(!themename) return;

	cleanup_temporary();

	FILE *gtkrc_fh = is_preview
		? fdopen(g_file_open_tmp("gtkrc.preview-XXXXXXXX", &tmp_rc, NULL), "w+")
		: fopen(gtkrc, "w");

	gchar *include_file = g_hash_table_lookup(hash, themename);

	fprintf(gtkrc_fh,
		"# -- THEME AUTO-WRITTEN DO NOT EDIT\n" "include \"%s\"\n\n",
		include_file);

	if (font)
	{
		fprintf(gtkrc_fh,
			"style \"user-font\" {\n" "\tfont_name = \"%s\"\n" "}\n\n", font);
		fprintf(gtkrc_fh, "widget_class \"*\" style \"user-font\"\n\n");
		fprintf(gtkrc_fh, "gtk-font-name=\"%s\"\n\n", font);
	}

	fprintf(gtkrc_fh, "include \"%s/.gtkrc.mine\"\n\n", getenv("HOME"));
	fprintf(gtkrc_fh, "# -- THEME AUTO-WRITTEN DO NOT EDIT\n");
	fclose(gtkrc_fh);

	gchar *default_files[] = { is_preview ? tmp_rc : gtkrc, NULL };
	gtk_rc_set_default_files(default_files);

	if (is_preview)
	{
		gtk_rc_reparse_all_for_settings(gtk_settings_get_default(), TRUE);
	}
	else
	{
		GdkEventClient event =
			{ GDK_CLIENT_EVENT, NULL, TRUE, gdk_atom_intern("_GTK_READ_RCFILES",
				FALSE), 8 };
		gdk_event_send_clientmessage_toall((GdkEvent *) & event);
	}
}

void await_activation(GtkWidget* w)
{
	if(themename) return;

	gtk_widget_set_sensitive(w, FALSE);
	awaiting_activation = g_slist_append(awaiting_activation, w);
}

void set_font(gchar * newfont, gboolean is_preview)
{
	if (font)
		g_free(font);
	font = newfont;
	apply_new_look(is_preview);
}

gchar* get_font(void)
{
	return font;
}

void set_theme(gchar * newthemename, gboolean is_preview)
{
	themename = newthemename;
	apply_new_look(is_preview);

	if (awaiting_activation)
	{
		g_slist_foreach(awaiting_activation,
			(GFunc) gtk_widget_set_sensitive, GINT_TO_POINTER(TRUE));
		g_slist_free(awaiting_activation);
		awaiting_activation = NULL;
	}
}

gchar* get_theme(void)
{
	return themename;
}

void rc_skip_section(GScanner *s, GTokenType close)
{
	while(!g_scanner_eof(s)) {
		g_scanner_get_next_token(s);
		switch(s->token)
		{
			case G_TOKEN_LEFT_PAREN: rc_skip_section(s, G_TOKEN_RIGHT_PAREN); break;
			case G_TOKEN_LEFT_CURLY: rc_skip_section(s, G_TOKEN_RIGHT_CURLY); break;
			case G_TOKEN_LEFT_BRACE: rc_skip_section(s, G_TOKEN_RIGHT_BRACE); break;
			default: if(s->token == close) return;
		}
	}
}

void set_theme_if_match(gpointer key, gpointer value, gpointer path)
{
	if(strcmp(path, value) == 0)
		set_theme(key, TRUE);
}

void parse_gtkrc(void)
{
	GScanner *s = gtk_rc_scanner_new();
	g_scanner_input_file(s, open(gtkrc, O_RDONLY));

	g_strconcat(getenv("HOME"), "/.themes", NULL);
	g_strconcat(gtk_rc_get_theme_dir(), "%s/gtk-2.0/gtkrc", NULL);

	while(!g_scanner_eof(s)) {
		g_scanner_get_next_token(s);
		switch(s->token)
		{
			case G_TOKEN_LEFT_PAREN: rc_skip_section(s, G_TOKEN_RIGHT_PAREN); break;
			case G_TOKEN_LEFT_CURLY: rc_skip_section(s, G_TOKEN_RIGHT_CURLY); break;
			case G_TOKEN_LEFT_BRACE: rc_skip_section(s, G_TOKEN_RIGHT_BRACE); break;
			case G_TOKEN_IDENTIFIER:
				if(strcmp(s->value.v_string, "include") == 0)
				{
					if(g_scanner_get_next_token(s) == G_TOKEN_STRING)
						g_hash_table_foreach(hash, set_theme_if_match, s->value.v_string);
				}
				else if(strcmp(s->value.v_string, "gtk-font-name") == 0)
				{
					if(g_scanner_get_next_token(s) == G_TOKEN_EQUAL_SIGN
							&& g_scanner_get_next_token(s) == G_TOKEN_STRING)
						set_font(strdup(s->value.v_string), TRUE);
				}
				break;
			default:
				break;
		}
	}
	g_scanner_destroy(s);
}

int main(int argc, char *argv[])
{
	hash = g_hash_table_new(g_str_hash, g_str_equal);
	gtkrc = g_strdup_printf("%s/.gtkrc-2.0", getenv("HOME"));

	gtk_init(&argc, &argv);

	init_new_stock_items();

	read_theme_list(g_strconcat(getenv("HOME"), "/.themes", NULL));
	read_theme_list(gtk_rc_get_theme_dir());

	parse_gtkrc();

	gtk_widget_show_all(create_mainwin());

	gtk_main();

	cleanup_temporary();

	return 0;
}
