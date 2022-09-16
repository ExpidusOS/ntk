namespace NtkExampleCairoBasic {
  public class Application : GLib.Application {
    public Application() {
      Object(application_id: "com.expidus.ntk.example.basic", flags: GLib.ApplicationFlags.FLAGS_NONE);
    }

    public override void activate() {
      base.activate();

      try {
        var renderer = new NtkCairo.Renderer(1024, 768);
        var ntk = new Ntk.Context(renderer, new NtkPango.Font.with_context(Pango.cairo_create_context(renderer.context), Pango.FontDescription.from_string("Droid Sans Regular 12px")));

        renderer.rendered.connect(() => {
          renderer.surface.write_to_png("cairo-basic.png");
        });

        var opt = false;
        float val = 0.55f;

        try {
          ntk.render((ntk, nk) => {
            Nk.Rect rect = {};
            rect.x = 50;
            rect.y = 50;

            rect.w = 220;
            rect.h = 220;

            if (nk.begin("Hello, world", rect, Nk.PanelFlags.BORDER | Nk.PanelFlags.TITLE | Nk.PanelFlags.MOVABLE | Nk.PanelFlags.CLOSABLE)) {
              nk.layout_row_static(30, 80, 1);
              nk.button_label("button");

              nk.layout_row_dynamic(30, 2);
              if (nk.option_label("easy", opt)) opt = false;
              if (nk.option_label("hard", !opt)) opt = true;

              nk.layout_row_begin(Nk.LayoutFormat.STATIC, 30, 2);

              nk.layout_row_push(50);
              nk.label("Volume: ", Nk.TextAlignment.LEFT);

              nk.layout_row_push(110);
              nk.slider_float(0, &val, 1.0f, 0.1f);

              nk.layout_row_end();
            }

            nk.end();
          });
        } catch (GLib.Error e) {
          GLib.error("Failed to render: %s:%d: %s", e.domain.to_string(), e.code, e.message);
        }
      } catch (GLib.Error e) {
        GLib.error("Failed to initialize: %s:%d: %s", e.domain.to_string(), e.code, e.message);
      }
    }
  }
}

public static int main(string[] args) {
  return new NtkExampleCairoBasic.Application().run(args);
}
