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
#include "preview_pane.h"
#include "util.h"
#include "stock.h"

#define MENU_PREVIEW_ITEMS 2

GtkWidget* create_preview_pane(void)
{
	GtkWidget* frame;
	GtkBox* box = GTK_BOX(unfocussable(gtk_vbox_new(FALSE, 5)));

	{
		gint i;
		GtkWidget *item;

		GtkWidget *menubar = unfocussable(gtk_menu_bar_new());

		GtkWidget *menu = unfocussable(gtk_menu_new());
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), unfocussable(gtk_tearoff_menu_item_new()));

		item = unfocussable(gtk_menu_item_new_with_label("Menu"));
		gtk_menu_shell_append(GTK_MENU_SHELL(menubar), item);
		gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), menu);

		for (i = 0; i < MENU_PREVIEW_ITEMS; i++)
		{
			gtk_menu_shell_append(GTK_MENU_SHELL(menu), unfocussable(gtk_menu_item_new_with_label("Simple item")));
		}

		gtk_menu_shell_append(GTK_MENU_SHELL(menu), unfocussable(gtk_separator_menu_item_new()));

		for (i = 0; i < MENU_PREVIEW_ITEMS; i++)
		{
			gtk_menu_shell_append(GTK_MENU_SHELL(menu), unfocussable(gtk_image_menu_item_new_from_stock(GTK_STOCK_IMAGE, NULL)));
		}

		gtk_menu_shell_append(GTK_MENU_SHELL(menu), unfocussable(gtk_separator_menu_item_new()));

		{
			GSList* group = NULL;
			for (i = 0; i < MENU_PREVIEW_ITEMS; i++)
			{
				item = unfocussable(gtk_radio_menu_item_new_with_label(group, "Radio"));
				gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
				group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM (item));
				if(i == 0)
					gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), TRUE);
			}
		}

		gtk_menu_shell_append(GTK_MENU_SHELL(menu), unfocussable(gtk_separator_menu_item_new()));

		for (i = 0; i < MENU_PREVIEW_ITEMS; i++)
		{
			item = unfocussable(gtk_check_menu_item_new_with_label("Check"));
			gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
			if(i == 0)
				gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), TRUE);
		}

		gtk_box_pack_start(box, menubar, FALSE, FALSE, 0);
	}

	{
		GtkToolbar* toolbar = GTK_TOOLBAR(unfocussable(gtk_toolbar_new()));
		GtkWidget* button;
		GtkRadioButton* radio;

		button = unfocussable(gtk_check_button_new_with_label("Check 1"));
		gtk_container_add(GTK_CONTAINER(toolbar), button);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);

		button = unfocussable(gtk_check_button_new_with_label("Check 2"));
		gtk_container_add(GTK_CONTAINER(toolbar), button);

		/* gtk_toolbar_append_space(toolbar); */

		radio = GTK_RADIO_BUTTON(unfocussable(gtk_radio_button_new_with_label(NULL, "Radio 1")));
		gtk_container_add(GTK_CONTAINER(toolbar), GTK_WIDGET(radio));
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio), TRUE);

		radio = GTK_RADIO_BUTTON(unfocussable(gtk_radio_button_new_with_label_from_widget(radio, "Radio 2")));
		gtk_container_add(GTK_CONTAINER(toolbar), GTK_WIDGET(radio));

		radio = GTK_RADIO_BUTTON(unfocussable(gtk_radio_button_new_with_label_from_widget(radio, "Radio 3")));
		gtk_container_add(GTK_CONTAINER(toolbar), GTK_WIDGET(radio));

		gtk_box_pack_start(box, GTK_WIDGET(toolbar), FALSE, FALSE, 0);
	}

	{
		GtkBox* hbox = GTK_BOX(unfocussable(gtk_hbox_new(FALSE, 0)));
		GtkWidget* button;

		gtk_box_pack_start(hbox, unfocussable(gtk_button_new_from_stock(GTK_STOCK_BUTTON)), TRUE, TRUE, 0);

		gtk_box_pack_start(hbox, unfocussable(gtk_label_new("")), FALSE, FALSE, 10);

		gtk_box_pack_start(hbox, button = unfocussable(gtk_toggle_button_new_from_stock(GTK_STOCK_TOGGLE1)), TRUE, TRUE, 0);
		gtk_box_pack_start(hbox, unfocussable(gtk_toggle_button_new_from_stock(GTK_STOCK_TOGGLE2)), TRUE, TRUE, 0);
		gtk_box_pack_start(hbox, unfocussable(gtk_toggle_button_new_from_stock(GTK_STOCK_TOGGLE3)), TRUE, TRUE, 0);

		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);

		gtk_box_pack_start(hbox, unfocussable(gtk_label_new("")), FALSE, FALSE, 10);

		{
			GtkWidget* button = unfocussable(gtk_button_new_from_stock(GTK_STOCK_DISABLED));
			gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);

			gtk_widget_set_sensitive(button, FALSE);
		}

		gtk_box_pack_start(box, GTK_WIDGET(hbox), FALSE, FALSE, 0);
	}

	{
		GtkBox* hbox = GTK_BOX(unfocussable(gtk_hbox_new(FALSE, 5)));

		GtkAdjustment* adj = GTK_ADJUSTMENT(gtk_adjustment_new(50.0, 0.0, 100.0, 1.0, 5.0, 0.0));

		GtkScale* scale = GTK_SCALE(unfocussable(gtk_hscale_new(adj)));
		gtk_scale_set_draw_value(scale, FALSE);

		gtk_box_pack_start(hbox, GTK_WIDGET(scale), TRUE, TRUE, 0);
		gtk_box_pack_start(hbox, unfocussable(gtk_hscrollbar_new(adj)), TRUE, TRUE, 0);
		gtk_box_pack_start(hbox, unfocussable(gtk_spin_button_new(adj, 1.0, 0)), FALSE, FALSE, 0);

		gtk_box_pack_start(box, GTK_WIDGET(hbox), FALSE, FALSE, 0);
	}

	frame = unfocussable(gtk_frame_new("Preview"));
	gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(box));

	return frame;
}
