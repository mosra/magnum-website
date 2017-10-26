Magnum --- C++11 and OpenGL graphics engine
###########################################

:save_as: index.html
:url:

Magnum is 2D/3D graphics engine written in C++11/C++14 and modern OpenGL. Its
goal is to simplify low-level graphics development and interaction with OpenGL
using recent C++11 features and to abstract away platform-specific issues.

.. note-danger:: Website under development

    This website is very much a work-in-progress, currently contains only
    archived content and might be *very broken* on your system. Please see
    `the explanatory blog post <{filename}/blog/meta/a-new-website-coming-soon.rst>`_
    for more information.

.. todo: button Magnum Download latest

Design goals
============

-   **2D is not an ugly stepchild.** Many engines out there were created as
    pure 2D or 3D and the alternative is usually just an afterthought, if it is
    present at all. If you want to do your next project in 2D only, you have to
    either relearn another engine from scratch or emulate it in 3D, leaving
    many things overly complicated. Magnum treats 2D equivalently to 3D so you
    can reuse what you already learned for 3D and even combine 2D and 3D in one
    project.
-   **Forward compatibility.** If newer technology makes things faster, simpler
    or more intuitive, it is the way to go. Magnum by default relies on decent
    C++11 support and modern OpenGL features and if some feature isn't
    available, it tries to emulate it using older functionality. However, you
    are not restricted to use the older functionality directly, if you really
    want to.
-   **Intuitive, but not restrictive API.** Scripting languages are often
    preferred to C/C++ because they are designed to have less complicated APIs
    and less boilerplate code. Magnum is designed with intuitivity in mind, but
    also with speed and static checks that strongly-typed native code offers.
    It wraps OpenGL into less verbose and more type-safe API, which is easier
    to use. Usually the most common way is the most simple, but if you need
    full control, you can have it.
-   **Extensible and replaceable components.** If you want to use different
    mathematical library for specific purposes, that new windowing toolkit,
    your own file formats or another physics library, you can. Conversion of
    math structures between different libraries can be done on top of pre-made
    skeleton classes, support for file formats is done using plugins and
    platform support is done by writing simple wrapper class.

Supported platforms
===================

Graphics APIs:

-   **OpenGL** 2.1 through 4.5, core profile functionality and modern
    extensions
-   **OpenGL ES** 2.0, 3.0, 3.1 and extensions to match desktop OpenGL
    functionality
-   **WebGL** 1.0 and extensions to match desktop OpenGL functionality

Platforms:

-   **Linux** and embedded Linux (natively using GLX/EGL and Xlib or through
    GLUT or SDL2 toolkit)
-   **Windows** (through GLUT or SDL2 toolkit)
-   **OS X** (through SDL2 toolkit, thanks to :gh:`Miguel Martin <miguelishawt>`
    and others)
-   **Google Chrome** (through `Native Client <https://developers.google.com/native-client/>`_,
    both ``newlib`` and ``glibc`` toolchains are supported)
-   **HTML5/JavaScript** (through `Emscripten <https://github.com/kripken/emscripten/wiki>`_)
-   **Android**

Features
========

-   Vector and matrix library with implementation of complex numbers,
    quaternions and their dual counterparts for representing transformations.
-   Classes wrapping OpenGL using RAII principle and simplifying its usage with
    direct state access and automatic fallback for unavailable features.
-   Extensible scene graph which can be modified for each specific usage.
-   Plugin-based data exchange framework, tools for manipulating meshes,
    textures and images.
-   Pre-made shaders, primitives and other tools for easy prototyping and
    debugging.

See `complete feature list <{filename}/pages/features.rst>`_ for more
information. You might want to read also
`official project announcement <{filename}/blog/announcements/introducing-magnum.rst>`_
or notes from latest `May 2015 release <{filename}/blog/announcements/may-2015-updates.rst>`_.

Related projects
================

The engine itself is kept as small as possible with only little dependencies.
Additional functionality, often depending on external libraries, is provided in
separate repositories --- integration with various external math and physics
libraries, various importer plugins for image, audio and 3D model formats and
also examples of engine usage, varying from simple Hello World-like example to
more advanced applications, such as viewer for complex 3D models.

There is also repository with bootstrap projects for many use cases, helping
you get up and running in no time.

Showcase
========

Magnum is currently used in two small games (one of which is still unannounced)
and the functionality is demonstrated in various examples. See
`showcase page <{filename}/pages/showcase.rst>`_ for images and live
applications.

Getting started
===============

Documentation is available :dox:`for online viewing <magnum>`, you can also
generate it directly from the source code, see above download instructions for
more information. The documentation has a thorough
:dox:`guide how to start using Magnum in your project <getting-started>`.

License
=======

Magnum is licensed under MIT/Expat license:

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017 Vladimír Vondruš
    <mosra@centrum.cz>

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
