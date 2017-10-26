May 2015 updates to Magnum
##########################

:date: 2015-05-09
:category: Announcements
:tags: C++, CMake, Corrade, OpenGL, NaCl, WebGL
:cover: {filename}/img/viewer.png
:summary: The Magnum C++11/C++14 and OpenGL graphics engine gained OpenGEX
    format support, a bunch of new importer plugins, cross-platform extension
    loader, transform feedback support and new features from OpenGL ES 3.1 and
    OpenGL 4.5, among other things.

.. role:: cpp(code)
    :language: c++
    :class: highlight

The new release comes almost a whole year after the previous one, mainly due to
the fact that I'm currently full-time employed and I have less free time than I
expected. Nevertheless the project accumulated quite a lot of changes over the
year, so this changelog may be a bit overwhelming. I plan to do one smaller
release in a few months to polish and clean up some more things and then I'll
probably start breaking stuff in never-before-seen ways to make
:gh:`Vulkan support <mosra/magnum#91>` a reality.

The May 2015 snapshot is available under ``snapshot-2015-05`` tag in
:gh:`Corrade <mosra/corrade^snapshot-2015-05>`, :gh:`Magnum <mosra/magnum^snapshot-2015-05>`,
:gh:`Magnum Plugins <mosra/magnum-plugins^snapshot-2015-05>`,
:gh:`Magnum Integration <mosra/magnum-integration^snapshot-2015-05>` and
:gh:`Magnum Examples <mosra/magnum-examples^snapshot-2015-05>` GitHub
repositories. This article will describe the most important changes, for
all the commits that went to master branches since the
`June 2014 <{filename}/blog/announcements/june-2014-updates.rst>`_
snapshot (tagged ``snapshot-2014-06``) see logs in
:gh:`Corrade <mosra/corrade/compare/snapshot-2014-06...snapshot-2015-05>`,
:gh:`Magnum <mosra/magnum/compare/snapshot-2014-06...snapshot-2015-05>`,
:gh:`Magnum Plugins <mosra/magnum-plugins/compare/snapshot-2014-06...snapshot-2015-05>`
and :gh:`Magnum Integration <mosra/magnum-integration/compare/snapshot-2014-06...snapshot-2015-05>`,
respectively.

For compatibility branch there is tag ``snapshot-2015-05-compatibility`` in
:gh:`Corrade <mosra/corrade^snapshot-2015-05-compatibility>`,
:gh:`Magnum <mosra/magnum^snapshot-2015-05-compatibility>`,
:gh:`Magnum Plugins <mosra/magnum-plugins^snapshot-2015-05-compatibility>` and
:gh:`Magnum Examples <mosra/magnum-plugins^snapshot-2015-05-compatibility>`
repositories.

`Plugin aliases, smart importers and new format support`_
=========================================================

In an ongoing attempt to make the engine useful even without a slew of external
library dependencies, I added a bunch of dependency-less plugins for loading
common scene and image formats (see the changelog below for a complete list). I
had a firsthand experience about how crazy hard it is to get working Freetype,
HarfBuzz or even libPNG binary for Windows, so there are more plugins planned
(especially for dependency-less font handling), stay tuned.

In addition to this, the plugin loader now supports aliases. It means that, to
load PNG images, for example, you no longer need to worry about which
particular plugin might be available on given platform and just always request
PngImporter plugin:

.. code:: c++

    PluginManager::Manager<Trade::AbstractImporter> manager{MAGNUM_PLUGINS_IMPORTER_DIR};
    std::unique_ptr<Trade::AbstractImporter> importer = manager.loadAndInstantiate("PngImporter");
    // ...

Now, if the ``PngImporter`` plugin is not available, it tries to load any other
plugin that aliases with ``PngImporter``. Currently it might be for example
:dox:`StbImageImporter <Trade::StbImageImporter>`, in future releases possibly
also ``Sdl2ImageImporter`` or ``WebGlImageImporter``.

Lastly, there are now *smart* image and scene loaders, named
:dox:`AnyImageImporter <Trade::AnyImageImporter>` and
:dox:`AnySceneImporter <Trade::AnySceneImporter>`.
They try to detect file type from filename and then redirect the operation to
dedicated importer plugin (so if you load ``image.tga``, for example, it tries
to load it with ``TgaImporter``, if you load ``texture.png``, it tries to load
it with ``PngImporter`` etc.). In combination with plugin aliases this makes a
fairly powerful importer framework.

`Initial OpenDDL/OpenGEX support`_
==================================

Possibly the biggest new importer plugin is the `OpenGEX <http://www.opengex.org/>`_
format importer. If you haven't heard about it yet, it is a fairly new 3D scene
file format based on the cleanly designed OpenDDL language. It aims to be a
better alternative to the bloated XML-based COLLADA format and currently there
are exporters from 3Ds Max, Maya and Blender.

Current OpenGEX importer implementation in Magnum covers the same feature set
as the `ColladaImporter <http://mosra.cz/blog/magnum-doc/classMagnum_1_1Trade_1_1ColladaImporter.html>`_
plugin, but with faster and more robust implementation and hopefully with less
bugs and uncovered corner cases. The future of the COLLADA importer is
uncertain and it will probably be deprecated in favor of the new OpenGEX
importer, because COLLADA support in 3D modelling software is far from ideal
and the XML parser is hell to implement.

The underlying `OpenDDL <http://mosra.cz/blog/magnum-doc/classMagnum_1_1OpenDdl_1_1Document.html#details>`_ parser that powers the
plugin is currently only a private part of the plugin, but if it is seen to be
useful outside of the plugin, I may move it into a publicly usable library.
Here's an example how OpenDDL file parsing can look like:

.. code:: c++

    OpenDdl::Document d;

    /* Parse the document */
    if(!d.parse(data, OpenGex::structures, OpenGex::properties)) {
        // ...
    }

    /* Validate its structure */
    if(!d.validate(OpenGex::rootStructures, OpenGex::structureInfo)) {
        // ...
    }

    /* Parse all meshes */
    for(OpenDdl::Structure geometryObject: d.childrenOf(OpenGex::GeometryObject)) {
        /* Decide about primitive */
        if(std::optional<OpenDdl::Property> primitive = geometryObject.findPropertyOf(OpenGex::primitive)) {
            auto&& str = primitive->as<std::string>();
            if(str == "triangles") {
                // ...
            } else if(str == "lines") {
                // ...
            } // ...
        } else {
            // default primitive ...
        }

        /* Parse vertex array */
        OpenDdl::Structure vertexArray = geometryObject.firstChildOf(OpenGex::VertexArray);
        auto&& attrib = vertexArray.propertyOf(OpenGex::attrib).as<std::string>();
        if(attrib == "position") {
            // ...
        } else if(attrib == "normal") {
            // ...
        }

        /* Parse vertex array data */
        Containers::ArrayReference<const Float> vertexData = vertexArray.firstChild().asArray<Float>();
        // ...
    }

The `Viewer example <http://mosra.cz/blog/magnum-doc/examples-viewer.html>`_
was also updated to support loading OpenGEX files and it is now also available
`online through Emscripten <http://mosra.cz/blog/magnum/viewer-emscripten.html>`_.

.. image:: {filename}/img/viewer.png
    :alt: Viewer example

`New extension loader, extension loading for OpenGL ES`_
========================================================

I was being more and more dissatisfied with the state of ``glLoadGen`` (which
was originally meant to replace the even worse GLEW) and switched to
:gh:`flextGL <ginkgo/flextgl>`. If you don't know it yet, it is very flexible
and customizable extension loader generator and consists of just a single very
clean Python file. Switching to it resulted in much shorter generated code
(which means faster build times) and after a few minor changes I managed to add
OpenGL ES support to it. So it is now finally possible to use extensions on
both desktop GL and OpenGL ES. The only platforms which don't have any notion
of traditional extension loading are WebGL and NaCl, on which the extension
handling is not yet fully implemented.

`OpenGL 4.5, OpenGL ES 3.1 support`_
====================================

In August 2014, OpenGL 4.5 was released, which was understood as some sort of a
cleanup release with no radically new functionality. The most essential
addition is :glext:`ARB_direct_state_access`, which finally cleans up some
rough corners of the old :glext:`EXT_direct_state_access` extension and puts it
in the core specification. Magnum currently implements both extensions and
prefers the ARB one. Some API changes were needed to accomodate to the new
design, mainly in construction of query objects and cube map texture data
uploads. The other minor features include ability to query only a part of
texture image, inverted conditional rendering etc., see below for complete
list.

Thanks to the new extension loader it was also finally possible to implement
OpenGL ES 3.1 support. Unlike the ES 2.0/3.0 switch, there is no compile-time
option to enable OpenGL ES 3.1, the library is compiled for both 3.0 and 3.1
and the new features are enabled based on version reported by the driver.

`IRC channel`_
==============

In addition to all other communication channels there is now a IRC channel:
join the discussion at ``#magnum-engine`` on Freenode.

`Corrade`_
==========

New features
------------

-   Support for plugin aliases in :dox:`PluginManager` library.
-   Range-based-for support in :dox:`Containers::LinkedList`.
-   Added convenience :dox:`PluginManager::Manager::loadAndInstantiate()`
    function.
-   Added :dox:`Containers::*Array::slice() <Containers::Array::slice()>` and
    friends.
-   Added :dox:`Utility::String::beginsWith()` and :dox:`Utility::String::endsWith()`.

Changes
-------

-   :dox:`TestSuite::Compare::Container` is now able to compare non-copyable
    containers such as Containers::ArrayReference (see :gh:`mosra/corrade#9`).
-   Using :cpp:`const char` instead of :cpp:`const unsigned char` for raw
    binary data.
-   Better algorithm for comparing floating-point values in :dox:`TestSuite`.

Build system
------------

-   CMake now always installs ``FindCorrade.cmake`` to library-specific
    location, making it usable without providing own copy of the file in
    depending projects. The ``WITH_FIND_MODULE`` option is no longer needed.
-   Displaying all header files, plugin metadata files and resource files in
    project view to make use of some IDEs less painful (such as QtCreator).
-   Gentoo ebuild (see :gh:`mosra/corrade#16`).

Bug fixes
---------

-   Removed static initializers to avoid memory corruption in static builds
    (see :gh:`mosra/magnum#90`).
-   Plugin manager now correctly folows dependency order when unloading
    plugins.

Deprecated APIs
---------------

No API was deprecated in this release.

Removed APIs and features
-------------------------

-   Removed unused plugin replacement feature, as it had questionable benefits
    and no real use.
-   All functionality deprecated in `January 2014 snapshot <{filename}january-2014-updates.rst>`_
    has been removed, namely:

    -   Removed deprecated ability to use relative includes (e.g.
        :cpp:`#include <Utility/Debug.h>`), use absolute paths (
        :cpp:`#include <Corrade/Utility/Debug.h>`) instead.
    -   Removed deprecated ``Utility::String::split()`` overload, use either
        :dox:`split() <Utility::String::split()>` or
        :dox:`splitWithoutEmptyParts() <Utility::String::splitWithoutEmptyParts()>`
        instead.

`Magnum`_
=========

New features
------------

-   Proper extension loading for OpenGL ES 2.0, 3.0 and 3.1 using flextGL.
-   Enabled already implemented functionality on OpenGL ES 3.1.
-   Support for new OpenGL functionality:

    -   Support for :glext:`ARB_direct_state_acccess` (OpenGL 4.5)
        everywhere except in Mesh.
    -   Support for :glext:`ARB_conditional_render_inverted` (OpenGL
        4.5) in :dox:`SampleQuery`.
    -   Support for :glext:`ARB_get_texture_sub_image` (OpenGL 4.5) in
        :dox:`*Texture::subImage() <Texture::subImage()>`.
    -   Support for :glext:`EXT_transform_feedback`, :glext:`ARB_transform_feedback2`
        (OpenGL 3.0, 4.0, OpenGL ES 3.0) in new :dox:`TransformFeedback` class,
        added :dox:`Renderer::Feature::RasterizerDiscard`.
    -   Support for :glext:`ARB_robustness_isolation` in
        :dox:`Renderer::graphicsResetStatus()`.
    -   Support for :glext:`ARB_framebuffer_sRGB` (OpenGL 3.0) in
        :dox:`Renderer::Feature::FramebufferSRGB`.
    -   Support for :glext:`OES_texture_npot` ES extension.
    -   Support for debug groups from :glext:`KHR_debug` (OpenGL 4.5) and
        :glext:`EXT_debug_marker` extensions in :dox:`DebugGroup` class.

-   Added :dox:`CubeMapTexture::*image() <CubeMapTexture::image()>` that
    returns all six faces together.
-   Added :dox:`Primitives::Cube::solidStrip()`.
-   Added :dox:`AbstractShaderProgram::attachShaders()` as a list-based
    complement to :dox:`Shader::compile()` and
    :dox:`AbstractShaderProgram::link()`.
-   Separated :cpp:`Renderer::setFeature()` into shorter
    :dox:`Renderer::enable()` and :dox:`Renderer::disable()`.
-   Added :cpp:`tau()`, :cpp:`piHalf()`, :cpp:`e()`, :cpp:`nan()` and
    :cpp:`inf()` to :dox:`Math::Constants`.
-   Added :dox:`Math::Matrix[34]::shearing*() <Math::Matrix4::shearingXY()>`.
-   Added :dox:`Math::Matrix4::lookAt()` (see :gh:`mosra/magnum#88`).
-   Added :dox:`Math::Vector::pad()`.
-   Added :dox:`Math::div()`.
-   Using range-based-for to traverse object children and features in
    :dox:`SceneGraph` and screens in :dox:`Platform::ScreenedApplication`.
-   Convenience overload for :dox:`*Framebuffer::read() <Framebuffer::read()>`
    and :dox:`*Texture::image() <Texture::image()>` that returns the queried
    image by value instead of as parameter.
-   Added :dox:`Buffer::uniformOffsetAlignment()` and
    :dox:`Mesh::maxElementIndex()` limit queries.
-   Added :dox:`AbstractTexture::unbind()` for unbinding range of texture
    units.
-   Added :dox:`SceneGraph::AbstractObject::addFeature()` and
    :dox:`SceneGraph::Object::addChild()` functions (see :gh:`mosra/magnum#86`).
-   VSync support in :dox:`Platform::Sdl2Application`.
-   Added :dox:`Platform::*Context <Platform::GlxContext>` libraries for users
    which want to use custom windowing toolkits instead of the ones provided.
-   Improved documentation about :dox:`Shaders` namespace, added sample image
    for each.

.. image:: {filename}/img/shaders.png
    :alt: Shader samples

Changes
-------

-   Using :gh:`flextGL <ginkgo/flextgl>` instead of ``glLoadGen`` for OpenGL
    extension loading.
-   :dox:`Platform::Sdl2Application` now tries to create core context on all
    platforms, not just OSX (see :gh:`mosra/magnum#80`).
-   :dox:`Buffer::bind()`, :dox:`Buffer::unbind()` and :dox:`Buffer::Target` is
    now used for indexed buffer binding only.
-   Moved static binary operations from :dox:`Math` classes into free functions
    for more convenient usage (see :gh:`mosra/magnum#74`).
-   Better algorithm for comparing floating-point values.
-   Ensuring that all OpenGL objects are properly created before using them.
-   Using :cpp:`const char` instead of :cpp:`const unsigned char` for raw
    binary data.
-   Removed annoying restriction when adding reference-counted resources to
    :cpp:`ResourceManager`.
-   Extension querying cleanup:

    -   Removed :glext:`APPLE_flush_buffer_range` extension.
    -   Replaced :glext:`APPLE_vertex_array_object` with
        :glext:`ARB_vertex_array_object`.
    -   Replaced :glext:`NV_half_float` with :glext:`ARB_half_float_vertex`.
    -   Replaced :glext:`EXT_framebuffer_sRGB` with :glext:`ARB_framebuffer_sRGB`.

-   Documentation cleanup, fixes and improvements.

Build system
------------

-   CMake now always installs ``FindMagnum.cmake`` to library-specific
    location, making it usable without providing own copy of the file in
    depending projects. The ``WITH_FIND_MODULE`` option is no longer needed.
-   CMake now handles inter-library dependencies automatically without manually
    specifying each and every one (see :gh:`mosra/magnum#73`).
-   Ability to control static build of plugins separately using
    ``BUILD_PLUGINS_STATIC``.
-   Displaying all header files, plugin metadata files and resource files in
    project view to make use of some IDEs less painful (such as QtCreator).
-   Gentoo ebuild (see :gh:`mosra/magnum#69`).

Bug fixes
---------

-   Fixed :dox:`TextureTools::distanceField()` to work in GLSL < 4.20 (see
    :gh:`mosra/magnum#62`).
-   Fixed :dox:`Shaders::MeshVisualizer` to work in GLSL ES.
-   Fixed :dox:`Shaders::*Vector <Shaders::Vector>` on Intel GPUs.
-   Fixed assertion on contexts without default framebuffer (see
    :gh:`mosra/magnum#93`).
-   Fixed cases where shader would use extension that is not advertised by the
    driver.
-   Properly handle case where
    :glfnext:`VertexArrayVertexAttribDivisor <EXT_direct_state_access>` is
    not available in the driver (see :gh:`mosra/magnum#77`).
-   Removed superfluous level argument from :dox:`Framebuffer::attachTexture*() <Framebuffer::attachTexture2D()>`
    overloads for texture types that don't support mipmapping.
-   Proper no-op fallback for :dox:`*Framebuffer::invalidate() <Framebuffer::invalidate()>`
    on platforms where the extensions is not supported.
-   Fixed garbage characters in shader compilation output (see
    :gh:`mosra/magnum#87`).
-   Fixed memory corruption on MSVC 2013 (see :gh:`mosra/magnum#70`).
-   Fixed potential memory corruption errors with static build (see
    :gh:`mosra/magnum#79`).
-   Fixed aspect-ratio-corrected projection with inverted Y in
    :dox:`SceneGraph::Camera*D <SceneGraph::Camera3D>`.
-   Fixed wheel direction in :dox:`Platform::Sdl2Application` (see
    :gh:`mosra/magnum#69`).
-   Fixed matrix to quaternion conversion algorithm.

Deprecated APIs
---------------

-   The ``Magnum/Query.h`` header is deprecated, use one of
    ``Magnum/PrimitiveQuery.h``, ``Magnum/SampleQuery.h`` or
    ``Magnum/TimerQuery.h`` instead.
-   Using :dox:`Buffer::Target` as constructor or :dox:`Buffer::setTargetHint()`
    parameter is deprecated, use :dox:`Buffer::TargetHint` instead.
-   The :cpp:`SceneGraph::TransformationType` enum is depracted, use separate
    ``<transform>()`` and ``<transform>Local()`` variants instead.
-   :cpp:`SceneGraph::AbstractObject::hasFeatures()`, :cpp:`firstFeature()`,
    :cpp:`lastFeature()`, :cpp:`SceneGraph::Object::hasChildren()`,
    :cpp:`firstChild()` and :cpp:`lastChild()` are deprecated, use methods on
    linked list returned by :dox:`SceneGraph::AbstractObject::features()` and
    :dox:`SceneGraph::Object::children()` instead.
-   :cpp:`Platform::ScreenedApplication::frontScreen()` and
    :cpp:`Platform::ScreenedApplication::backScreen()` are deprecated, use
    methods on linked list returned by
    :dox:`Platform::ScreenedApplication::screens()` instead.
-   :dox:`*Framebuffer::read() <Framebuffer::read()>` functions taking two
    vectors are deprecated, use overload taking :dox:`Range2Di` instead.
-   The :cpp:`Mesh::maxVertexAttributes()` function is deprecated, use
    :dox:`AbstractShaderProgram::maxVertexAttributes()` instead.
-   :cpp:`Math::Vector::dot()`, :cpp:`Math::Complex::dot()`,
    :cpp:`Math::Quaternion::dot()`, :cpp:`Math::Vector::angle()`,
    :cpp:`Math::Complex::angle()`, :cpp:`Math::Quaternion::angle()`,
    :cpp:`Math::Quaternion::lerp()`, :cpp:`Math::Quaternion::slerp()`,
    :cpp:`Math::Vector2::cross()` and :cpp:`Math::Vector3::cross()` are
    deprecated, use :dox:`Math::dot()`, :dox:`Math::angle()`,
    :dox:`Math::lerp()`, :dox:`Math::slerp()` and :dox:`Math::cross()` instead.
-   The :cpp:`*Framebuffer::bind(FramebufferTarget)` function is deprecated,
    use parameter-less :dox:`*Framebuffer::bind() <Framebuffer::bind()>`
    instead.
-   The :cpp:`FramebufferTarget::ReadDraw` enum value is deprecated, use
    separate :cpp:`FramebufferTarget::Read` and :cpp:`FramebufferTarget::Draw`
    values instead.
-   :dox:`CubeMapTexture::imageSize()` with explicit face parameter is
    deprecated, use overload that returns one value for all faces instead.
-   The ``Magnum/DebugMessage.h`` header is deprecated, use
    :dox:`Magnum/DebugOutput.h` instead.
-   The :cpp:`DebugMessage::Severity` enum is deprecated, use
    :dox:`DebugOutput::Severity` instead.
-   The :cpp:`DebugMessage::Callback` typedef, :cpp:`DebugMessage::setCallback()`
    and :cpp:`DebugMessage::setDefaultCallback()` function is deprecated, use
    :dox:`DebugOutput::Callback`, :dox:`DebugOutput::setCallback()` and
    :dox:`DebugOutput::setDefaultCallback()` instead.
-   The :cpp:`DebugMessage::maxLoggedMessages()` and
    :cpp:`DebugMessage::maxMessageLength()` functions are deprecated, use
    :dox:`DebugOutput::maxLoggedMessages()` and
    :dox:`DebugOutput::maxMessageLength()` instead.
-   The :cpp:`DebugMessage::setEnabled()` function and related values from
    :cpp:`DebugMessage::Source` enum are deprecated, use
    :dox:`DebugOutput::setEnabled()` along with :dox:`DebugOutput::Source`,
    :dox:`DebugOutput::Type` and :dox:`DebugOutput::Severity` instead.
-   Parameter-less :cpp:`*Query` constructor and parametrized
    :cpp:`*Query::begin()` function are deprecated, use constructor with
    parameter and parameter-less :dox:`*Query::begin() <Query::begin()>`
    instead.

Removed APIs and functionality
------------------------------

-   All functionality deprecated in `January 2014 snapshot`_ has been removed,
    namely:

    -   Removed deprecated ability to use relative includes (e.g.
        :cpp:`#include <Mesh.h>`), use absolute paths (:cpp:`#include <Magnum/Mesh.h>`)
        instead.
    -   Removed deprecated :cpp:`Mesh::Primitive` enum, use :dox:`MeshPrimitive`
        instead.
    -   Removed deprecated :cpp:`Sampler::maxAnisotropy()` function, use
        :dox:`Sampler::maxMaxAnisotropy()` instead.
    -   Removed deprecated :cpp:`Math::Geometry::Rectangle` class, use
        :dox:`Math::Range` instead.
    -   Removed deprecated :cpp:`SceneGraph::Animable::group()` function, use
        :dox:`SceneGraph::Animable::animables()` instead.
    -   Removed deprecated ``Shaders/magnumShadersResourceImport.hpp`` file,
        use :dox:`Shaders/resourceImport.hpp` instead.
    -   Removed deprecated :cpp:`Text::TextRenderer` alias, use
        :dox:`Text::Renderer` instead.
    -   Removed long-deprecated :dox:`BufferImage::setData()` overload, use the
        other one instead.

Performance improvements
------------------------

-   Faster vector multiplication in :dox:`Math::Quaternion`.
-   Saved one swizzle in :dox:`Math::cross()`.
-   Allocation-free overloads of functions that took :cpp:`std::string` just to
    pass its contents to OpenGL.

`Magnum Plugins`_
=================

New features
------------

-   New :dox:`OpenDDL parser <Trade::OpenDdl::Document>` and
    :dox:`OpenGexImporter <Trade::OpenGexImporter>` plugin for importing
    `OpenGEX`_ files.
-   New :dox:`StanfordImporter <Trade::StanfordImporter>` plugin for importing
    Stanford (``*.ply``) files.
-   New :dox:`StbImageImporter <Trade::StbImageImporter>` plugin for importing
    BMP, GIF, HDR, JPEG, PIC, PNG, PSD, TGA and more formats using
    :gh:`stb_image <nothings/stb>`.
-   New :dox:`StbPngImageConverter <Trade::StbPngImageConverter>` plugin for
    exporting PNG files using :gh:`stb_image_write <nothings/stb>`.
-   New :dox:`AnyImageImporter <Trade::AnyImageImporter>` plugin that detects
    image file type and delegates the loading to plugin dedicated for given
    format (see :gh:`mosra/magnum#61`).
-   New :dox:`AnySceneImporter <Trade::AnySceneImporter>` plugin that detects
    scene file type and delegates the loading to plugin dedicated for given
    format.
-   Plugin aliases, as explained above.

Removed APIs and functionality
------------------------------

-   All functionality deprecated in `January 2014 snapshot`_ has been removed,
    namely removed deprecated ability to use relative includes (e.g.
    :cpp:`#include <PngImporter/PngImporter.h>`), use absolute paths
    (:cpp:`#include <MagnumPlugins/PngImporter/PngImporter.h>`) instead.

Build system
------------

-   CMake now always installs ``FindMagnumPlugins.cmake`` to library-specific
    location, making it usable without providing own copy of the file in
    depending projects. The ``WITH_FIND_MODULE`` option is no longer needed.
-   Displaying all header files, plugin metadata files and resource files in
    project view to make use of some IDEs less painful (such as QtCreator).
-   Gentoo ebuild (see :gh:`mosra/magnum-plugins#5`).

Magnum Integration
------------------

Build system
------------

-   CMake now always installs ``FindMagnumIntegration.cmake`` to
    library-specific location, making it usable without providing own copy of
    the file in depending projects. The ``WITH_FIND_MODULE`` option is no
    longer needed.
-   Displaying all header files, plugin metadata files and resource files in
    project view to make use of some IDEs less painful (such as QtCreator).
-   Gentoo ebuild (see :gh:`mosra/magnum-integration#2`).

Removed APIs and functionality
------------------------------

All functionality deprecated in `January 2014 snapshot`_ has been removed,
namely removed deprecated ability to use relative includes (e.g.
:cpp:`#include <BulletIntegration/Integration.h>`), use absolute paths
(:cpp:`#include <Magnum/BulletIntegration/Integration.h>`) instead.

Deprecated APIs
---------------

No API was deprecated in this release.

Removed APIs
------------

No deprecated API was removed in this release.

`Magnum Examples`_
==================

Changes
-------

-   Reworked and simplified the Viewer example, updated it to work with
    virtually any file format.

Bug fixes
---------

-   Removed unneeded requirement for :glext:`ARB_invalidate_subdata` from
    ``cubemap`` example (see :gh:`mosra/magnum#63`).

Build system
------------

-   All examples are now installable in a way similar to Magnum itself.
-   Displaying all header files, plugin metadata files and resource files in
    project view to make use of some IDEs less painful (such as QtCreator).
-   Added ArchLinux, Debian and Gentoo packages (see :gh:`mosra/magnum-examples#8`).

Magnum Bootstrap
----------------

No user-visible changes in this release, except for updates of CMake modules.
