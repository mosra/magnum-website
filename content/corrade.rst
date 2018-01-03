Corrade
#######

:cover: {filename}/img/cover-corrade.jpg
:summary: A multiplatform utility library written in C++11/C++14

..

    *corrade* (v.) --- "To scrape together, to gather together from various sources"

Corrade is a multiplatform utility library written in C++11/C++14. It's used as
a base for the Magnum graphics engine, among other things.

-   Documentation --- http://doc.magnum.graphics/corrade/
-   GitHub project page --- https://github.com/mosra/corrade

Supported platforms
===================

-   **Linux** and embedded Linux
-   **Windows**, **Windows RT** (Store/Phone)
-   **macOS**, **iOS**
-   **Android**
-   **Web** (`asm.js <http://asmjs.org/>`_ or `WebAssembly <http://webassembly.org/>`_),
    through `Emscripten <http://kripken.github.io/emscripten-site/>`_

See the `Build Status page <http://magnum.graphics/build-status/>`_ for
detailed per-platform build status.

Features
========

-   Low-level utilities to bridge platform differences when accessing OS
    functionality, filesystem, console and environment
-   Lightweight container implementations, complementing STL features with
    focus on compilation speed, ease of use and performance
-   Test framework emphasizing flexibility, extensibility, minimal use of
    macros and clarity of diagnostic output
-   Plugin management library with static and dynamic plugins, dependency
    handling and hot code reload
-   Signal/slot connection library with full type safety

Check also the `Feature Overview pages <http://magnum.graphics/features/>`_ for
further information.

Documentation, getting started
==============================

Head over :dox:`to the documentation <corrade>` for building instructions.
The best way to get started is to read some
:dox:`examples and tutorials <corrade-example-index>`.

License
=======

Corrade is licensed under the MIT/Expat license:

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
    2017, 2018 Vladimír Vondruš <mosra@centrum.cz> and contributors

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
