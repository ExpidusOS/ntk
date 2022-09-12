namespace NtkExampleGtkBasic {
	public class Window : TokyoGtk.ApplicationWindow {
		private NtkRendererGtk4.Widget widget;
		private Ntk.Context ntk;

		public Window(Gtk.Application app) {
			Object(application: app);
		}

		construct {
			this.widget = new NtkRendererGtk4.Widget();
			this.widget.set_hexpand(true);
			this.widget.set_vexpand(true);
			this.get_box().append(this.widget);

			this.ntk = new Ntk.Context(this.widget.renderer);

			this.ntk.rendered.connect(() => {
				this.widget.queue_draw();
			});

			this.widget.renderer.request_draw.connect(() => {
				try {
					this.ntk.render((ntk, nk) => {
          });
				} catch (GLib.Error e) {}
			});
		}
	}

	public class Application : TokyoGtk.Application {
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
