Features
########

Nearly exhaustive feature list of Magnum graphics engine, including notable
features provided by `Corrade utility library <{filename}/corrade.rst>`_.

Architecture
============

-   Intuitive C++11/C++14 API with :dox:`extensive documentation <magnum>`
-   Emphasis on performance and compile-time polymorphism
-   Effort to reduce amount of allocations, memory copying and virtual calls
-   Move semantics, reference passing and automatic memory management instead
    of naked pointers
-   Strong coverage with unit tests
-   Modularity allowing to replace particular features with external
    implementations

Platform support
================

Renderers
---------

-   OpenGL 2.1 through 4.5
-   OpenGL ES 2.0, 3.0 and 3.1
-   WebGL 1.0
-   Additional features provided by OpenGL, OpenGL ES and WebGL extensions

Platforms
---------

-   Linux and embedded Linux
-   Windows
-   OS X
-   `Google Chrome Native Client <https://developers.google.com/native-client/>`_,
    both ``newlib`` and ``glibc`` toolchains
-   HTML5/JavaScript (through :gh:`Emscripten <kripken/emscripten>`)
-   Android

Toolkit integration
-------------------

-   SDL2 on desktop and Emscripten
-   GLUT on desktop
-   Xlib + GLX/EGL on Linux and embedded Linux
-   PPAPI on Native Client
-   Windowless context creation on Linux (using GLX), Windows (using WGL) and
    OSX (using CGL)
-   Support for custom toolkit integration

Supported compilers and build systems
-------------------------------------

-   GCC 4.7+
-   Clang 3.1+
-   GCC 4.4, 4.5, 4.6 and MSVC 2013 supported through compatibility branch
-   CMake 2.8.9+

Math library
============

-   Vector and matrix classes templated on component count and underlying type
-   2D and 3D transformation and projection matrices
-   Complex and dual complex numbers for 2D transformations
-   Quaternions and dual quaternions for 3D transformations
-   Compile-time vector swizzling
-   Strongly-typed angle literals to avoid degree/radian mismatches
-   Vector/scalar overloads of common functions
-   Algorithms for matrix decomposition and orthogonalization
-   Algorithms for computing distances and intersections

OpenGL support
==============

-   RAII wrappers for debug output, buffers, meshes, framebuffers, queries,
    shaders, textures and transform feedback
-   State tracking to avoid needless rebinding
-   Extension detection with automatic fallback to older/slower functionality
    if given feature is not supported
-   Transparent support for :glext:`ARB_direct_state_access` and
    :glext:`EXT_direct_state_access` extensions
-   Fast extension queries in runtime

Engine subsystems
=================

Scene graph
-----------

-   Templated to allow using virtually any transformation implementation, 2D or
    3D
-   Builtin implementation using either plain translation, matrices, dual
    complex numbers or dual quaternions
-   Not imposing any particular rendering technique or draw call ordering
-   Decoupled features from objects, allowing non-linear feature hierarchies
-   Automatic memory management based on object hierarchy

Text rendering
--------------

-   Hardware-accelerated glyph cache
-   Fast text updates using buffer mapping
-   Support for distance-field glyph rendering
-   Full UTF-8 support
-   Plugin interface for font engines and layouters
-   Plugin interface for font converters

Audio support
-------------

-   RAII wrapper for OpenAL buffers and sources
-   Plugin interface for audio importers
-   Scene graph integration

Collision detection
-------------------

-   Both 2D and 3D collision shapes
-   AABBs, OBBs, spheres, capsules, cylinders, points, planes and lines
-   Inverted collision shapes
-   Bounding volume hierarchies
-   Scene graph integration

Resource management
-------------------

-   Reference-counting resource manager with manual or immediate memory
    reclaiming
-   Interface for synchronous and asynchronous resource loaders
-   Resource hot-swapping

Signal/slot library
-------------------

-   Pure-C++ implementation of design pattern popularized by Qt
-   No meta-compiler needed, compile-time argument type checking
-   Any member, non-member or lambda function as a slot
-   Compile-time signal-powered state machine

Containers
----------

-   RAII wrapper around plain C arrays, array view
-   Enum set
-   Intrusive linked list

More
----

-   Dynamic and static plugins with aliases and dependency management
-   INI-style configuration parser and writer
-   Command-line argument parser
-   File system, Unicode, string and data hashing utilities

Ready-to-use functionality
==========================

Builtin shaders
---------------

-   Textured Phong shader
-   Textured flat shader, vertex color shaders
-   Intensity based and distance-field shader for vector graphics
-   Wireframe mesh visualizer

Primitives
----------

-   Square and cube primitives
-   Parametrized circle, sphere and capsule primitives

Tools
=====

Mesh tools
----------

-   Attribute interleaver utility
-   Index array compression utility
-   Cache-aware index optimization algorithm
-   Subdivision and duplicate vertex removal
-   Normal computation and flipping
-   Batch vertex transformation

Texture tools
-------------

-   Atlas packing utility
-   Distance field computation

Debugging functionality
-----------------------

-   Pretty-printer for mathematic structures, collections and enumeration
    values
-   Run-time assertions with pretty-printer
-   Visualizers for collision shapes, object transformations and forces
-   Frame-based profiler

Command-line utilities
======================

-   OpenGL extension and capability viewer
-   Font converter
-   Distance field converter

Data exchange
=============

Import and export
-----------------

-   Direct access to raw imported data for additional conversions
-   Plugin interface for media importers
-   Plugin interface for image converters

Media support
-------------

-   BMP, GIF, HDR, JPEG, PGM, PIC, PNG, PPM, PSD and TGA image import
-   PNG and TGA image export
-   OBJ and PLY mesh import
-   `OpenGEX <https://opengex.org>`_ mesh, texture, scene and material import
-   COLLADA mesh, texture, scene and material import
-   WAV audio import
-   FreeType font engine
-   HarfBuzz font layouter
-   Internal raster font format along with converter plugin

External framework integration
------------------------------

-   Bullet Physics integrated into scene graph with collider visualization
