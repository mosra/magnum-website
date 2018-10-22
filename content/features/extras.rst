Extra Functionality
###################

:breadcrumb:
    {filename}/features.rst Features
:cover: {filename}/img/cover-features-extras.jpg
:summary: Non-core functionality offered by the Magnum engine
:footer:
    .. note-dim::
        :class: m-text-center

        `Features <{filename}/features.rst>`_ | `Plugins & Extensions » <{filename}/features/extensions.rst>`_

.. container:: m-container-inflate

    .. raw:: html
        :file: extras.svg

The following libraries provide additional functionality on top of
`core Magnum APIs <{filename}/features.rst>`_. They complement each other for a
well-balanced experience, however none of them are strictly required and it's
possible to use any subset that matches your particular use case.

`Plugin management`_
====================

Used extensively by various other parts of Magnum, provides a way to extend
functionality at runtime, implement *hot code reloading* or alter behavior of
already deployed applications without needing to modify the binaries
themselves.

-   Plugin manager handling dependencies, inheritance and aliases through the
    same API on all platforms
-   Versioning plugin API and ABI for graceful handling of runtime errors
-   Both dynamically and statically loaded plugins

.. note-dim::

    See the :dox:`plugin-management` example and documentation of the
    :dox:`PluginManager` namespace for details.

`Signals and slots`_
====================

Inspired by Qt's `signals and slots API <https://doc.qt.io/qt-5/signalsandslots.html>`_,
provides an easy-to-use design pattern to connect listeners to events.
Macro-less syntax using C++11 variadic templates, with full type safety and
*no need for any preprocessor*.

-   Lightweight implementation with focus on debug performance and transparency
-   Signals are member functions, slots can be members, free functions or
    lambdas
-   Ability to equip third-party classes with signal/slot functionality using
    multiple inheritance

.. note-dim::

    See the :dox:`interconnect` example and documentation of the
    :dox:`Interconnect` namespace for details.

`Testing and benchmarking`_
===========================

Borrowing initial ideas from Qt's `QTest <https://doc.qt.io/qt-5/qtest-overview.html>`_,
gives the user a clear API for creating automated unit tests, integration tests
and benchmarks with *minimal use of macros* and no opaque magic behind the
scenes.

-   Focus on clarity of test output and diagnostics with colors, pretty
    printers and rich command-line options for test white/blacklisting,
    repeating and shuffling
-   Equivalent workflow on desktop, iOS/Android and web platforms, making
    automated testing a no-brainer
-   Extensible with comparators for special data types, benchmarking of custom
    measured data

.. note-dim::

    See the :dox:`testsuite` example, documentation of the :dox:`TestSuite`
    namespace or the :dox:`DebugTools::CompareImage` class for details.

`Animation framework`_
======================

Layered architecture exposing everything from direct value interpolation over
keyframe structures up to high-level animation-driven behavior and playback
directly from loaded scene data.

-   Defaults to easy-to-use high-level APIs but allows for fine-tuning of all
    parameters including interpolator functions and extrapolation behavior
-   Designed for cache-efficient memory access
-   Ability to use different types for animation input and output (for spline
    interpolation or on-the-fly keyframe data decompression)
-   Ability to use different global time type and animation track time type

.. note-dim::

    See documentation of classes and functions in the :dox:`Animation`
    namespace for details.

`OpenGL wrapping layer`_
========================

Abstracts away platform differences and extensions, resulting in a single API
covering desktop OpenGL, OpenGL ES and WebGL. Depending on the GPU driver, the
backend selects the most suitable feature implementation, giving priority to
recent extensions.

-   OpenGL 2.1 to 4.6, OpenGL ES 2.0, 3.0--3.2, WebGL 1 / 2 + extensions
-   DSA-like API with state tracking, elimination of redundant GL calls,
    feature emulation on older drivers
-   Can co-exist with other renderers sharing the same OpenGL context,
    exchange live OpenGL objects with third-party code

.. note-dim::

    See documentation of the :dox:`opengl-wrapping` and
    :dox:`OpenGL support state <opengl>` for details.

`Vulkan wrapping layer`_
========================

*Coming soon.* Subscribe to :gh:`mosra/magnum#234` for updates.

`Platform integration`_
=======================

Wraps platform-specific or generic windowing toolkits in a thin layer to
provide *consistent access* to OpenGL context and input events across various
systems, utilities for platform capability introspection.

-   Integrates SDL2, GLFW, Xlib, Android native activities and more
-   Abstraction layer to provide OpenGL context to command-line tools in a
    cross-platform way
-   Completely opt-in layer, you can instead hook Magnum directly into
    your own window, context and event handling

.. note-dim::

    See documentation of the :dox:`platform`, :dox:`Platform` namespace and the
    :dox:`magnum-gl-info` utility for details.

`Mesh, texture and shader tools`_
=================================

Collection of essential mesh, texture and image algorithms. Simple input/output
functions working on plain data types for *easy integration* into existing
pipelines.

-   Mesh optimization, indexed mesh handling, vertex transformations, normal,
    subdivision algorithms and attribute (de)interleaving
-   Texture atlas packing
-   Distance field calculation, available also through a command-line utility

*Shader tools are coming soon.* Subscribe to :gh:`mosra/magnum#234` for
updates.

.. note-dim::

    See documentation of the :dox:`MeshTools`, :dox:`TextureTools` namespaces
    and the :dox:`magnum-distancefieldconverter` utility for details.

`Asset management`_
===================

Defines data types and interfaces for import of generic scene formats and
handling of uncompressed and compressed images.

-   Scene hierarchy, mesh data, material, camera, light and texture properties
-   Command-line utility for image conversion, compression and decompression
-   Not required to be used when implementing your own asset loading, provides
    just a ready-to-use API for the general case

.. note-dim::

    See documentation of the :dox:`plugins`, :dox:`Trade` namespace and the
    :dox:`magnum-imageconverter` for details.

`Debugging and prototyping`_
============================

Go-to toolbox for when you are playing around, exploring new techniques,
prototyping a game, debugging a glitch or measuring your app performance.

-   Set of builtin primitives for placeholder objects, material testing or
    debug wireframes
-   Shaders to give your object a basic look or to visualize particular mesh
    properties
-   Profiling, visualization and automatic testing aids

.. note-dim::

    See documentation of the :dox:`shaders`, :dox:`debug-tools`,
    :dox:`Primitives`, :dox:`Shaders`, :dox:`DebugTools` namespaces and the
    :dox:`OpenGLTester` class for details.

`Scene graph functionality`_
============================

Arranges scene data in a tree with automatic object lifetime management,
transformation inheritance and ability to attach any number of features to
scene objects.

-   Choice of different transformation implementations depending on desired
    flexibility and speed/accuracy tradeoff
-   Camera object implementation with convenient handling of projection and
    camera matrices
-   Collection of collision shapes for basic physics or object picking

.. note-dim::

    See documentation of the :dox:`scenegraph`, :dox:`shapes` and
    :dox:`SceneGraph`, :dox:`Shapes` namespaces for details.

`Text and UI`_
==============

GPU-centered text rendering using a pre-populated glyph cache texture with
ability to use arbitrary third-party text layouters for kerning and other
advanced text shaping and layouting support.

-   Rendering from either pixel-perfect or distance field glyph cache
-   Experimental UI library focused on efficient rendering, minimal footprint
    and fast iteration times
-   Label, button, input widgets, modality support

.. note-dim::

    See documentation of the :dox:`Text`, :dox:`Ui` namespaces and the
    :dox:`magnum-ui-gallery` app for details.

`Audio`_
========

Wraps `OpenAL <https://www.openal.org/>`_ in a fashion similar to the
`OpenGL wrapping layer <{filename}/features.rst#opengl-wrapping-layer>`_;
defines data types and interfaces for audio data import.

-   Full support of stock OpenAL 1.1 API with various extensions
-   Player and listener objects for scene graph integration
-   Command-line utility for platform capability introspection

.. note-dim::

    See documentation of the :dox:`Audio` namespace,
    :dox:`OpenAL support state <openal>` and the :dox:`magnum-al-info` utility
    for details.

`Integration libraries`_
========================

Convenience utilities, wrapper types, data conversion and interfaces that
improve interoperability with third-party libraries.

-   Partial or full API wrapping depending on what's needed for seamless
    integration of given library
-   `Bullet Physics <https://bulletphysics.org/>`_ math type conversion, debug
    renderer implementation and scene graph integration
-   `DART Dynamics Animation and Robotics Toolkit <https://dartsim.github.io/>`_
    math type conversion and renderer for DART worlds
-   `Oculus SDK <https://www.oculus.com/>`_ math type conversion and wrapper
    classes for full Oculus VR support in Magnum

.. note-dim::

    See documentation of the :dox:`BulletIntegration`, :dox:`DartIntegration`
    and :dox:`OvrIntegration` namespaces for details.
