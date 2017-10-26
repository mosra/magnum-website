January 2014 updates to Magnum
##############################

:date: 2014-01-21
:category: Announcements
:tags: C++, CMake, Corrade, OpenGL
:summary: Since the previous October snapshot Magnum gained Visual Studio port,
    font conversion utility, improved text rendering along with example
    application, support for OpenGL debug output, documentation updates, better
    test coverage and many more usability improvements and fixes.

.. role:: cpp(code)
    :language: c++

The January snapshot is available under ``snapshot-2014-01`` tag in
:gh:`Corrade <mosra/corrade^snapshot-2014-01>`,
:gh:`Magnum <mosra/magnum^snapshot-2014-01>`,
:gh:`Magnum Plugins <mosra/magnum-plugins^snapshot-2014-01>` and
:gh:`Magnum Integration <mosra/magnum-integration^snapshot-2014-01>` GitHub
repositories. This article will describe the most important changes, for all
the commits that went to master branches since the
`October snapshot <{filename}october-2013-updates.rst>`_ (tagged
``snapshot-2013-10``) see logs in
:gh:`Corrade <mosra/corrade/compare/snapshot-2013-10...snapshot-2014-01>`,
:gh:`Magnum <mosra/magnum/compare/snapshot-2013-10...snapshot-2014-01>`,
:gh:`Magnum Plugins <mosra/magnum-plugins/compare/snapshot-2013-10...snapshot-2014-01>`
and :gh:`Magnum Integration <mosra/magnum-integration/compare/snapshot-2013-10...snapshot-2014-01>`,
respectively.

For compatibility branch there is tag ``snapshot-2014-01-compatibility`` in
:gh:`Corrade <mosra/corrade^snapshot-2014-01-compatibility>`,
:gh:`Magnum <mosra/magnum^snapshot-2014-01-compatibility>` and
:gh:`Magnum Plugins <mosra/magnum-plugins^snapshot-2014-01-compatibility>`
repositories.

`Google Groups discussion`_
===========================

You can now ask for help, share your work or participiate in development at the
`new Google Groups forum <https://groups.google.com/forum/#!forum/magnum-engine>`_.
The group is open to everyone, you just need Google account to post. In my
opinion this is better than requiring everyone to create new account on some
self-hosted forum.

`New text rendering example`_
=============================

I added text rendering example to showcase new features such as text alignment
and multi-line text, in addition to already existing distance-field rendering
capabilities. See `showcase page <{filename}/pages/showcase.rst>`_ for live
application running on both Emscripten and Native Client. I also added live
Textured Triangle example.

.. container:: m-row

    .. container:: m-col-m-6

        .. image:: {filename}/img/textured-triangle.png
            :alt: Textured Triangle Example

    .. container:: m-col-m-6

        .. image:: {filename}/img/text.png
            :alt: Text Example

`Using absolute includes by default`_
=====================================

All projects were switched to use absolute includes by default, i.e. headers
are included this way:

.. code:: c++

    #include <Magnum/Math/Vector3.h>

rather than this way:

.. code:: c++

    #include <Math/Vector3.h>

This solves many problems with filename collisions (namely with Visual Studio
compiler, Doxygen and possibly elsewhere), see :gh:`mosra/magnum#36` for more
information. As this is a very drastic change, you may need to recreate your
build directories and update at least ``FindCorrade.cmake`` and
``FindMagnum.cmake`` CMake modules with new ones from ``modules/`` subdirectory
in :gh:`Corrade <mosra/corrade/tree/master/modules>` and
:gh:`Magnum <mosra/magnum/tree/master/modules>` repositories (and possibly also
other ones).

If you are building with deprecated features enabled (``BUILD_DEPRECATED``
CMake parameter in *both* Corrade and Magnum), you can still use the old
non-absolute includes, but you are encouraged to switch to absolute ones.

`Visual Studio 2013 support`_
=============================

This release is the first one with experimental Visual Studio 2013 support.
Because the compiler still lacks some required features (:cpp:`constexpr` and
:cpp:`noexcept` keywords) and has some serious bugs in C++11 implementation,
the support is provided only through ``compatibility`` branch. The port
involves some ugly hacks and workarounds which can't be put in ``master``,
hopefully the next MSVC release will make my life easier. Snapshot of
``compatibility`` branch with MSVC 2013 support is available under
``snapshot-2014-01-compatibility`` tag in
:gh:`Corrade <mosra/corrade^snapshot-2014-01-compatibility>` and
:gh:`Magnum <mosra/magnum^snapshot-2014-01-compatibility>` GitHub repositories.
The compatibility branch doesn't contain anything new except workarounds for
said compilers.

`Corrade`_
==========

Dependency changes
------------------

No dependency changes in this release.

New features
------------

-   MSVC 2013 support in compatibility branch
-   Ability to create :dox:`Containers::Array` from initializer list
-   :dox:`Containers::Array::release()` function to release ownership of the
    data, similar to :cpp:`std::unique_ptr::release()`
-   :dox:`Containers::Array[Reference]::data() <Containers::Array::data()>`
    function for easier explicit data access and also disallowing pointer
    conversion of rvalue Array as it would result in access to deleted memory
    (GCC 4.8.1/Clang only).
-   Improved state change signals in :dox:`Interconnect::StateMachine`
-   Support for multiple template arguments in :dox:`CORRADE_COMPARE_AS()`
    macro in :dox:`TestSuite` library.
-   :dox:`Utility::String::join()` and :dox:`Utility::String::joinWithoutEmptyParts()`
    functions.
-   :dox:`Utility::String::fromArray()` convenience functions to create
    :cpp:`std::string` from either character array or :cpp:`nullptr`
-   Ability to retrieve list of all compiled-in resources in given group using
    :dox:`Utility::Resource::list()`
-   Added :cpp:`read()`, :cpp:`readString()`, :cpp:`write()` and :cpp:`writeString()`
    functions to :dox:`Utility::Directory`
-   New :dox:`CORRADE_HAS_TYPE()` type traits macro, which now allows to use
    :dox:`Utility::Debug` to print any container that is iterable with C++11
    range-based for.
-   New :dox:`CORRADE_TARGET_WINDOWS` and :dox:`CORRADE_TARGET_UNIX`
    preprocessor definitions and CMake variables.
-   All classes in documentation are now shown with corresponding :cpp:`#include`
    path

Changes
-------

-   :dox:`TestSuite` tests now display ``?: <unknown>()`` in test output for
    test cases without any checks to make them more discoverable
-   Skipped test cases in :dox:`TestSuite` tests are displayed without useless
    file/line information (unlike failed test cases, where the information is
    needed)
-   All deprecated functions and types are now emitting compiler warnings to
    encourage updating the code
-   Various documentation and example updates and improvements

Bug fixes
---------

-   :dox:`TestSuite::Compare::*File* <TestSuite::Compare::File>` is comparing
    the files in binary mode to avoid platform-dependent EOL issues.
-   :dox:`Utility::Resource` now properly handles empty files when compiling
    resources.
-   :dox:`Utility::Directory` is now working properly under Windows.
-   Fixed compilation issues with Clang and libc++
-   Fixed various compilation issues under MSVC (and worked around a much
    larger amount of compiler bugs)

Deprecated APIs
---------------

-   :cpp:`Utility::String::split(..., bool)` is deprecated, use separate
    functions :dox:`split() <Utility::String::split()>` and
    :dox:`splitWithoutEmptyParts() <Utility::String::splitWithoutEmptyParts()>`
    instead.

Removed APIs
------------

-   Removed :cpp:`Utility::log2()` and :cpp:`Utility::pow2()` utility
    functions, as they weren't used anywhere anymore (and the latter one was a
    bad joke)
-   Removed useless metadata from :dox:`PluginManager::PluginMetadata` class
    (author name, version, description), as they don't fit current use case
    anymore (they were used back in 2007 when Corrade was part of Kompas).
    These metadata can be easily added back as custom configuration values.
-   Removed now useless :cpp:`Utility::Translator` class, it was used only for
    plugin metadata and was prime example of bad design (exposing unguarded
    naked pointers)

Internal changes
----------------

-   Using Jenkins Continuous Integration to maintain compilable and bug-free
    state of the project more easily. Currently GCC 4.8, 4.7, 4.6, 4.5, 4.4,
    Clang 3.3, Clang 3.3 with libc++, Emscripten, x86-32/x86-64 Native Client
    and MinGW32 compilers are tested with both shared and static libraries.
    Separately testing builds with deprecated features enabled and disabled.
-   All files from ``src/`` directory were moved to ``src/Corrade``,
    ``examples/`` directory was moved to ``src/examples`` (required to make
    inter-project includes absolute, as mentioned above)

`Magnum`_
=========

Dependency changes
------------------

No dependency changes in this release.

New features
------------

-   MSVC 2013 support in ``compatibility`` branch
-   Dependency-less plugins from Magnum Plugins repository are now part of
    Magnum to make their usage easier, in particular the :dox:`MagnumFont <Text::MagnumFont>`,
    :dox:`MagnumFontConverter <Text::MagnumFontConverter>`,
    :dox:`TgaImporter <Trade::TgaImporter>`, :dox:`TgaImageConverter <Trade::TgaImageConverter>`
    and :dox:`WavAudioImporter <Audio::WavImporter>` plugins.
-   Documented :dox:`state of OpenGL support <opengl-support>`,
    :dox:`mapping between OpenGL and Magnum API <opengl-mapping>`,
    :dox:`general plugin usage <plugins>` and added
    :dox:`troubleshooting page <troubleshooting>` for most common building and
    rendering issues
-   Support for horizontal and vertical text alignment and multi-line text in
    :dox:`Text::Renderer` (line wrapping is still left up to the user)
-   New :dox:`magnum-fontconverter` utility for converting between font formats
    (e.g. creating distance-field raster font from TTF font). Enable it with
    ``WITH_FONTCONVERTER`` CMake option (currently Linux only), see
    ``magnum-fontconverter --help`` for more information.
-   Support for OpenGL object labels, debug message insertion and retrieval
    (:glext:`KHR_debug`, :glext:`EXT_debug_label` and :glext:`EXT_debug_marker`
    extensions)
-   :dox:`Platform::ScreenedApplication` class allowing the users to easily use
    multiple independent screens in the application.
-   New 1D, 2D and 3D :dox:`Math::Range` class
-   Added :cpp:`red()`, :cpp:`green()`, :cpp:`blue()`, :cpp:`cyan()`,
    :cpp:`magenta()` and :cpp:`yellow()` convenience functions to :dox:`Color3`
    and :dox:`Color4` classes
-   Convenience :dox:`Color3ub` and :dox:`Color4ub` typedefs
-   New :dox:`version()` utility functions for converting :dox:`Version` enum
    value to major/minor version number and back
-   Added :cpp:`release()` function to :dox:`Image` and :dox:`Trade::ImageData`
    for releasing ownership of the data, similar to :cpp:`std::unique_ptr::release()`
-   :dox:`*Image::dataSize() <Image::dataSize()>` function for computing size
    of buffer required to store image of given size
-   Disallowing conversion of rvalue :dox:`Image` and :dox:`Trade::ImageData`
    to ImageReference as it would result in access to deleted memory (GCC
    4.8.1/Clang only)
-   No-op fallback for :dox:`*Texture::setMaxAnisotropy() <Texture::setMaxAnisotropy()>`
    in case :glext:`EXT_texture_filter_anisotropic extension` is not supported
-   Added :cpp:`round()`, :cpp:`floor()` and :cpp:`ceil()` scalar/vector
    functions to :dox:`Math` namespace
-   :dox:`Math::minmax()` and :dox:`Math::Vector2::minmax()` functions
-   Modulo operations for integral :dox:`Math::Vector` classes
-   Ability to request context version in all windowed
    :dox:`Platform::*Application <Platform::Sdl2Application>` classes
-   Mouse wheel support in :dox:`Platform::NaClApplication::MouseEvent`
-   No-op :dox:`Platform::GlutApplication::keyReleaseEvent()` and
    :dox:`Platform::NaClApplication::Configuration::setTitle()` functions to
    preserve source compatibility with other application classes.
-   :dox:`SceneGraph::Drawable::drawables()` function as a non-ambiguous
    alternative to :cpp:`group()`
-   Ability to specify background color color in :dox:`Shaders::Vector`,
    allowing it to use without blending enabled (fully transparent black was
    used previously)
-   New :dox:`Shaders::Generic` class with common definitions, so you can
    configure mesh for the generic shader and render it with any other
    compatible shader
-   Convenience :cpp:`hasNormals()`, :cpp:`hasTextureCoords2D()` functions to
    :dox:`Trade::MeshData2D` and :dox:`Trade::MeshData3D`
-   OpenGL ES 3.0 build now shares list of vendor extensions with OpenGL ES 2.0
    build (i.e. only those extensions that aren't part of ES 3.0 are present in
    :dox:`Magnum::Extensions`)
-   All classes in documentation are now shown with corresponding :cpp:`#include`
    path

Changes
-------

-   The ``magnum-distancefield`` utility is now named :dox:`magnum-distancefieldconverter`
    for consistency with other command-line utilities
-   :dox:`ImageReference` class now stores const data pointer instead of
    mutable one, modifying the data through the class interface isn't valid use
    case anyway
-   Added default template parameter to :dox:`Buffer::data()` and made
    :dox:`*Image::data() <Image::data()>` templated to have them consistent.
    Note that this is source-compatible change.
-   :dox:`Query::result<bool>() <Query::result()>` returns true if value is
    nonzero (was returning true only for value of 1 previously)
-   All functions in :dox:`MeshTools` namespace are now returning either
    :dox:`Containers::Array` or :cpp:`std::unique_ptr` instead of naked
    pointers to avoid accidental out-of-bounds access and memory leaks
-   All functions in :dox:`Text` namespace are now returning :cpp:`std::unique_ptr`
    instead of naked pointers to avoid accidental memory leaks
-   Implementation of :dox:`Platform::*Application::viewportEvent() <Platform::Sdl2Application::viewportEvent()>`
    is not required anymore, because in many cases the application doesn't need
    to react to window resize events at all
-   Textured :dox:`Shaders::Flat` now multiplies texture with the specified
    color instead of ignoring it. See also :gh:`mosra/magnum#34`
-   All deprecated functions and types are now emitting compiler warnings to
    encourage updating the code
-   Various documentation and example updates and improvements

Bug fixes
---------

-   Fixed :dox:`CubeMapTextureArray::image()` function (more precisely it
    wasn't working at all). See also :gh:`mosra/magnum#31`.
-   Fixed wrong assertion in :dox:`Texture::setWrapping()` for rectangle
    textures
-   Fixed wrong assertion in :dox:`AbstractImage::pixelSize()` when computing
    size of separate depth and stencil formats
-   :dox:`Mesh::addVertexBuffer()` now properly computes offsets for matrix
    attributes
-   Taking index buffer offset into account in :dox:`MeshView` class
-   Fixed various issues with textured :dox:`Shaders::Flat` shader (actually
    the textured version was not working at all)
-   Various OS X-related fixes in :dox:`Shaders` library. See also
    :gh:`mosra/magnum#27`.
-   Fixed building issues when both SDL1 and SDL2 is installed on OS X. See
    also :gh:`mosra/magnum#25`.
-   Fixed building issues with CMake-generated XCode project on OS X. See also
    :gh:`mosra/magnum#37`.
-   Proper and failsafe OpenGL 3 context creation on OS X (3.0 is implemented
    only as core context, thus requesting 2.1 won't expose newer features). See
    also :gh:`mosra/magnum#26` and :gh:`mosra/magnum#35`.
-   Fixed compilation issues with Clang and libc++
-   Fixed various compilation issues under MSVC (and worked around an insane
    amount of compiler bugs)

Deprecated APIs
---------------

-   :cpp:`Buffer::Usage` enum is deprecated to reduce header dependencies, use
    global :dox:`BufferUsage` enum instead
-   :cpp:`Mesh::Primitive` enum is deprecated to reduce header dependencies,
    use global :dox:`MeshPrimitive` enum instead
-   :cpp:`Sampler::maxAnisotropy()` is deprecated to avoid naming conflict with
    :dox:`*Texture::setMaxAnisotropy() <Texture::setMaxAnisotropy()>`, use
    :dox:`Sampler::maxMaxAnisotropy()` instead
-   :cpp:`Magnum::Geometry::Rectangle` class (and :cpp:`Rectangle*` typedefs)
    is deprecated, use more generic and feature-rich :dox:`Math::Range`
    instead.
-   :cpp:`Platform::Sdl2Application::Configuration::setFlags()` function and
    related enum is deprecated to avoid naming conflict in the future, use :dox:`Platform::Sdl2Application::Configuration::setWindowFlags()` instead
-   :cpp:`SceneGraph::Animable::group()` is deprecated, use non-ambiguous
    :dox:`SceneGraph::Animable::animables()` instead
-   ``Magnum/Shaders/magnumShadersResourceImport.hpp`` header is deprecated in
    favor of less verbose :dox:`Magnum/Shaders/resourceImport.hpp`
-   :cpp:`Text::TextRenderer` class (and :cpp:`Text::TextRenderer2D`,
    :cpp:`Text::TextRenderer3D` typedefs) is deprecated, use less redundant
    name :dox:`Text::Renderer` instead

Removed APIs
------------

-   Removed :cpp:`SceneGraph::*Transformation::move()` from 2D transformation
    classes. It was originally meant for modifying draw order, but as the draw
    order is managed by :dox:`SceneGraph::DrawableGroup` class and not by
    parent object, this function is essentially useless.

Internal changes
----------------

-   Added thorough tests for all core OpenGL functionality to make refactoring
    easier
-   Various internal optimizations and cleanup to make maintenance easier
-   Using Jenkins Continuous Integration to maintain compilable and bug-free
    state of the project more easily. Currently GCC 4.8, 4.7, 4.6, 4.5, 4.4,
    Clang 3.3, Clang 3.3 with libc++, Emscripten, x86-32/x86-64 Native Client
    and MinGW32 compilers are tested with both shared and static libraries.
    Separately testing builds with deprecated features enabled and disabled.
    Also explicitly testing desktop OpenGL, OpenGL ES 2.0, desktop OpenGL ES
    2.0 emulation, OpenGL ES 3.0 and desktop OpenGL ES 3.0 emulation.
-   All files from ``src/`` directory were moved to ``src/Magnum``,
    ``external/`` directory was moved to ``src/MagnumExternal`` (required to
    make inter-project includes absolute, as mentioned above)

`Magnum Bootstrap`_
===================

-   Added new :gh:`base-sdl2 <mosra/magnum-bootstrap$base-sdl2>` bootstrap
    application so you can start using SDL2 easier
-   Adapted all bootstrap applications to Magnum improvements

`Magnum Plugins`_
=================

Dependency changes
------------------

No dependency changes in this release.

New features
------------

-   All classes in documentation are now shown with corresponding
    :cpp:`#include` path

Changes
-------

-   :dox:`MagnumFont <Text::MagnumFont>`,
    :dox:`MagnumFontConverter <Text::MagnumFontConverter>`,
    :dox:`TgaImporter <Trade::TgaImporter>`,
    :dox:`TgaImageConverter <Trade::TgaImageConverter>` and
    :dox:`WavAudioImporter <Audio::WavImporter>` plugins were moved to core
    Magnum repository
-   Various documentation updates and improvements

Internal changes
----------------

-   Using Jenkins Continuous Integration to maintain compilable and bug-free
    state of the project more easily. Currently GCC 4.8, 4.7, 4.6, 4.5, 4.4,
    Clang 3.3, Clang 3.3 with libc++, Emscripten, x86-32/x86-64 Native Client
    and MinGW32 compilers are tested with both shared and static libraries.
    Separately testing builds with deprecated features enabled and disabled.
    Also explicitly testing desktop OpenGL, OpenGL ES 2.0, desktop OpenGL ES
    2.0 emulation, OpenGL ES 3.0 and desktop OpenGL ES 3.0 emulation.
-   All files from ``src/`` directory were moved to ``src/MagnumPlugins``
    (required to make inter-project includes absolute, as mentioned above)

`Magnum Integration`_
=====================

Dependency changes
------------------

No dependency changes in this release.

New features
------------

-   Initial documentation
-   All classes in documentation are now shown with corresponding
    :cpp:`#include` path

Deprecated APIs
---------------

No API was deprecated in this release.

Removed APIs
------------

No deprecated API was removed in this release.

Internal changes
----------------

-   All files from ``src/`` directory were moved to ``src/Magnum`` (required to
    make inter-project includes absolute, as mentioned above)
