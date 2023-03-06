INCLUDE_FILES = -iquotesrc
INCLUDE_FILES += -I lib/SDL/include

LIB_LINKS = -lm
LIB_LINKS += $(shell bin/sdl/sdl2-config --prefix=bin --static-libs)

default: all

all: build clean

clean:
	-rm -f bin/*.o


mkbin:
	mkdir -p bin/src

%.o: src/%.c mkbin
	cc -o bin/src/$@ -c $(INCLUDE_FILES) $<

build: application.o
	cc -o bin/app bin/src/application.o $(LIB_LINKS)

sdl:
	mkdir -p bin/sdl
	cmake -S lib/SDL -B bin/sdl
	cd bin/sdl && make -j 10
	chmod +x bin/sdl/sdl2-config
	mkdir -p bin/lib
	cp bin/sdl/libSDL2.a bin/lib