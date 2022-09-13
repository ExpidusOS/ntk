#ifndef __NTK_INPUT_DISPATCHER_H_
#define __NTK_INPUT_DISPATCHER_H_ 1

#include <stdarg.h>
#include <glib-object.h>
#include <nuklear.h>
#include "context.h"

G_BEGIN_DECLS

#define NTK_TYPE_INPUT_DISPATCHER ntk_input_dispatcher_get_type()
G_DECLARE_DERIVABLE_TYPE(NtkInputDispatcher, ntk_input_dispatcher, NTK, INPUT_DISPATCHER, GObject);

typedef enum {
  NTK_INPUT_DISPATCHER_TYPE_NONE = 0,
  NTK_INPUT_DISPATCHER_TYPE_MOTION,
  NTK_INPUT_DISPATCHER_TYPE_KEY,
  NTK_INPUT_DISPATCHER_TYPE_BUTTON,
  NTK_INPUT_DISPATCHER_TYPE_SCROLL,
  NTK_INPUT_DISPATCHER_TYPE_ASCII,
  NTK_INPUT_DISPATCHER_TYPE_GLYPH,
  NTK_INPUT_DISPATCHER_TYPE_UNICODE
} NtkInputDispatcherEventType;

typedef struct {
  NtkInputDispatcherEventType type;

  union {
    struct {
      int x;
      int y;
    } motion;
    struct {
      enum nk_keys key;
      int down;
    } key;
    struct {
      enum nk_buttons btn;
      int x;
      int y;
      int down;
    } button;
    struct {
      float x;
      float y;
    } scroll;
    struct {
      char* str;
      size_t len;
    } ascii;
    struct {
      nk_glyph g;
    } glyph;
    struct {
      nk_rune r;
    } unicode;
  } data;
} NtkInputDispatcherEvent;

#define NTK_INPUT_DISPATCHER_EVENT_MOTION(ev) ((ev)->data.motion)
#define NTK_INPUT_DISPATCHER_EVENT_KEY(ev) ((ev)->data.key)
#define NTK_INPUT_DISPATCHER_EVENT_BUTTON(ev) ((ev)->data.button)
#define NTK_INPUT_DISPATCHER_EVENT_SCROLL(ev) ((ev)->data.scroll)
#define NTK_INPUT_DISPATCHER_EVENT_ASCII(ev) ((ev)->data.ascii)
#define NTK_INPUT_DISPATCHER_EVENT_GLYPH(ev) ((ev)->data.glyph)
#define NTK_INPUT_DISPATCHER_EVENT_UNICODE(ev) ((ev)->data.unicode)

/**
 * NtkInputDispatcher:
 *
 * Since: 0.1.0
 */
struct _NtkInputDispatcherClass {
  GObjectClass parent_class;

  void (*wait)(NtkInputDispatcher* self, NtkInputDispatcherEvent* event);
};

void ntk_input_dispatcher_attach(NtkInputDispatcher* self, NtkContext* ctx);
void ntk_input_dispatcher_detach(NtkInputDispatcher* self);

/**
 * ntk_input_dispatcher_wait:
 *
 * Waits for the dispatcher to get an event
 */
void ntk_input_dispatcher_wait(NtkInputDispatcher* self);

/**
 * ntk_input_dispatcher_trigger:
 *
 * Triggers an input event manually.
 */
void ntk_input_dispatcher_trigger(NtkInputDispatcher* self, NtkInputDispatcherEventType type, ...);

/**
 * ntk_input_dispatcher_triggerv:
 *
 * Triggers an input event manually.
 */
void ntk_input_dispatcher_triggerv(NtkInputDispatcher* self, NtkInputDispatcherEventType type, va_list ap);

/**
 * ntk_input_dispatcher_triggerev:
 *
 * Triggers an input event manually.
 */
void ntk_input_dispatcher_triggerev(NtkInputDispatcher* self, NtkInputDispatcherEvent* ev);

G_END_DECLS

#endif
