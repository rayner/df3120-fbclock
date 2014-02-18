
# Edit INCFLAGS and LIBFLAGS to point to the minifs build directory where
# the include/library files for the DF3120 are installed.
INCFLAGS = -I../minifs/build-df3120/staging/usr/include
LIBFLAGS = -L../minifs/build-df3120/staging/usr/lib

CC = arm-v4t-linux-uclibcgnueabi-gcc
CFLAGS = -Wall -pedantic -ansi -static
LIBS = -lpng -lz -lm
SRCS = fbclock.c framebuffer.c
HEADERS = fbclock.h framebuffer.h pngimage.h

.PHONY: all clean


all: fbclock


fbclock: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(INCFLAGS) $(LIBFLAGS) -o $@ $(SRCS) $(LIBS)

clean:
	rm -f fbclock *~

