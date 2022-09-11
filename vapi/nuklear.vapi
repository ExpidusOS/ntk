[CCode(lower_case_cprefix = "nk_", cheader_filename = "nuklear.h")]
namespace Nk {
	[SimpleType]
	[CCode(cname = "nk_rune", has_type_id = true)]
	public struct Rune : uint {}

	[SimpleType]
	[CCode(cname = "nk_flags", has_type_id = true)]
	public struct Flags : uint {}

	[SimpleType]
	[CCode(cname = "enum nk_text_alignment", has_type_id = false)]
	public enum TextAlignment {
		[CCode(cname = "NK_TEXT_LEFT")] LEFT,
		[CCode(cname = "NK_TEXT_CENTERED")] CENTERED,
		[CCode(cname = "NK_TEXT_RIGHT")] RIGHT
	}

	[SimpleType]
	[CCode(cname = "enum nk_layout_format", has_type_id = false)]
	public enum LayoutFormat {
		[CCode(cname = "NK_DYNAMIC")] DYNAMIC,
		[CCode(cname = "NK_STATIC")] STATIC
	}

	[SimpleType]
	[CCode(cname = "enum nk_panel_flags", has_type_id = false)]
	public enum PanelFlags {
		[CCode(cname = "NK_WINDOW_BORDER")] BORDER,
		[CCode(cname = "NK_WINDOW_MOVABLE")] MOVABLE,
		[CCode(cname = "NK_WINDOW_SCALABLE")] SCALABLE,
		[CCode(cname = "NK_WINDOW_CLOSABLE")] CLOSABLE,
		[CCode(cname = "NK_WINDOW_MINIMIZABLE")] MINIMIZABLE,
		[CCode(cname = "NK_WINDOW_NO_SCROLLBAR")] NO_SCROLLBAR,
		[CCode(cname = "NK_WINDOW_TITLE")] TITLE,
		[CCode(cname = "NK_WINDOW_SCROLL_AUTO_HIDE")] SCROLL_AUTO_HIDE,
		[CCode(cname = "NK_WINDOW_BACKGROUND")] BACKGROUND,
		[CCode(cname = "NK_WINDOW_SCALE_LEFT")] SCALE_LEFT,
		[CCode(cname = "NK_WINDOW_NO_INPUT")] NO_INPUT
	}

	[SimpleType]
	[CCode(cname = "enum nk_command_type", has_type_id = false)]
	public enum CommandType {
		[CCode(cname = "NK_COMMAND_NOP")] NOP,
		[CCode(cname = "NK_COMMAND_SCISSOR")] SCISSOR,
		[CCode(cname = "NK_COMMAND_LINE")] LINE,
		[CCode(cname = "NK_COMMAND_CURVE")] CURVE,
		[CCode(cname = "NK_COMMAND_RECT")] RECT,
		[CCode(cname = "NK_COMMAND_RECT_FILLED")] RECT_FILLED,
		[CCode(cname = "NK_COMMAND_RECT_MULTI_COLOR")] RECT_MULTI_COLOR,
		[CCode(cname = "NK_COMMAND_CIRCLE")] CIRCLE,
		[CCode(cname = "NK_COMMAND_CIRCLE_FILLED")] CIRCLE_FILLED,
		[CCode(cname = "NK_COMMAND_ARC")] ARC,
		[CCode(cname = "NK_COMMAND_ARC_FILLED")] ARC_FILLED,
		[CCode(cname = "NK_COMMAND_TRIANGLE")] TRIANGLE,
		[CCode(cname = "NK_COMMAND_TRIANGLE_FILLED")] TRIANGLE_FILLED,
		[CCode(cname = "NK_COMMAND_POLYGON")] POLYGON,
		[CCode(cname = "NK_COMMAND_POLYGON_FILLED")] POLYGON_FILLED,
		[CCode(cname = "NK_COMMAND_POLYLINE")] POLYLINE,
		[CCode(cname = "NK_COMMAND_TEXT")] TEXT,
		[CCode(cname = "NK_COMMAND_IMAGE")] IMAGE,
		[CCode(cname = "NK_COMMAND_CUSTOM")] CUSTOM
	}

	[CCode(cname = "nk_text_width_f", has_target = false)]
	public delegate float TextWidth(Handle handle, float h, [CCode(array_length_cname="len")] string str, int len);

	[CCode(cname = "nk_query_font_glyph_f", has_target = false)]
	public delegate void QueryFontGlyph(Handle handle, float h, out UserFontGlyph glyph, Rune codepoint, Rune next_codepoint);

	[SimpleType]
	[CCode(cname = "nk_handle", has_destroy_function = false)]
	public struct Handle {
		void* ptr;
		int id;
	}

	[SimpleType]
	[CCode(cname = "struct nk_color", has_destroy_function = false)]
	public struct Color {
		uint8 r;
		uint8 g;
		uint8 b;
		uint8 a;
	}

	[SimpleType]
	[CCode(cname = "struct nk_vec2", has_destroy_function = false)]
	public struct Vec2 {
		float x;
		float y;
	}

	[SimpleType]
	[CCode(cname = "struct nk_vec2i", has_destroy_function = false)]
	public struct Vec2i {
		int x;
		int y;
	}

	[SimpleType]
	[CCode(cname = "struct nk_rect", has_type_id = false, has_destroy_function = false, has_construct_function = false, has_copy_function = false)]
	public struct Rect {
		float x;
		float y;
		float w;
		float h;
	}

	[SimpleType]
	[CCode(cname = "struct nk_user_font_glyph", has_destroy_function = false)]
	public struct UserFontGlyph {
		Vec2 uv[2];
		Vec2 offset;
		float width;
		float height;
		float xadvance;
	}

	[SimpleType]
	[CCode(cname = "struct nk_user_font", has_destroy_function = false)]
	public struct UserFont {
		Handle userdata;
		float height;
		TextWidth width;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command", has_destroy_function = false)]
	public struct Command {
		[CCode(cname = "nk__begin")]
		public static Command* begin(ref Context ctx);

		[CCode(cname = "nk__next")]
		public static Command* iterate(ref Context ctx, Command* cmd);

		CommandType type;
		size_t next;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_scissor", has_destroy_function = false)]
	public struct CommandScissor : Command {
		short x;
		short y;
		ushort w;
		ushort h;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_line", has_destroy_function = false)]
	public struct CommandLine : Command {
		ushort line_thickness;
		Vec2i begin;
		Vec2i end;
		Color color;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_curve", has_destroy_function = false)]
	public struct CommandCurve : Command {
		ushort line_thickness;
		Vec2i begin;
		Vec2i end;
		Vec2i ctrl[2];
		Color color;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_rect", has_destroy_function = false)]
	public struct CommandRect : Command {
		ushort rounding;
		ushort line_thickness;
		short x;
		short y;
		ushort w;
		ushort h;
		Color color;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_rect_filled", has_destroy_function = false)]
	public struct CommandRectFilled : Command {
		ushort rounding;
		short x;
		short y;
		ushort w;
		ushort h;
		Color color;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_rect_multi_color", has_destroy_function = false)]
	public struct CommandRectMultiColor : Command {
		short x;
		short y;
		ushort w;
		ushort h;
		Color left;
		Color top;
		Color bottom;
		Color right;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_triangle", has_destroy_function = false)]
	public struct CommandTriangle : Command {
		ushort line_thickness;
		Vec2i a;
		Vec2i b;
		Vec2i c;
		Color color;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_triangle_filled", has_destroy_function = false)]
	public struct CommandTriangleFilled : Command {
		Vec2i a;
		Vec2i b;
		Vec2i c;
		Color color;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_circle", has_destroy_function = false)]
	public struct CommandCircle : Command {
		short x;
		short y;
		ushort line_thickness;
		ushort w;
		ushort h;
		Color color;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_circle_filled", has_destroy_function = false)]
	public struct CommandCircleFilled : Command {
		short x;
		short y;
		ushort w;
		ushort h;
		Color color;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_arc", has_destroy_function = false)]
	public struct CommandArc : Command {
		short cx;
		short cy;
		ushort r;
		ushort line_thickness;
		float a[2];
		Color color;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_arc_filled", has_destroy_function = false)]
	public struct CommandArcFilled : Command {
		short cx;
		short cy;
		ushort r;
		float a[2];
		Color color;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_polygon", has_destroy_function = false)]
	public struct CommandPolygon : Command {
		Color color;
		ushort line_thickness;
		ushort point_count;
		[CCode(array_length_cname = "point_count")] Vec2i[] points;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_polygon_filled", has_destroy_function = false)]
	public struct CommandPolygonFilled : Command {
		Color color;
		ushort point_count;
		[CCode(array_length_cname = "point_count")] Vec2i[] points;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_polyline", has_destroy_function = false)]
	public struct CommandPolyline : Command {
		Color color;
		ushort line_thickness;
		ushort point_count;
		[CCode(array_length_cname = "point_count")] Vec2i[] points;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_image", has_destroy_function = false)]
	public struct CommandImage : Command {
		short x;
		short y;
		ushort w;
		ushort h;
		Image img;
		Color col;
	}

	[SimpleType]
	[CCode(cname = "struct nk_command_text", has_destroy_function = false)]
	public struct CommandText : Command {
		UserFont* font;
		Color background;
		Color foreground;
		short x;
		short y;
		ushort w;
		ushort h;
		float height;
		int length;
		[CCode(cname = "string", array_length_cname = "length")] string text;
	}

	[SimpleType]
	[CCode(cname = "struct nk_image", has_destroy_function = false)]
	public struct Image {
		Handle handle;
		ushort w;
		ushort h;
		ushort region[4];
	}

	[SimpleType]
	[CCode(cname = "struct nk_input", has_destroy_function = false)]
	public struct Input {}

	[SimpleType]
	[CCode(cname = "struct nk_style", has_destroy_function = false)]
	public struct Style {
		UserFont* font;
	}

	[Compact]
	[CCode(cname = "struct nk_context")]
	public struct Context {
		public Input input;
		public Style style;
	}

	[CCode(cname = "nk_init_default")]
	public bool init_default(out Context ctx, UserFont* font);

	[CCode(cname = "nk_clear")]
	public void clear(ref Context ctx);

	[CCode(cname = "nk_begin")]
	public bool begin(ref Context ctx, string title, Rect bounds, Flags flags);

	[CCode(cname = "nk_end")]
	public void end(ref Context ctx);

	[CCode(cname = "nk_layout_row_static")]
	public void layout_row_static(ref Context ctx, float height, int item_width, int cols);

	[CCode(cname = "nk_layout_row_dynamic")]
	public void layout_row_dynamic(ref Context ctx, float height, int cols);

	[CCode(cname = "nk_layout_row_begin")]
	public void layout_row_begin(ref Context ctx, LayoutFormat fmt, float row_height, int cols);

	[CCode(cname = "nk_layout_row_end")]
	public void layout_row_end(ref Context ctx);

	[CCode(cname = "nk_layout_row_push")]
	public void layout_row_push(ref Context ctx, float val);

	[CCode(cname = "nk_label")]
	public void label(ref Context ctx, string title, Nk.Flags align);

	[CCode(cname = "nk_slider_float")]
	public void slider_float(ref Context ctx, float min, float* val, float max, float step);

	[CCode(cname = "nk_button_label")]
	public bool button_label(ref Context ctx, string title);

	[CCode(cname = "nk_option_label")]
	public bool option_label(ref Context ctx, string title, bool active);
}
