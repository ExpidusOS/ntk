namespace NtkExampleCairoBasic {
  public class Application : GLib.Application {
    public Application() {
			Object(application_id: "com.expidus.ntk.example.basic", flags: GLib.ApplicationFlags.FLAGS_NONE);
		}

    public override void activate() {
      base.activate();

      var renderer = new NtkRendererCairo.Renderer(1024, 768);
      var ntk = new Ntk.Context(renderer);

      ntk.rendered.connect(() => {
        ntk.renderer.request_draw();
        renderer.surface.write_to_png("cairo-basic.png");
      });

      try {
        ntk.render();
      } catch (GLib.Error e) {
      }
    }
  }
}

public static int main(string[] args) {
  return new NtkExampleCairoBasic.Application().run(args);
}
