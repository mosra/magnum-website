Introducing Corrade, an plugin management and utility library
#############################################################

:date: 2012-02-08
:category: Announcements
:tags: C++, Corrade, Qt
:summary: Originally developed for *Kompas*, this library provides both static
    and dynamic plugin management with dependency support and also various
    filesystem, portability and debugging utilities.

.. role:: cpp(code)
    :language: c++
    :class: highlight

..

    *corrade* (v.) --- "To scrape together, to gather together from various
    sources"

Features
========

-   Easy-to-use plugin management library with support for both static and
    dynamic plugins and also dependency handling
-   INI-style configuration parser and writer with hierarchic group support
-   Classes for easy debug/error output with support for printing out custom
    types, inspired with QDebug
-   Manager for resources compiled directly into executables, filesystem,
    endianness utilites and more
-   Written in pure C++ with no external dependencies, using CMake build system
-   Strong code coverage with unit tests

Get Corrade
===========

Current version is 0.2 (previous version 0.1 was part of *Kompas*), you can
download packages for your favorite distribution, build from source or view
documentation at download page:

.. todo: button Corrade Download 0.2

Future plans
============

Because I use this library in another project (and that's also why I split it
from Kompas), many new features are planned, such as unit testing suite aiming
to be more tailored for my needs than QtTest or C++11-only features, such as
easy printing of iterable containers to debug output. This feature is already
in experimental :gh:`branch c++11 on GitHub <mosra/corrade$c++11>`.

Main focus will be on support for plugins written in other languages besides
C++ (such as Java or Python). This is also school project which I have to have
done.

Speaking of Kompas, next version will depend on this library and it will be
released soon (read: in less than a year, as I'm currently having some problems
with Windows build).
