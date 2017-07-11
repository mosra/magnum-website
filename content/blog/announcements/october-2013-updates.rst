October 2013 updates to Magnum
##############################

:date: 2013-10-30 18:50
:category: Announcements
:tags: C++, CMake, Corrade, OpenGL
:summary: Summary of changes during the first two months after public release.
    Two new ports, usability improvements, OpenGL limit queries and
    introduction of API deprecation.

The January snapshot is available under ``snapshot-2013-10`` tag in
:gh:`Corrade <mosra/corrade^snapshot-2013-10>`,
:gh:`Magnum <mosra/magnum^snapshot-2013-10>`,
:gh:`Magnum Plugins <mosra/magnum-plugins^snapshot-2013-10>` and
:gh:`Magnum Integration <mosra/magnum-integration^snapshot-2013-10>` GitHub
repositories. This article will describe the most important changes, for all
the commits that went to master branches since the
`first public release in August <{filename}introducing-magnum.rst>`_ (tagged
``snapshot-2013-08``) see logs in
:gh:`Corrade <mosra/corrade/compare/snapshot-2013-08...snapshot-2013-10>`,
:gh:`Magnum <mosra/magnum/compare/snapshot-2013-08...snapshot-2013-10>`,
:gh:`Magnum Plugins <mosra/magnum-plugins/compare/snapshot-2013-08...snapshot-2013-10>`
and :gh:`Magnum Integration <mosra/magnum-integration/compare/snapshot-2013-08...snapshot-2013-10>`,
respectively.

`API deprecation and backwards compatibility`_
==============================================

Because the library is constantly evolving, some APIs will be replaced with
faster, safer or more intuitive versions from time to time. The old API is then
marked as deprecated and scheduled for removal in some future release. The
libraries are source compatible, but not binary compatible, as full binary
compatibility might impose unwanted performance issues, which is usually not
wanted for high-performance graphics frameworks.

Both Corrade and Magnum libraries are by default built with deprecated APIs
included. However, to make your code future-proof and more robust, you should
disable building of deprecated APIs by disabling ``BUILD_DEPRECATED`` CMake
option. Information about whether given library is built with deprecated APIs
or not is exposed via :dox:`CORRADE_BUILD_DEPRECATED` or
:dox:`MAGNUM_BUILD_DEPRECATED` CMake variable and also preprocessor definition.
Please note that the deprecated APIs will be periodically removed after some
time (on which I have yet to decide, but the deprecation period will be not
shorter than six months).

The documentation now also contains list of deprecated APIs in both
:dox:`Corrade <deprecated>` and :dox:`Magnum <deprecated>`, information about
whether :dox:`CORRADE_BUILD_DEPRECATED` and :dox:`MAGNUM_BUILD_DEPRECATED` is
enabled or not is now also printed in :dox:`magnum-info`.

`Corrade`_
==========

Dependency changes
------------------

No dependency changes in this release.

New features
------------

-   Mac OS X port (thanks to David Lin)
-   Fixed and updated Emscripten (HTML5) port
-   :dox:`Interconnect` library can now connect signals to non-member functions
    and C++11 lambdas. See :dox:`Interconnect::connect()` for more information.
-   Added simple :dox:`StateMachine <Interconnect::StateMachine>`
    implementation into :dox:`Interconnect` library.

Changes
-------

-   Returning :cpp:`std::unique_ptr` instead of naked pointer from
    :dox:`PluginManager::Manager::instance()` for better usability and to avoid
    accidental memory leaks
-   Documentation updates and improvements to make the library more accessible
    for newcomers
-   Various build system updates for better compile times and easier
    installation

Bug fixes
---------

-   :dox:`CORRADE_VERIFY()` macro in TestSuite can now be conveniently used
    also on classes with explicit :cpp:`operator bool()`
-   Fixed assertion failure on ``-long-arguments`` parsing in
    :dox:`Utility::Arguments`

Deprecated APIs
---------------

-   :dox:`Interconnect::Emitter::connect()` is deprecated, use
    :dox:`Interconnect::connect()` instead.

Removed APIs
------------

No deprecated API was removed in this release.

`Magnum`_
=========

Dependency changes
------------------

-   Removed GLEW dependency in favor of built-in ``glLoadGen``, resulting in
    greatly reduced compilation time and less compatibility issues. The base
    library thus now depends only on Corrade and OpenGL libraries.

New features
------------

-   Initial Emscripten (HTML5/WebGL) port. See :dox:`Platform::Sdl2Application`
    for detailed porting information. The `showcase page <{filename}/pages/showcase.rst>`_
    has now online Emscripten demos, see also
    :gh:`base-emscripten <mosra/magnum-bootstrap$base-emscripten>` branch in
    Magnum Bootstrap.
-   Mac OS X port (thanks to Miguel Martin)
-   :dox:`Math::Vector::isZero()` and :dox:`Math::Vector2::aspectRatio()`
    convenience functions
-   Bitwise AND, OR, XOR, bit inversion and bit shift in integral
    :dox:`Math::Vector` classes
-   :dox:`Math::MatrixMxN<T> <Math::Matrix4x3>` and :dox:`Math::MatrixNxN<T> <Math::Matrix3x3>`
    template aliases for non-square and non-transforming square matrices in
    :dox:`Math` namespace, :dox:`MatrixNxN <Matrix3x2>` and
    :dox:`MatrixNxNd <Matrix2x2d>` typedefs for float and double
    non-transforming square matrices in root Magnum namespace. Note that
    template aliases are not present in GCC 4.6 builds.
-   Limit queries for all OpenGL objects, the information is now printed also
    by :dox:`magnum-info` utility.
-   :dox:`Context::isExtensionSupported()` is now able to query extension
    availability on particular GLSL version
-   Using :cpp:`std::unique_ptr` and C++1y :cpp:`std::optional` for resources
    returned from :dox:`Trade::AbstractImporter` for better usability and to
    avoid accidental memory leaks
-   New translation-only transformation in :dox:`SceneGraph` supporting also
    purely integral coordinates, useful e.g. for UI or 2D platformers.
-   Detailed collision queries and new :dox:`InvertedSphere <Shapes::InvertedSphere>`
    shape in Shapes library
-   Texture support in :dox:`Shaders::Flat`
-   Mouse button queries in :dox:`Platform::*Application::MouseMoveEvent <Platform::Sdl2Application::MouseMoveEvent>`

Changes
-------

-   :dox:`Buffer::setData()` and :dox:`Buffer::setSubData()` are now accepting
    :dox:`Containers::ArrayReference` for more convenient usage
-   :dox:`Platform::*Application::viewportEvent() <Platform::Sdl2Application::viewportEvent()>`
    is not called at all if window size doesn't change, allowing for less
    cumbersome ofscreen rendering setups
-   Removed redundant operators from :dox:`Math::*Matrix <Math::RectangularMatrix>`,
    use vectors for component-wise multiplication and division instead
-   Documentation updates and improvements to make the library more accessible
    for newcomers
-   Various build system updates for better compile times and easier
    installation

Bug fixes
---------

-   Fixed compilation with Clang's libc++
-   Fixed compatibility of builtin shaders with GLSL 1.20 and GLSL ES 1.00,
    added shader compilation tests to prevent this issue from reappearing

Deprecated APIs
---------------

-   :cpp:`Matrix2` and :cpp:`Matrix2d` is deprecated, use :dox:`Matrix2x2` and
    :dox:`Matrix2x2d` instead
-   :cpp:`swizzle()` is deprecated, all of its functionality has been moved
    into :dox:`Math::swizzle()`
-   :cpp:`Buffer::setData()` and :cpp:`Buffer::setSubData()` overloads taking
    pair of data pointer and size are deprecated, use
    :dox:`Containers::ArrayReference`-based :dox:`Buffer::setData()` and
    :dox:`Buffer::setSubData()`
-   ``ImageFormat.h`` header, :cpp:`ImageFormat` and :cpp:`ImageType` enums are
    deprecated, use :dox:`ColorFormat.h` header, :dox:`ColorFormat` and
    :dox:`ColorType` enums which reflect OpenGL naming better instead
-   :cpp:`Platform::*Application::InputEvent::Modifier::*Button` enum values
    are deprecated, use
    :cpp:`Platform::*Application::*Event::buttons() <Platform::Sdl2Application::MouseMoveEvent::buttons()>`
    and :cpp:`Platform::*Application::*Event::Button <Platform::Sdl2Application::MouseMoveEvent::Button>`
    enum instead
-   :cpp:`AbstractShaderProgram::maxSupportedVertexAttributeCount()`,
    :cpp:`AbstractTexture::maxSupportedLayerCount()` and
    :cpp:`Sampler::maxSupportedAnisotropy()` are deprecated in favor of less
    verbose :cpp:`AbstractShaderProgram::maxVertexAttributes()`,
    :cpp:`AbstractTexture::maxLayers()` and :cpp:`Sampler::maxAnisotropy()`

Removed APIs
------------

No deprecated API was removed in this release.

`Magnum Plugins`_
=================

Dependency changes
------------------

No dependency changes in this release.

New features
------------

-   Emscripten port of dependency-less :dox:`TgaImporter <Trade::TgaImporter>`,
    :dox:`TgaImageConverter <Trade::TgaImageConverter>` and
    :dox:`MagnumFont <Text::MagnumFont>` plugins

Changes
-------

-   Reworked :dox:`ColladaImporter <Trade::ColladaImporter>` internals to avoid
    double deletions
-   Documentation updates and improvements to make the library more accessible
    for newcomers
-   Various build system updates for better compile times and easier
    installation

Bug fixes
---------

-   Fixed compilation of plugins using :cpp:`std::u32string` on MinGW
-   Fixed linker errors on MinGW

`Magnum Integration`_
=====================

Dependency changes
------------------

No dependency changes in this release.

Changes
-------

-   Documentation updates and improvements to make the library more accessible
    for newcomers
-   Various build system updates for better compile times and easier
    installation

Deprecated APIs
---------------

No API was deprecated in this release.

Removed APIs
------------

No deprecated API was removed in this release.
