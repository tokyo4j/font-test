all: cairo gtk4 qt6

cairo: cairo.c
	$(CC) -o $@ $< $(shell pkg-config --cflags --libs pangocairo)

gtk4: gtk4.c
	$(CC) -o $@ $< $(shell pkg-config --cflags --libs gtk4)

qt6: qt6.cpp
	$(CXX) -fPIC -o $@ $< $(shell pkg-config --cflags --libs Qt6Widgets)

clean:
	rm -f cairo gtk4 qt6 *.png
