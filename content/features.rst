Features
########

:cover: {filename}/img/cover-features.jpg
:summary: High-level overview of feature set offered by the Magnum engine

.. role:: dox-dim(dox)
    :class: m-dim

.. container:: m-container-inflate

    .. raw:: html
        :file: features.svg

The following pages provide a high-level overview of the feature set offered by
Magnum. For greater detail please read documentation linked from the sections
below. You can also click on various parts of the architecture diagram.

`Our "Zen Garden" philosophy`_
==============================

-   Consistent coding, building, testing and deploying experience across all
    platforms
-   Lightweight core with modular and extensible addons that enable diverse use
    cases
-   Clean code with high test coverage, `built and tested at least once a week <{filename}/build-status.rst>`_
    on all platforms to ensure continuous reliability and stability
-   Actively maintained :dox:`extensive documentation <magnum>`
-   First-class support and friendly community (`Gitter chat <https://gitter.im/mosra/magnum>`_,
    `mailing list <https://groups.google.com/forum/#!forum/magnum-engine>`_)
-   **Premium support and consulting offers** (see the
    `Premium Services <{filename}/premium.rst>`_ page for details)

`Supported platforms`_
======================

-   **Linux** and embedded Linux
-   **Windows** with both MSVC and MinGW, **Windows RT** (Store/Phone)
-   **macOS**, **iOS**
-   **Android**
-   **Web** (`WebAssembly <http://webassembly.org/>`_ or `asm.js <http://asmjs.org/>`_),
    through `Emscripten <http://kripken.github.io/emscripten-site/>`_

.. note-dim::

    See the :dox:`building` documentation for detailed platform requirements.

`Platform abstraction`_
=======================

*Corrade* is a standalone library that forms the base dependency for Magnum. In
its minimal form it provides platform abstraction and low-level utilities that
are not directly related to graphics. See the `Corrade project page <{filename}/corrade.rst>`_
for details.

`Low-level utilities`_
----------------------

Set of convenience classes and functions to make writing of command-line
applications actually enjoyable; with minimal differences across various
desktop, mobile and web platforms.

-   OS, filesystem and terminal environment access with consistent UTF-8
    encoding across all platforms
-   Colored output that prints to console on desktop and iOS, browser console
    on web and *logcat* on Android
-   String, Unicode, hashing and other utilities

.. note-dim::

    See documentation of the :dox:`Utility` namespace for details.

`Simple containers`_
--------------------

Data structure implementations that have no direct equivalent in the STL or are
better tailored to specific use cases than equivalent general standard
implementations. Some are present simply to bridge differences between C++11,
C++14 and C++17.

-   Lightweight C array wrappers with ability to control item initialization
-   Non-owning array views with utilities for array slicing and casting
-   Type-safe bitfields, intrusive list and more

.. note-dim::

    See documentation of the :dox:`Containers` namespace for details.

`Core API`_
===========

The base Magnum library contains the minimal set of APIs for graphics
development. Apart from Corrade it has no external dependencies to make porting
to new platforms as frictionless as possible.

`Math library`_
---------------

Designed primarily for graphics with emphasis on implementation simplicity and
debug performance. Provides vector and matrix operations, complex numbers,
quaternions and their dual counterparts. Intuitive transformation APIs in both
2D and 3D.

-   Strong types and literals for entering radians, degrees and half-floats
-   Hex color literals, sRGB, HSV and XYZ colorspace support
-   Distance and intersection calculation, matrix decomposition algorithms

.. note-dim::

    See documentation of the :dox:`types`, :dox:`matrix-vector` and
    :dox:`transformations` for details.

`Extra Functionality » <{filename}/features/extras.rst>`_
=========================================================

While you can stay with just the core Magnum library, there are additional
opt-in libraries and tools providing features that can make development of your
application easier. OpenGL and Vulkan wrapper layer, APIs for asset management,
mesh, texture and shader tools, debugging and prototyping utilities,
command-line utilities and much more.

`Plugins & Extensions » <{filename}/features/extensions.rst>`_
==============================================================

The libraries are extensible via plugins for asset loading and conversion or,
for example, text rendering. Magnum provides a rich set of plugins for loading
common image and scene formats, audio formats and image format conversion. It's
also possible to integrate math library of your choice for seamless use with
core Magnum APIs.

`Community Contributions » <{filename}/features/community.rst>`_
================================================================

On top of everything else, there are libraries, tools and concepts built by the
community --- not part of Magnum itself, but worth mentioning nevertheless.
There are integrations with various external UI toolkits, packaging scripts or
proofs of concept for upcoming features.
