#define G_LOG_DOMAIN "NtkInputDispatcher"
#include <ntk/input-dispatcher.h>
#include "input-dispatcher-priv.h"
#include "context-priv.h"

#define NTK_INPUT_DISPATCHER_PRIVATE(self) (ntk_input_dispatcher_get_instance_private(self))

G_DEFINE_TYPE_WITH_PRIVATE(NtkInputDispatcher, ntk_input_dispatcher, G_TYPE_OBJECT);

enum {
  SIG_ATTACH,
  SIG_DETACH,
  N_SIGNALS
};

static guint obj_sigs[N_SIGNALS] = { 0 };

static void ntk_input_dispatcher_constructed(GObject* obj) {
  G_OBJECT_CLASS(ntk_input_dispatcher_parent_class)->constructed(obj);

  NtkInputDispatcher* self = NTK_INPUT_DISPATCHER(obj);
  NtkInputDispatcherPrivate* priv = NTK_INPUT_DISPATCHER_PRIVATE(self);

  g_mutex_init(&priv->running);
}

static void ntk_input_dispatcher_finalize(GObject* obj) {
  NtkInputDispatcher* self = NTK_INPUT_DISPATCHER(obj);
  NtkInputDispatcherPrivate* priv = NTK_INPUT_DISPATCHER_PRIVATE(self);

  g_mutex_clear(&priv->running);
  g_clear_object(&priv->ctx);

  G_OBJECT_CLASS(ntk_input_dispatcher_parent_class)->finalize(obj);
}

static void ntk_input_dispatcher_class_init(NtkInputDispatcherClass* klass) {
  GObjectClass* object_class = G_OBJECT_CLASS(klass);

  object_class->constructed = ntk_input_dispatcher_constructed;
  object_class->finalize = ntk_input_dispatcher_finalize;

  obj_sigs[SIG_ATTACH] = g_signal_new("attach", G_OBJECT_CLASS_TYPE(object_class), G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL, G_TYPE_NONE, 1, NTK_TYPE_CONTEXT);
  obj_sigs[SIG_DETACH] = g_signal_new("detach", G_OBJECT_CLASS_TYPE(object_class), G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
}

static void ntk_input_dispatcher_init(NtkInputDispatcher* self) {}

void ntk_input_dispatcher_attach(NtkInputDispatcher* self, NtkContext* ctx) {
  g_return_if_fail(NTK_IS_INPUT_DISPATCHER(self));

  NtkInputDispatcherPrivate* priv = NTK_INPUT_DISPATCHER_PRIVATE(self);
  g_return_if_fail(priv != NULL);
  g_return_if_fail(priv->ctx == NULL);

  priv->ctx = NTK_CONTEXT(g_object_ref(ctx));
  g_signal_emit(self, obj_sigs[SIG_ATTACH], 0, priv->ctx);
}

void ntk_input_dispatcher_detach(NtkInputDispatcher* self) {
  g_return_if_fail(NTK_IS_INPUT_DISPATCHER(self));

  NtkInputDispatcherPrivate* priv = NTK_INPUT_DISPATCHER_PRIVATE(self);
  g_return_if_fail(priv != NULL);
  g_return_if_fail(priv->ctx != NULL);

  g_clear_object(&priv->ctx);
  g_signal_emit(self, obj_sigs[SIG_DETACH], 0);
}

void ntk_input_dispatcher_wait(NtkInputDispatcher* self, NtkInputDispatcherEvent* ev) {
  g_return_if_fail(NTK_IS_INPUT_DISPATCHER(self));

  NtkInputDispatcherPrivate* priv = NTK_INPUT_DISPATCHER_PRIVATE(self);
  g_return_if_fail(priv != NULL);

  NtkInputDispatcherClass* klass = NTK_INPUT_DISPATCHER_GET_CLASS(self);
  g_return_if_fail(klass->wait != NULL);

  NtkInputDispatcherEvent event;
  if (ev == NULL) event.type = NTK_INPUT_DISPATCHER_TYPE_NONE;
	else ev->type = NTK_INPUT_DISPATCHER_TYPE_NONE;
  klass->wait(self, ev == NULL ? &event : ev);

  ntk_input_dispatcher_triggerev(self, ev == NULL ? &event : ev);
}

void ntk_input_dispatcher_trigger(NtkInputDispatcher* self, NtkInputDispatcherEventType type, ...) {
	va_list ap;
	va_start(ap, type);
	ntk_input_dispatcher_triggerv(self, type, ap);
	va_end(ap);
}

void ntk_input_dispatcher_triggerv(NtkInputDispatcher* self, NtkInputDispatcherEventType type, va_list ap) {
  g_return_if_fail(NTK_IS_INPUT_DISPATCHER(self));
  g_return_if_fail(ap != NULL);

	NtkInputDispatcherEvent ev;
	ev.type = type;

	switch (ev.type) {
		case NTK_INPUT_DISPATCHER_TYPE_NONE:
			break;
    case NTK_INPUT_DISPATCHER_TYPE_MOTION:
			ev.data.motion.x = va_arg(ap, int);
			ev.data.motion.y = va_arg(ap, int);
			break;
    case NTK_INPUT_DISPATCHER_TYPE_KEY:
			ev.data.key.key = va_arg(ap, enum nk_keys);
			ev.data.key.down = va_arg(ap, int);
			break;
    case NTK_INPUT_DISPATCHER_TYPE_BUTTON:
			ev.data.button.btn = va_arg(ap, enum nk_buttons);
			ev.data.button.x = va_arg(ap, int);
			ev.data.button.y = va_arg(ap, int);
			ev.data.button.down = va_arg(ap, int);
			break;
    case NTK_INPUT_DISPATCHER_TYPE_SCROLL:
			ev.data.scroll.x = va_arg(ap, double);
			ev.data.scroll.y = va_arg(ap, double);
			break;
    case NTK_INPUT_DISPATCHER_TYPE_ASCII:
			ev.data.ascii.str = va_arg(ap, char*);
			ev.data.ascii.len = va_arg(ap, size_t);
			break;
    case NTK_INPUT_DISPATCHER_TYPE_GLYPH:
			memcpy(&ev.data.glyph.g, va_arg(ap, nk_glyph), sizeof (nk_glyph));
			break;
    case NTK_INPUT_DISPATCHER_TYPE_UNICODE:
			ev.data.unicode.r = va_arg(ap, nk_rune);
			break;
	}

	ntk_input_dispatcher_triggerev(self, &ev);
}

void ntk_input_dispatcher_triggerev(NtkInputDispatcher* self, NtkInputDispatcherEvent* ev) {
  g_return_if_fail(NTK_IS_INPUT_DISPATCHER(self));
  g_return_if_fail(ev != NULL);

  NtkInputDispatcherPrivate* priv = NTK_INPUT_DISPATCHER_PRIVATE(self);
  g_return_if_fail(priv != NULL);
  g_return_if_fail(priv->ctx != NULL);

  g_return_if_fail(g_mutex_trylock(&priv->running));
  nk_input_begin(&priv->ctx->priv->nk);

  switch (ev->type) {
    case NTK_INPUT_DISPATCHER_TYPE_NONE:
      break;
    case NTK_INPUT_DISPATCHER_TYPE_MOTION:
			g_debug("Received motion: x: %d, y: %d", NTK_INPUT_DISPATCHER_EVENT_MOTION(ev).x, NTK_INPUT_DISPATCHER_EVENT_BUTTON(ev).y);
      nk_input_motion(&priv->ctx->priv->nk, NTK_INPUT_DISPATCHER_EVENT_MOTION(ev).x, NTK_INPUT_DISPATCHER_EVENT_BUTTON(ev).y);
      break;
    case NTK_INPUT_DISPATCHER_TYPE_KEY:
			g_debug("Received key: key: %d, down: %d", NTK_INPUT_DISPATCHER_EVENT_KEY(ev).key, NTK_INPUT_DISPATCHER_EVENT_KEY(ev).down);
      nk_input_key(&priv->ctx->priv->nk, NTK_INPUT_DISPATCHER_EVENT_KEY(ev).key, NTK_INPUT_DISPATCHER_EVENT_KEY(ev).down);
      break;
    case NTK_INPUT_DISPATCHER_TYPE_BUTTON:
			g_debug("Received button: btn: %d, x: %d, y: %d, down: %d", NTK_INPUT_DISPATCHER_EVENT_BUTTON(ev).btn, NTK_INPUT_DISPATCHER_EVENT_BUTTON(ev).x, NTK_INPUT_DISPATCHER_EVENT_BUTTON(ev).y,
        NTK_INPUT_DISPATCHER_EVENT_BUTTON(ev).down);
      nk_input_button(&priv->ctx->priv->nk, NTK_INPUT_DISPATCHER_EVENT_BUTTON(ev).btn, NTK_INPUT_DISPATCHER_EVENT_BUTTON(ev).x, NTK_INPUT_DISPATCHER_EVENT_BUTTON(ev).y,
        NTK_INPUT_DISPATCHER_EVENT_BUTTON(ev).down);
      break;
    case NTK_INPUT_DISPATCHER_TYPE_SCROLL:
      {
        struct nk_vec2 val;
        val.x = NTK_INPUT_DISPATCHER_EVENT_SCROLL(ev).x;
        val.y = NTK_INPUT_DISPATCHER_EVENT_SCROLL(ev).y;
				g_debug("Received scroll: %f, %f", val.x, val.y);
        nk_input_scroll(&priv->ctx->priv->nk, val);
      }
      break;
    case NTK_INPUT_DISPATCHER_TYPE_ASCII:
			g_debug("Received ascii: %lu \"%s\"", NTK_INPUT_DISPATCHER_EVENT_ASCII(ev).len, NTK_INPUT_DISPATCHER_EVENT_ASCII(ev).str);
      for (size_t i = 0; i < NTK_INPUT_DISPATCHER_EVENT_ASCII(ev).len; i++) {
        nk_input_char(&priv->ctx->priv->nk, NTK_INPUT_DISPATCHER_EVENT_ASCII(ev).str[i]);
      }
      break;
    case NTK_INPUT_DISPATCHER_TYPE_GLYPH:
      nk_input_glyph(&priv->ctx->priv->nk, NTK_INPUT_DISPATCHER_EVENT_GLYPH(ev).g);
      break;
    case NTK_INPUT_DISPATCHER_TYPE_UNICODE:
      nk_input_unicode(&priv->ctx->priv->nk, NTK_INPUT_DISPATCHER_EVENT_UNICODE(ev).r);
      break;
  }

  nk_input_end(&priv->ctx->priv->nk);
  g_mutex_unlock(&priv->running);

	ntk_renderer_request_draw(priv->ctx->priv->renderer);
}
