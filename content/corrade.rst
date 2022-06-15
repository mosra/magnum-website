Corrade
#######

:cover: {static}/img/cover-corrade.jpg
:summary: A multiplatform utility library written in C++11/C++14

..

    *corrade* (v.) --- "To scrape together, to gather together from various sources"

Corrade is a multiplatform utility library written in C++11/C++14. It's used as
a base for the Magnum graphics engine, among other things.

-   Documentation --- https://doc.magnum.graphics/corrade/
-   GitHub project page --- https://github.com/mosra/corrade

Supported platforms
===================

-   **Linux** and embedded Linux
-   **Windows**, **Windows RT** (Store/Phone)
-   **macOS**, **iOS**
-   **Android**
-   **Web** (`asm.js <http://asmjs.org/>`_ or `WebAssembly <https://webassembly.org/>`_),
    through `Emscripten <https://kripken.github.io/emscripten-site/>`_

See the `Build Status page <https://magnum.graphics/build-status/>`_ for
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

Check also the `Feature Overview pages <https://magnum.graphics/features/>`_
for further information.

Documentation, getting started
==============================

Head over :dox:`to the documentation <corrade>` for building instructions.
The best way to get started is to read some
:dox:`examples and tutorials <corrade-example-index>`.

License
=======

Corrade is licensed under the MIT/Expat license:

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
    2017, 2018, 2019, 2020, 2021, 2022 Vladimír Vondruš <mosra@centrum.cz> and
    contributors

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

All :dox:`Corrade example code <corrade-example-index>` is put into public
domain (or UNLICENSE) to free you from any legal obstacles when reusing the
code in your projects:

    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or distribute
    this software, either in source code form or as a compiled binary, for any
    purpose, commercial or non-commercial, and by any means.

    In jurisdictions that recognize copyright laws, the author or authors of
    this software dedicate any and all copyright interest in the software to
    the public domain. We make this dedication for the benefit of the public
    at large and to the detriment of our heirs and successors. We intend this
    dedication to be an overt act of relinquishment in perpetuity of all
    present and future rights to this software under copyright law.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

    For more information, please refer to <https://unlicense.org/>

Some Corrade libraries, plugins and executables depend on third-party software.
A full list together with license info is maintained
:dox:`in the documentation <corrade-credits-third-party>`. There's also a
detailed :dox:`info about contributors <corrade-credits-contributors>`.
