#include <QGuiApplication>
#include <QImage>
#include <QPainter>

static void draw(QPainter &p, int y, QColor bg, QColor fg)
{
	p.fillRect(0, y, 400, 50, bg);
	p.setPen(fg);
	p.drawText(QRect(10, y, 380, 50), Qt::AlignVCenter, "Hello 日本語テキスト World");
}

int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);
	QImage image(400, 100, QImage::Format_RGB32);
	QPainter p(&image);
	QFont font("Noto Sans");
	font.setPixelSize(16);
	p.setFont(font);
	draw(p, 0, Qt::white, Qt::black);
	draw(p, 50, Qt::black, Qt::white);
	p.end();
	image.save("qt6.png");
	return 0;
}
