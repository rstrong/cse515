CC=g++
# Use commented line for linux
#CFLAGS=-I/usr/include/ImageMagick -Wall
CFLAGS=-I/usr/local/include/ImageMagick -Wall
MAGICK=`Magick++-config --cppflags --cxxflags --ldflags --libs`

segment: 
	$(CC) Segment.cpp -o segment  $(CFLAGS) $(MAGICK)

clean:
	rm -f segment segment.o
