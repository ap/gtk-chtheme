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
#include "stock.h"

#define NUM_NEW_STOCK_ITEMS 6

GtkStockItem stock_items[NUM_NEW_STOCK_ITEMS] = {
	{ GTK_STOCK_BUTTON,   "Button",     0, 0, "gtk20" },
	{ GTK_STOCK_TOGGLE1,  "Toggle",     0, 0, "gtk20" },
	{ GTK_STOCK_TOGGLE2,  "Toggle",     0, 0, "gtk20" },
	{ GTK_STOCK_TOGGLE3,  "Toggle",     0, 0, "gtk20" },
	{ GTK_STOCK_DISABLED, "Disabled",   0, 0, "gtk20" },
	{ GTK_STOCK_IMAGE,    "With image", 0, 0, "gtk20" },
};

struct { gchar *old_id, *new_id; } stock_map[NUM_NEW_STOCK_ITEMS] = {
	{ GTK_STOCK_BUTTON,   GTK_STOCK_EXECUTE   },
	{ GTK_STOCK_TOGGLE1,  GTK_STOCK_BOLD      },
	{ GTK_STOCK_TOGGLE2,  GTK_STOCK_ITALIC    },
	{ GTK_STOCK_TOGGLE3,  GTK_STOCK_UNDERLINE },
	{ GTK_STOCK_DISABLED, GTK_STOCK_CLOSE     },
	{ GTK_STOCK_IMAGE,    GTK_STOCK_CONVERT   },
};

void init_new_stock_items(void) {
	GtkIconFactory* demo_icon_factory = gtk_icon_factory_new();

	int i;
	for(i = 0; i < NUM_NEW_STOCK_ITEMS; ++i) {
		gtk_icon_factory_add(demo_icon_factory, stock_map[i].old_id, gtk_icon_factory_lookup_default(stock_map[i].new_id));
	}

	gtk_stock_add_static(stock_items, NUM_NEW_STOCK_ITEMS);
	gtk_icon_factory_add_default(demo_icon_factory);
}
