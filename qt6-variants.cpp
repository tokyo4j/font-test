#include <QGuiApplication>
#include <QImage>
#include <QPainter>
#include <QRawFont>
#include <QTextLayout>
#include <cstdio>
#include <cstring>
#include <ft2build.h>
#include FT_MODULE_H

// Not public API, but exported from libQt6Gui
FT_Library qt_getFreetype();

static const QString text = "Hello 日本語テキスト World";

struct Variant {
	const char *name;
	QFont font;
	QString text = ::text;
};

static void print_runs(const Variant &v)
{
	QTextLayout layout(v.text, v.font);
	layout.beginLayout();
	layout.createLine();
	layout.endLayout();
	printf("%s:\n", v.name);
	for (const QGlyphRun &run : layout.glyphRuns()) {
		QRawFont raw = run.rawFont();
		printf("  %-18s %-10s weight=%d glyphs=%lld\n",
		       qPrintable(raw.familyName()), qPrintable(raw.styleName()),
		       raw.weight(), (long long)run.glyphIndexes().size());
	}
}

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);

	// Qt enables CFF stem darkening and, for fonts using it, blends glyphs
	// with gamma correction. Turning it off must happen before fonts load.
	bool no_stem_darkening = argc > 1 && !strcmp(argv[1], "--no-stem-darkening");
	if (no_stem_darkening) {
		FT_Bool value = true;
		FT_Property_Set(qt_getFreetype(), "cff", "no-stem-darkening", &value);
	}

	QFont base("Noto Sans");
	base.setPixelSize(16);

	QList<Variant> variants;
	variants.append({"default", base});
	{
		QFont f = base;
		f.setStyleStrategy(QFont::NoAntialias);
		variants.append({"NoAntialias", f});
	}
	{
		QFont f = base;
		f.setHintingPreference(QFont::PreferNoHinting);
		variants.append({"PreferNoHinting", f});
	}
	{
		QFont f = base;
		f.setHintingPreference(QFont::PreferVerticalHinting);
		variants.append({"PreferVerticalHinting", f});
	}
	{
		QFont f = base;
		f.setHintingPreference(QFont::PreferFullHinting);
		variants.append({"PreferFullHinting", f});
	}
	{
		QFont f("Noto Sans CJK JP");
		f.setPixelSize(16);
		variants.append({"Noto Sans CJK JP", f});
	}
	variants.append({"Japanese only", base, "日本語テキスト"});

	const int row = 40, col = 300, label = 180;
	QImage image(label + 2 * col, row * variants.size(), QImage::Format_RGB32);
	image.fill(Qt::white);
	QPainter p(&image);
	for (int i = 0; i < variants.size(); i++) {
		const Variant &v = variants[i];
		int y = i * row;
		p.setFont(base);
		p.setPen(Qt::gray);
		p.drawText(QRect(10, y, label - 10, row), Qt::AlignVCenter, v.name);
		p.setFont(v.font);
		p.fillRect(label + col, y, col, row, Qt::black);
		p.setPen(Qt::black);
		p.drawText(QRect(label + 10, y, col - 10, row), Qt::AlignVCenter, v.text);
		p.setPen(Qt::white);
		p.drawText(QRect(label + col + 10, y, col - 10, row), Qt::AlignVCenter, v.text);
		print_runs(v);
	}
	p.end();
	image.save(no_stem_darkening ? "qt6-variants-no-stem-darkening.png" : "qt6-variants.png");
	return 0;
}
