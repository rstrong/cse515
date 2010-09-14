CC=g++
CFLAGS=-I/usr/include/ImageMagick -Wall
MAGICK=`Magick++-config --cppflags --cxxflags --ldflags --libs`

segment: 
	$(CC) Segment.cpp -o segment  $(CFLAGS) $(MAGICK)

clean:
	rm -f segment segment.o
