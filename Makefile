INCLUDE_FILES = -I lib/SDL/include

CHENGE_FILES = $(wildcard bin/chenge/*.o)

LIB_LINKS = -lm
LIB_LINKS += $(shell bin/sdl/sdl2-config --prefix=bin --static-libs)

default: all

all: build

clean:
	-rm -rf bin/src

mkbin:
	mkdir -p bin/src

%.o: src/%.c mkbin
	cc -o bin/src/$@ -c $(INCLUDE_FILES) $<

build: window.o
	cc -o bin/app bin/src/window.o $(CHENGE_FILES) $(LIB_LINKS)

pong: pong.o
	cc -o bin/pong bin/src/pong.o $(LIB_LINKS)

chenge:
	mkdir -p bin/chenge
	cd src/CHenge && for i in *.c; do cc -o ../../bin/chenge/$${i%%.*}.o -c $(INCLUDE_FILES) $${i}; done

sdl:
	mkdir -p bin/sdl
	cmake -S lib/SDL -B bin/sdl
	cd bin/sdl && make -j 10
	chmod +x bin/sdl/sdl2-config
	mkdir -p bin/lib
	cp bin/sdl/libSDL2.a bin/lib