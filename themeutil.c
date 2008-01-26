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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "themeutil.h"

GHashTable *hash;
gchar *tmp_rc = NULL, *gtkrc  = NULL;

gchar* homedir(void)
{ return getenv("HOME"); }

void cleanup_temporary(void)
{
	if(tmp_rc) {
		unlink(tmp_rc);
		g_free(tmp_rc);
		tmp_rc = NULL;
	}
}

// dirname gets freed!
void init_theme_list(gchar *dirname)
{
	GDir *dir = g_dir_open(dirname, 0, NULL);
	const gchar *entry;
	gchar *rc_file;

	if(!hash)
		hash = g_hash_table_new(g_str_hash, g_str_equal);

	if(!gtkrc)
		gtkrc = g_strdup_printf("%s/.gtkrc-2.0", homedir());

	if (dir) {
		while ((entry = g_dir_read_name(dir))) {
			if (g_file_test(entry, G_FILE_TEST_IS_DIR))
				continue;

			rc_file = g_strdup_printf("%s/%s/gtk-2.0/gtkrc", dirname, entry);

			if (!g_file_test(rc_file, G_FILE_TEST_IS_REGULAR)) {
				g_free(rc_file);
				continue;
			}

			g_hash_table_insert(hash, g_strdup(entry), rc_file);
		}
	}

	g_free(dirname);
}

gchar* write_rc_file(gchar *theme_dir, gchar* newfont, gboolean is_preview)
{
	FILE *gtkrc_fh;

	cleanup_temporary();

	gtkrc_fh = is_preview
		? fdopen(g_file_open_tmp("gtkrc.preview-XXXXXXXX", &tmp_rc, NULL), "w+")
		: fopen(gtkrc, "w");

	gchar *include_file = g_hash_table_lookup(hash, theme_dir);
	fprintf(gtkrc_fh, "# -- THEME AUTO-WRITTEN DO NOT EDIT\n" "include \"%s\"\n\n", include_file);

	if (newfont)
		fprintf(
			gtkrc_fh,
			"style \"user-font\"\n"
			"{\n"
			"\tfont_name=\"%s\"\n"
			"}\n"
			"widget_class \"*\" style \"user-font\"\n\n",
			newfont
		);
	
	fprintf(gtkrc_fh, "include \"%s/.gtkrc.mine\"\n\n", homedir());
	fprintf(gtkrc_fh, "# -- THEME AUTO-WRITTEN DO NOT EDIT\n");
	fclose(gtkrc_fh);

	return tmp_rc ? tmp_rc : gtkrc;
}

void populate_themelist (gpointer func, gpointer user_data)
{
	g_hash_table_foreach(hash, (GHFunc)func, user_data);
}
