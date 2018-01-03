June 2014 updates to Magnum
###########################

:date: 2014-06-29
:archived: True
:category: Announcements
:tags: C++, CMake, Corrade, OpenGL
:summary: The Magnum C++11 and OpenGL/OpenGL ES/WebGL graphics engine gained
    experimental Android support, windowless applications on OS X and Windows,
    uses SDL2 as the default toolkit, adds new texture and mesh features,
    improves build system and got huge documentation review.

The June snapshot is available under ``snapshot-2014-06`` tag in
:gh:`Corrade <mosra/corrade^snapshot-2014-06>`,
:gh:`Magnum <mosra/magnum^snapshot-2014-06>`,
:gh:`Magnum Plugins <mosra/magnum-plugins^snapshot-2014-06>`,
:gh:`Magnum Integration <mosra/magnum-integration^snapshot-2014-06>` and
:gh:`Magnum Examples <mosra/magnum-examples^snapshot-2014-06>` GitHub
repositories. This article will describe the most important changes, for all
the commits that went to ``master`` branches since the
`January snapshot <{filename}/blog/announcements/january-2014-updates.rst>`_
(tagged ``snapshot-2014-01``) see logs in
:gh:`Corrade <mosra/corrade/compare/snapshot-2014-01...snapshot-2014-06>`,
:gh:`Magnum <mosra/magnum/compare/snapshot-2014-01...snapshot-2014-06>`,
:gh:`Magnum Plugins <mosra/magnum-plugins/compare/snapshot-2014-01...snapshot-2014-06>`
and :gh:`Magnum Integration <mosra/magnum-integration/compare/snapshot-2014-01...snapshot-2014-06>`,
respectively.

For compatibility branch there is tag ``snapshot-2014-06-compatibility`` in
:gh:`Corrade <mosra/corrade^snapshot-2014-06-compatibility>`,
:gh:`Magnum <mosra/magnum^snapshot-2014-06-compatibility>`,
:gh:`Magnum Plugins <mosra/magnum-plugins^snapshot-2014-06-compatibility>` and
:gh:`Magnum Examples <mosra/magnum-examples^snapshot-2014-06-compatibility>`
repositories.

`Experimental Android support`_
===============================

This snapshot contains experimental support for Android, see
:dox:`Corrade <building-corrade-cross-android>` and :dox:`Magnum <building-cross-android>`
building documentation. Initial platform support is in :dox:`Platform::AndroidApplication`.

.. image:: {filename}/img/android.png
    :alt: Android Application

`Windowless applications on Mac OS X and Windows`_
==================================================

Windowless applications are now supported on Windows and, thanks to joint
effort by :gh:`amaranth` and :gh:`ArEnSc`, also on OS X. All command-line
utilities such as :dox:`magnum-info`, :dox:`magnum-distancefieldconverter` and
:dox:`magnum-fontconverter` are thus now available on all three major
platforms.

`Dropping GCC 4.6 support from master`_
=======================================

Since GCC 4.7 is available even in LTS Ubuntu, it was time to drop support for
4.6 from ``master`` and providing it only in ``compatibility`` branch. Master
branch now makes use of additonal C++11 features, such as template aliases,
delegating constructors, user-defined literals etc., resulting in more
maintainable code.

`Using SDL2 as the default windowing toolkit`_
==============================================

Since LTS Ubuntu now also has SDL2 in the repositories, it is now used in the
documentation, tutorials and examples as the default windowing toolkit. The
GLUT toolkit, which was used before, is still available in the library, but its
usage is not recommended and it probably won't receive new features anymore.

`Installing debug and release libraries and plugins into separate locations`_
=============================================================================

As explained in :gh:`mosra/magnum#45`, on some platforms it is not possible to
mix debug and release libraries, as it leads to linker errors. Additionally it
also wasn't possible to install both debug and release libraries into the same
location, causing various inconveniences. Since this release, debug libraries
are installed with ``-d`` suffix and debug plugins are installed into separate
``magnum-d/`` directory. From end-user point of view not much has changed,
CMake will automatically select proper libraries based on build type and also
tries the best to select appropriate plugin directory. See
:dox:`documentation about plugins <plugins>` for more information about plugin
directories on multi-configuration build systems.

`Corrade`_
==========

Dependency changes
------------------

-   Minimal required GCC version is now 4.7. Support for GCC 4.6 has been moved
    to :gh:`compatibility <mosra/corrade$compatibility>` branch.

New features
------------

-   Initial Android support, see :dox:`building documentation <building-corrade-cross-android>`
    for more information.
-   :dox:`Utility::AndroidLogStreamBuffer` class for sending messages to
    Android system log
-   :dox:`CORRADE_TARGET_ANDROID` and :dox:`CORRADE_TARGET_APPLE` CMake and
    preprocessor variables

Changes
-------

-   Libraries and plugins in debug build configuration are installed to
    different locations, making it possible to have debug and release libraries
    installed alongside each other
-   Compiler compatibility mode is now automatically detected by CMake to make
    the building easier
-   It's no longer needed to explicitly test validity of pointer returned from
    :dox:`PluginManager::Manager::instance()`
-   Improved error reporting in :dox:`PluginManager` library
-   Thorough documentation review, fixes and improvements

Bug fixes
---------

-   Fixed corner case when :dox:`Utility::Resource::getRaw()` could
    unexpectedly return :cpp:`nullptr`.

Deprecated APIs
---------------

No API was deprecated in this release.

Removed APIs
------------

-   All functionality deprecated in `October snapshot <{filename}october-2013-updates.rst>`_
    has been removed. In particular the deprecated :cpp:`Interconnect::Emitter::connect()`
    was removed, use :dox:`Interconnect::connect()` instead.

`Magnum`_
=========

Dependency changes
------------------

-   Minimal required GCC version is now 4.7. Support for GCC 4.6 has been moved
    to :gh:`compatibility <mosra/magnum$compatibility>` branch.

New features
------------

-   Experimental Android support, see :dox:`building documentation <building-cross-android>`
    for more information
-   Initial implementation of :dox:`Platform::AndroidApplication` class
-   :dox:`Platform::WindowlessWglApplication` and :dox:`Platform::WindowlessCglApplication`,
    which enable :dox:`magnum-info`, :dox:`magnum-distancefieldconverter` and
    :dox:`magnum-fontconverter` utilities on OS X and Windows
-   :dox:`ObjImporter <Trade::ObjImporter>` plugin, mesh import only (no
    materials yet)
-   Completed support of :glext:`EXT_texture_integer`,
    :glext:`ARB_stencil_texturing`, :glext:`ARB_texture_rectangle`,
    :glext:`ARB_texture_buffer_object`, :glext:`ARB_texture_swizzle`,
    :glext:`EXT_texture_sRGB_decode` extensions (and related ES extensions) and
    other missing parameters in :dox:`*Texture <Texture>` classes
-   Basic support for :glext:`ARB_texture_multisample` and
    :glext:`ARB_texture_storage_multisample` in new :dox:`MultisampleTexture`
    class
-   Instanced rendering, base vertex and base instance specification in
    :dox:`Mesh` and :dox:`MeshView` classes
-   :dox:`Mesh::addVertexBufferInstanced()` function for adding buffers with
    per-instance data
-   Mesh multi-draw using :dox:`MeshView::draw()`
-   Initial :glext:`ARB_multi_bind` support with :dox:`AbstractTexture::bind()`
-   Support for :glext:`ARB_vertex_type_10f_11f_11f_rev` in
    :dox:`AbstractShaderProgram::Attribute::DataType`
-   New variants of :dox:`Shader::compile()` and :dox:`AbstractShaderProgram::link()`,
    allowing the driver to perform paralell compilation in multiple threads.
-   Added :dox:`*Texture::maxSize() <Texture::maxSize()>` queries
-   :dox:`MeshTools::compile()` for automagic creation of 2D and 3D meshes from
    imported data
-   :dox:`MeshTools::interleaveInto()` for interleaving data into existing
    buffer
-   :dox:`Shapes::Shape::collision()` function as complement to
    :dox:`Shapes::Shape::collides()`
-   :dox:`Platform::GlutApplication::KeyEvent` finally has more complete key
    support
-   :dox:`Math::Range::data()` function, useful when querying range values from
    GL
-   Support for specifying context flags using
    :dox:`Platform::*Application::Configuration::setFlags() <Platform::Sdl2Application::Configuration::setFlags()>`,
    they are now also shown in :dox:`magnum-info`
-   More robust support for driver detection and driver bug workarounds. Driver
    can be detected using :dox:`Context::detectedDriver()`, problematic
    extension disabled, which is then shown in :dox:`magnum-info` and can be
    also queried using :dox:`Context::isExtensionDisabled()`
-   :dox:`Context::extensionStrings()` for getting list of all extension
    strings exposed by the driver
-   :dox:`Context::resetState()` for resetting internal state tracker, allowing
    to use Magnum along with third-party OpenGL code
-   :dox:`MAGNUM_TARGET_WEBGL` CMake and preprocessor variable

Changes
-------

-   Libraries and plugins in debug build configuration are installed to
    different locations, making it possible to have debug and release libraries
    installed alongside each other.
-   :dox:`Platform::Sdl2Application` is now used everywhere (documentation,
    tutorials, examples) by default, replacing :dox:`Platform::GlutApplication`.
    The GLUT application is still available, though.
-   The need for compiler compatibility mode is now automatically detected by
    CMake to avoid strange compilation errors later
-   Split out :dox:`Texture` functionality into dedicated :dox:`TextureArray`,
    :dox:`MultisampleTexture` and :dox:`RectangleTexture` classes to make their
    usage less error-prone
-   :dox:`Buffer::invalidataData()`, :dox:`Buffer::invalidateSubData()` and
    :dox:`Renderer::resetNotificationStrategy()` functions are enabled on
    OpenGL ES as a no-op
-   Added :cpp:`std::vector` overload of :dox:`MeshTools::combineIndexArrays()`
    for greater runtime-usage flexibility
-   :dox:`Platform::Sdl2Application` now defaults to non-resizable window, you
    can change the behavior using :dox:`Platform::Sdl2Application::Configuration::setWindowFlags()`
-   It's now possible to import objects without materials
-   More robust OpenGL version checks when creating context
-   Thorough documentation review, fixes and improvements

Bug fixes
---------

-   Fixed usage with OpenGL ES 3.1 contexts (it complained about invalid
    version)
-   Fixed compilation of Shaders::MeshVisualizer under WebGL and with ANGLE,
    see :gh:`mosra/magnum#56`
-   Fixed various build issues on Mac OS X, see :gh:`mosra/magnum#51` and
    :gh:`mosra/magnum#54`

Deprecated APIs
---------------

-   :cpp:`Mesh::setVertexCount()` and :cpp:`Mesh::setIndexCount()` had
    nonintuitive behavior in some corner cases and are deprecated, use general
    :dox:`Mesh::setCount()` function instead
-   :cpp:`MeshView::setVertexRange()` and four-/two-argument
    :cpp:`MeshView::setIndexRange()` are deprecated for similar reason as
    above, use :dox:`MeshView::setCount()`, :dox:`MeshView::setBaseVertex()`
    and three-/one-argument :dox:`MeshView::setIndexRange()` instead
-   :cpp:`Texture::Target` enum is deprecated as the API was too error-prone,
    use dedicated :dox:`TextureArray`, :dox:`MultisampleTexture` and
    :dox:`RectangleTexture` classes instead
-   :cpp:`Framebuffer::attachTexture*D()` is deprecated, use more generic
    :dox:`Framebuffer::attachTexture()` and
    :dox:`Framebuffer::attachTextureLayer()` instead
-   :cpp:`FramebufferBlit::*Buffer`` enum values are deprecated, use shorter
    :dox:`FramebufferBlit::Color` and similar instead
-   :dox:`Audio::Source::play()`, :dox:`SceneGraph::AbstractObject::setClean()`,
    :dox:`SceneGraph::AbstractObject::transformationMatrices()` and similar
    taking :cpp:`std::initializer_list` of pointers are deprecated, use
    versions taking list of references instead
-   :dox:`MeshTools::compressIndices()` and :dox:`MeshTools::interleave()`
    which filled :dox:`Mesh` and :dox:`Buffer` directly are deprecated as they
    had undesired side-effects in some cases, use the data-returning versions
    instead and then configure mesh and buffer manually
-   :dox:`MeshTools::combineIndexedArrays()` taking :cpp:`std::tuple` is
    deprecated, use version taking :cpp:`std::pair` instead
-   :dox:`MeshTools::removeDuplicates()` taking also list of indices is
    deprecated, use the function in conjunction with :dox:`MeshTools::duplicate()`.
    See function documentation for more information.
-   Parameter-less :dox:`Mesh::draw()` and :dox:`MeshView::draw()` are
    deprecated, use versions with explicit shader parameter instead
-   Deprecated implicit conversion of :dox:`Resource<T, U> <Resource>` to
    :cpp:`U&`, as it is not safe. Use explicit dereference operator instead.
-   Texture binding using :dox:`*Texture::bind() <Texture::bind()>` is
    deprecated, use setup functions of particular shaders instead (e.g.
    :dox:`Shaders::Phong::setDiffuseTexture()`)
-   :cpp:`Context::Flag::Robustness` is deprecated, use
    :cpp:`Context::Flag::RobustAccess` which reflects OpenGL naming better
    instead
-   :cpp:`Texture::maxLayers()` has misleading naming and is deprecated, use
    :dox:`Shader::maxCombinedTextureImageUnits()` instead

Removed APIs
------------

-   All functionality deprecated in `October snapshot`_ has been removed,
    namely:

    -   Removed deprecated raw-pointer versions of :dox:`Buffer::setData()` and
        :dox:`Buffer:setSubData()`, use overloads taking
        :dox:`Containers::ArrayReference` instead
    -   Removed deprecated ``Magnum/ImageFormat.h`` header and
        :cpp:`ImageFormat`/:cpp:`ImageType` enums, use :dox:`Magnum/ColorFormat.h`
        header and :dox:`ColorFormat`/:dox:`ColorType` enums instead
    -   Removed deprecated :cpp:`Matrix2` and :cpp:`Matrix2d` typedefs, use
        :cpp:`Matrix2x2` and :cpp:`Matrix2x2d` instead
    -   Removed deprecated ``Magnum/Swizzle.h`` header and :cpp:`Magnum::swizzle()`
        function, use :dox:`Magnum/Math/Swizzle.h` and :dox:`Math::swizzle()`
        instead
    -   Removed deprecated :cpp:`*::maxSupported*()` limit queries, use the
        less verbose :cpp:`*::max*()` versions instead
    -   Removed deprecated :cpp:`Platform::*Application::InputEvent::Modifier::*Button`
        enum values, use :dox:`Platform::*Application::*Event::buttons() <Platform::Sdl2Application::MouseMoveEvent::buttons()>`
        function and :dox:`Platform::*Application::*Event::Button <Platform::Sdl2Application::MouseMoveEvent::Button>`
        enum instead
    -   :cpp:`Platform::Sdl2Application::Configuration::Flag` enum and related
        functions were replaced with WindowFlag, as the name would now conflict
        with context flags

Internal changes
----------------

-   Removed last remaining bits of global state, everything is now stored
    per-context.

`Magnum Bootstrap`_
===================

Changes
-------

-   All windowed applications now use SDL2 by default. Minimal application
    using GLUT is available in new :gh:`base-glut <mosra/magnum-bootstrap$base-glut>`
    branch.
-   Adapted all bootstrap applications to Magnum improvements

`Magnum Plugins`_
=================

Dependency changes
------------------

-   Minimal required GCC version is now 4.7. Support for GCC 4.6 has been moved
    to :gh:`compatibility <mosra/magnum-plugins$compatibility>` branch.

Changes
-------

-   Libraries and plugins in debug build configuration are installed to
    different locations, making it possible to have debug and release libraries
    installed alongside each other.

`Magnum Integration`_
=====================

Dependency changes
------------------

-   Minimal required GCC version is now 4.7.

Changes
-------

-   Libraries and plugins in debug build configuration are installed to
    different locations, making it possible to have debug and release libraries
    installed alongside each other.

Deprecated APIs
---------------

No API was deprecated in this release.

Removed APIs
------------

No deprecated API was removed in this release.
