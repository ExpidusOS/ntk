namespace NtkExampleEGLBasic {
  public class Application : GLib.Application {
    public Application() {
      Object(application_id: "com.expidus.ntk.example.basic", flags: GLib.ApplicationFlags.FLAGS_NONE);
    }

    public override void activate() {
      base.activate();

      try {
        var renderer = new NtkEGL.Renderer(new NtkHW.DrmRender());
        
        /* renderer.render.connect((buffer, horiz, vert) => {
          var surf = new Cairo.ImageSurface.for_data((uchar[])buffer, Cairo.Format.ARGB32, horiz, vert, Cairo.Format.ARGB32.stride_for_width(horiz));
          surf.write_to_png("egl-basic.png");
        }); */

        var ntk = new Ntk.Context(renderer, renderer.get_font("Droid Sans Regular", 12));

        var opt = false;
        float val = 0.55f;

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
        GLib.error("Failed to initalize or render: %s:%d: %s", e.domain.to_string(), e.code, e.message);
      }
    }
  }
}

public static int main(string[] args) {
  return new NtkExampleEGLBasic.Application().run(args);
}
