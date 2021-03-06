CFLAGS += -Igodot-headers -Ihigh-level-gdnative -std=c11

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0
else
	CFLAGS += -O3
endif

all: windows64 windows32 linux64 linux32 wasm32

# Cross compilers for windows targets if not running windows
# TODO: test/fix when running on windows
ifneq ($(OS),Windows_NT)
windows32: CC := i686-w64-mingw32-cc
windows64: CC := x86_64-w64-mingw32-cc
endif
# Emscripten compiler for WebAssembly (WASM) libraries
%.wasm: CC := emcc

example_windows64.dll: example.c high-level-gdnative/hgdn.h
	$(CC) $< -o $@ $(CFLAGS) -shared

example_windows32.dll: example.c high-level-gdnative/hgdn.h
	$(CC) $< -o $@ $(CFLAGS) -shared -m32

example_linux64.so: example.c high-level-gdnative/hgdn.h
	$(CC) $< -o $@ $(CFLAGS) -shared -fPIC

example_linux32.so: example.c high-level-gdnative/hgdn.h
	$(CC) $< -o $@ $(CFLAGS) -shared -fPIC -m32 

example_wasm32.wasm: example.c high-level-gdnative/hgdn.h
	$(CC) $< -o $@ $(CFLAGS) -s SIDE_MODULE=1

windows32: example_windows32.dll
windows64: example_windows64.dll
linux32: example_linux32.so
linux64: example_linux64.so
wasm32: example_wasm32.wasm

.PHONY: clean
clean:
	rm -f *.dll *.so *.wasm
