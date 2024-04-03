# InternetMessage
[RFC 2822](https://datatracker.ietf.org/doc/html/rfc2822) "Internet Message Format"

* The Internet Message Format is format in which text messages are transferred over the Internet. Where SMTP is equivalent to the message envelope, IMF is equivalent to the letter within the envelope. It contains the originator, recipients, subject and dates. Whilst IMF only handles text messages, it can be augmented with MIME_multipart to support multi-media messages.

## Building the C++ Implementation

A portable library is built which depends only on the C++11 compiler and
standard library, so it should be supported on almost any platform.  The
following are recommended toolchains for popular platforms.

* Windows -- [Visual Studio](https://www.visualstudio.com/) (Microsoft Visual
  C++)
* Linux -- clang or gcc
* MacOS -- Xcode (clang)

This library is not intended to stand alone.  It is intended to be included in
a larger solution which uses [CMake](https://cmake.org/) to generate the build
system and build applications which will link with the library.

There are two distinct steps in the build process:

1. Generation of the build system, using CMake
2. Compiling, linking, etc., using CMake-compatible toolchain

### Prerequisites

* [CMake](https://cmake.org/) version 3.8 or newer
* C++11 toolchain compatible with CMake for your development platform (e.g.
  [Visual Studio](https://www.visualstudio.com/) on Windows)

### Build system generation

Generate the build system using [CMake](https://cmake.org/) from the solution
root.  For example:

```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A "x64" ..
```

### Compiling, linking, etc...

Either use [CMake](https://cmake.org/) or your toolchain's IDE to build.
For [CMake](https://cmake.org/):

```bash
cd build
cmake --build . --config Release
```

## License

Licensed under the [MIT license](LICENSE.txt).