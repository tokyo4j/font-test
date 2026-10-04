#include <pango/pangocairo.h>

static void draw(cairo_t *cr, double y, double bg, double fg)
{
	cairo_set_source_rgb(cr, bg, bg, bg);
	cairo_rectangle(cr, 0, y, 400, 50);
	cairo_fill(cr);

	PangoLayout *layout = pango_cairo_create_layout(cr);
	PangoFontDescription *desc = pango_font_description_from_string("Noto Sans 16px");
	pango_layout_set_font_description(layout, desc);
	pango_layout_set_text(layout, "Hello 日本語テキスト World", -1);
	cairo_set_source_rgb(cr, fg, fg, fg);
	cairo_move_to(cr, 10, y + 10);
	pango_cairo_show_layout(cr, layout);
	pango_font_description_free(desc);
	g_object_unref(layout);
}

int main(void)
{
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, 400, 100);
	cairo_t *cr = cairo_create(surface);
	draw(cr, 0, 1, 0);
	draw(cr, 50, 0, 1);
	cairo_surface_write_to_png(surface, "cairo.png");
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	return 0;
}
