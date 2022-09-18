#pragma once

#include <gtk/gtk.h>
#include <ntk/renderer.h>

typedef struct _NtkGtk4WidgetPrivate {
  NtkRenderer* renderer;
  int width;
  int height;

  GtkIMContext* im_context;

  GtkGesture* controller_gclick;
  GtkEventController* controller_key;
  GtkEventController* controller_scroll;
  GtkEventController* controller_motion;
} NtkGtk4WidgetPrivate;
