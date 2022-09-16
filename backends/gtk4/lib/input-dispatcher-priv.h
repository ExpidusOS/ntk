#pragma once

#include <gtk/gtk.h>

typedef struct _NtkGtk4InputDispatcherPrivate {
  GtkWidget* widget;
  GHashTable* handlers;
  GListModel* controllers;
  gulong controllers_change_id;
} NtkGtk4InputDispatcherPrivate;
