#include <gtk/gtk.h>

static void add_label(GtkWidget *box, const char *css_class)
{
	GtkWidget *label = gtk_label_new("Hello 日本語テキスト World");
	gtk_label_set_xalign(GTK_LABEL(label), 0);
	gtk_widget_add_css_class(label, css_class);
	gtk_box_append(GTK_BOX(box), label);
}

/* Render the box with the window's renderer after the first frame */
static void after_paint(GdkFrameClock *clock, GtkWidget *box)
{
	g_signal_handlers_disconnect_by_func(clock, after_paint, box);

	int w = gtk_widget_get_width(box);
	int h = gtk_widget_get_height(box);
	GdkPaintable *paintable = gtk_widget_paintable_new(box);
	GtkSnapshot *snapshot = gtk_snapshot_new();
	gdk_paintable_snapshot(paintable, snapshot, w, h);
	GskRenderNode *node = gtk_snapshot_free_to_node(snapshot);
	GskRenderer *renderer = gtk_native_get_renderer(gtk_widget_get_native(box));
	GdkTexture *texture = gsk_renderer_render_texture(renderer, node,
		&GRAPHENE_RECT_INIT(0, 0, w, h));
	gdk_texture_save_to_png(texture, "gtk4.png");

	g_object_unref(texture);
	gsk_render_node_unref(node);
	g_object_unref(paintable);
	g_application_quit(g_application_get_default());
}

static void map(GtkWidget *window, GtkWidget *box)
{
	g_signal_connect(gtk_widget_get_frame_clock(window), "after-paint",
		G_CALLBACK(after_paint), box);
}

static void activate(GtkApplication *app)
{
	GtkCssProvider *css = gtk_css_provider_new();
	gtk_css_provider_load_from_string(css,
		"label { font-family: 'Noto Sans'; font-size: 16px; padding: 10px; }"
		".light { background: white; color: black; }"
		".dark { background: black; color: white; }");
	gtk_style_context_add_provider_for_display(gdk_display_get_default(),
		GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_size_request(box, 400, -1);
	add_label(box, "light");
	add_label(box, "dark");

	GtkWidget *window = gtk_application_window_new(app);
	gtk_window_set_child(GTK_WINDOW(window), box);
	g_signal_connect(window, "map", G_CALLBACK(map), box);
	gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
	GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	return g_application_run(G_APPLICATION(app), argc, argv);
}
