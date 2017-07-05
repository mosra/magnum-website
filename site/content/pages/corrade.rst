Corrade
#######

    *corrade* (v.) --- "To scrape together, to gather together from various sources"

Corrade is multiplatform utility library written in C++11/C++14, originally
developed for *Kompas* and now used in Magnum.

.. todo: Download latest

Supported platforms
===================

-   **Linux** and embedded Linux
-   **Windows**
-   **Mac OS X** (thanks to :gh:`David Lin <davll>`)
-   **Google Chrome** (through `Native Client <https://developers.google.com/native-client/>`_,
    both ``newlib`` and ``glibc`` toolchains are supported)
-   **HTML5/JavaScript** (through :gh:`Emscripten <kripken/emscripten>`)
-   **Android**

Features
========

-   Signal/slot connection library with compile-time argument checking.
-   Plugin management library with static and dynamic plugins and dependency
    handling.
-   Easy-to-use unit test framework and extensible debugging output.
-   Various utilities to ease multiplatform development.

You might want to read also
`official project announcement <{filename}/blog/announcements/introducing-corrade.rst>`_
or notes from latest `May 2015 release <{filename}/blog/announcements/may-2015-updates.rst#corrade>`_.

Documentation, getting started
==============================

Documentation is available :dox:`for online viewing <corrade>`, you can also
generate it directly from the source code, see above download instructions for
more information.

License
=======

Corrade is licensed under MIT/Expat license:

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015 Vladimír
    Vondruš <mosra@centrum.cz>

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
