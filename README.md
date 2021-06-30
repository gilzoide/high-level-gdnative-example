# Example project for [High level GDNative C/C++ API (HGDN)](https://github.com/gilzoide/high-level-gdnative)
This project is a working example of HGDN usage. The source C code is located
in the [gdnative](gdnative) folder, with a [Makefile](gdnative/Makefile) for
building it.

The `GDNativeLibrary` Resource [gdnative/example.gdnlib](gdnative/example.gdnlib)
is configured with the Makefile targets' outputs and there is an example of
calling the functions directly using `GDNative.call_native` from GDScript in
the [example_call_native.gd](example_call_native.gd) file.


## Building the GDNative library
Run `make <target>` command from the `gdnative` folder. If running from project
root, add `-C gdnative` to tell `make` that the Makefile is in the `gdnative`
folder. Available targets are the following:
```sh
# Build for Windows x32. This uses MinGW if not running in Windows itself
$ make -C gdnative windows32
# Build for Windows x64. This uses MinGW if not running in Windows itself
$ make -C gdnative windows64
# Build for Linux x32
$ make -C gdnative linux32
# Build for Linux x64
$ make -C gdnative linux64
# Build for WebAssembly with Emscripten. HTML5 export preset must have GDNative enabled.
$ make -C gdnative wasm32

# Shorthand for building all targets described above at once
$ make -C gdnative all
```
TODO:
- Test Makefile on Windows, possibly will work only for Cygwin/MinGW
- Add OSX, Android and iOS targets
