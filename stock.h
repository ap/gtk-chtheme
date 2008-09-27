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

#ifndef ___STOCK_H
#define ___STOCK_H 1

#ifndef USE_OWN_ABOUT_STOCK
#define USE_OWN_ABOUT_STOCK (! GTK_CHECK_VERSION( 2, 6, 0 ))
#endif

#define GTK_STOCK_BUTTON   "gtk-button"
#define GTK_STOCK_TOGGLE1  "gtk-toggle1"
#define GTK_STOCK_TOGGLE2  "gtk-toggle2"
#define GTK_STOCK_TOGGLE3  "gtk-toggle3"
#define GTK_STOCK_DISABLED "gtk-disabled"
#define GTK_STOCK_IMAGE    "gtk-image"

#if USE_OWN_ABOUT_STOCK
#define GTK_STOCK_ABOUT "gtk-about"
#endif

void init_new_stock_items(void);

#endif
