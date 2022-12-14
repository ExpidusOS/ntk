namespace NtkExampleGtkBasic {
  public class Window : Gtk.ApplicationWindow {
    private NtkGtk4.Widget widget;
    private Ntk.InputDispatcher input;
    private Ntk.Context ntk;

    public Window(Gtk.Application app) {
      Object(application: app);
    }

    construct {
      this.widget = new NtkGtk4.Widget();
      this.widget.set_hexpand(true);
      this.widget.set_vexpand(true);
      this.child = this.widget;

      this.input = new NtkGtk4.InputDispatcher(this.widget);

      try {
        this.ntk = new Ntk.Context(this.widget.renderer, this.widget.renderer.get_font("Droid Sans Regular", 12));
      } catch (GLib.Error e) {
        GLib.error("Failed to initialize: %s:%d: %s", e.domain.to_string(), e.code, e.message);
      }

      this.input.attach(this.ntk);

      this.ntk.rendered.connect(() => {
        this.widget.queue_draw();
      });

      var opt = false;
      float val = 0.55f;

      this.widget.renderer.request_draw.connect(() => {
        try {
          this.ntk.render((ntk, nk) => {
            Nk.Rect rect = {};
            rect.x = 50;
            rect.y = 50;

            rect.w = 220;
            rect.h = 220;

            if (nk.begin("Hello, world", rect, Nk.PanelFlags.BORDER | Nk.PanelFlags.TITLE | Nk.PanelFlags.MOVABLE | Nk.PanelFlags.CLOSABLE)) {
              nk.layout_row_static(30, 80, 1);
              if (nk.button_label("button")) GLib.message("Hello, world");

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
      });

      this.set_default_size(800, 600);
    }
  }

  public class Application : Gtk.Application {
    public Application() {
      Object(application_id: "com.expidus.ntk.example.basic", flags: GLib.ApplicationFlags.FLAGS_NONE);
    }

    public override void activate() {
      base.activate();

      if (this.get_windows().length() == 0) {
        var win = new Window(this);
        this.add_window(win);
        win.show();
      } else {
        this.get_windows().nth_data(0).show();
      }
    }
  }
}

public static int main(string[] args) {
  return new NtkExampleGtkBasic.Application().run(args);
}
