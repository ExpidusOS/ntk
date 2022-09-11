#define G_LOG_DOMAIN "NtkFont"
#include <ntk/font.h>

static float ntk_pango_layout_font_get_width(nk_handle userdata, float height, const char* text, int length) {
	PangoLayout* font_layout = PANGO_LAYOUT(userdata.ptr);

	PangoContext* pango_ctx = pango_layout_get_context(font_layout);
	const PangoFontDescription* font_desc = pango_layout_get_font_description(font_layout);

	PangoLayout* layout = pango_layout_new(pango_ctx);
	pango_layout_set_font_description(layout, font_desc);
	pango_layout_set_height(layout, height);
	pango_layout_set_text(layout, text, length);
	int width = pango_layout_get_width(layout);
	g_object_unref(layout);
	return width;
}

NtkPangoLayoutFont* ntk_pango_layout_font_new(PangoLayout* layout, PangoFontDescription* font_desc) {
	pango_layout_set_font_description(layout, font_desc);

	NtkPangoLayoutFont* font = g_try_malloc0(sizeof (NtkPangoLayoutFont));
	if (font == NULL) return NULL;

	font->userdata.ptr = layout;
	font->height = pango_layout_get_height(layout);
	font->width = ntk_pango_layout_font_get_width;
	return font;
}

NtkPangoLayoutFont* ntk_pango_layout_font_ref(NtkPangoLayoutFont* font) {
	PangoLayout* layout = PANGO_LAYOUT(g_object_ref(G_OBJECT(font->userdata.ptr)));
	PangoFontDescription* font_desc = pango_font_description_copy(pango_layout_get_font_description(layout));
	return ntk_pango_layout_font_new(layout, font_desc);
}

void ntk_pango_layout_font_unref(NtkPangoLayoutFont* font) {
	g_object_unref(font->userdata.ptr);
	g_free(font);
}

void ntk_pango_layout_font_free(NtkPangoLayoutFont* font) {
	g_object_unref(font->userdata.ptr);
	g_free(font);
}
