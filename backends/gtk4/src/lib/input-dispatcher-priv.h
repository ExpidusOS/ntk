#ifndef __INPUT_DISPATCHER_PRIV_H_
#define __INPUT_DISPATCHER_PRIV_H_ 1

#include <gtk/gtk.h>

typedef struct _NtkGtk4InputDispatcherPrivate {
	GtkWidget* widget;
	GHashTable* handlers;
	GListModel* controllers;
	gulong controllers_change_id;
} NtkGtk4InputDispatcherPrivate;

#endif
