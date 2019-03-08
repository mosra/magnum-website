Plugin usability improvements
#############################

:date: 2018-03-27
:category: Announcements
:cover: {static}/img/blog/announcements/plugin-usability-improvements.jpg
:tags: C++, CMake, Corrade, glTF, Assimp, Vulkan, Vcpkg
:summary: I made a brief detour on the way to Vulkan support and equipped
    Magnum with features that make plugin workflow nicer and open up new
    possibilities.

.. role:: cpp(code)
    :language: c++
.. role:: cmake(code)
    :language: cmake

`Autodetected plugin paths`_
============================

Until now, in order to load a dynamic plugin, one *had to* specify a path where
to look for them. That was as flexible as it could get, yes, but in most cases
the plugins would be in one or two common places and so the plugin usage
usually involved just propagating a well-known path from CMake via :cmake:`configure_file()`.

*Well, not anymore!*

Every plugin interface now has a few plugin search paths predefined, which will
"just work" in most cases. For example, in case of :dox:`*Importer <Trade::AbstractImporter>`
plugins, the plugins are first searched in a ``magnum/importers/`` subdirectory
next to the executable (which is a common case when deploying on Windows) and
as a fallback in ``/usr/lib/magnum/importers`` (or any other system-wide
location depending on ``CMAKE_INSTALL_PREFIX``). See documentation of various
:dox:`pluginSearchPaths() <Trade::AbstractImporter::pluginSearchPaths()>`
functions for details.

With this change in place, loading a model from a glTF file is now just a
matter of this:

.. code:: c++

    PluginManager::Manager<Trade::AbstractImporter> manager;
    std::unique_ptr<Trade::AbstractImporter> importer =
        manager.loadAndInstantiate("GltfImporter");

    importer->openFile("cave.gltf");
    Containers::Optional<Trade::MeshData3D> data =
        importer->mesh3D(importer->mesh3DForName("treasure-chest"));

The flexibility was not taken away, though --- it's still possible to pass the
plugin directory to the constructor the same as before, if you need to.

`Plugin alias priorities`_
==========================

There's usually more than one plugin available to achieve a particular goal
--- for example in order to open a PNG file, you can choose among
:dox:`PngImporter <Trade::PngImporter>`, :dox:`DevIlImageImporter <Trade::DevIlImageImporter>`
or :dox:`StbImageImporter <Trade::StbImageImporter>`. Rather than this being an
unnecessary redundancy, it allows you to pick a particular performance /
portability / feature-set tradeoff --- a plugin with no external dependencies
for a web build or, on the other hand, the fastest possible implementation for
a tool that does heavy image processing.

To make this simpler in the code and defer the decision to the buildsystem or
app deployment, all plugins that support a particular format provide a common
*alias* --- in case of PNG images, you can just load a :cpp:`"PngImporter"`
plugin and if :dox:`PngImporter <Trade::PngImporter>` is not available, it will
pick up any of the other candidates.

For greater control you can now use :dox:`setPreferredPlugins() <PluginManager::AbstractManager::setPreferredPlugins()>`,
giving a prioritized candidate list for a particular alias. This is especially
useful with font plugins, where you can get advanced layout capabilities if the
:dox:`HarfBuzzFont <Text::HarfBuzzFont>` plugin is available or at least a
faster and smoother glyph rendering if you can get the :dox:`FreeTypeFont <Text::FreeTypeFont>`
plugin. If none of the suggestions is available, it falls back to whatever is
left (which can be, for example, the :dox:`StbTrueTypeFont <Text::StbTrueTypeFont>`
plugin).

.. code:: c++

    PluginManager::Manager<Text::AbstractFont> manager;
    manager.setPreferredPlugins("TrueTypeFont", {"HarfBuzzFont", "FreeTypeFont"});

This of course works also in combination with the
:dox:`AnyImporter <Trade::AnyImageImporter>` plugins --- the following snippet
will use DevIL instead of the builtin :dox:`DdsImporter <Trade::DdsImporter>`
to decode a DXT-compressed texture into plain RGBA on load:

.. code:: c++

    PluginManager::Manager<Trade::AbstractImporter> manager;
    manager.setPreferredPlugins("DdsImporter", {"DevIlImageImporter"});
    std::unique_ptr<Trade::AbstractImporter> importer =
        manager.loadAndInstantiate("AnyImageImporter");
    importer->openFile("texture.dds");

`Plugin-specific configuration`_
================================

Because it's not possible for a general statically typed plugin API to expose
all possible knobs and switches that a file format could support, the plugins
have a possibility to supply additional configuration via the
:dox:`configuration() <Corrade::PluginManager::AbstractPlugin::configuration()>`
function. For example, in the :dox:`AssimpImporter <Trade-AssimpImporter-configuration>`
plugin you can toggle various postprocessing steps that are applied to loaded
scene files:

.. code:: c++

    std::unique_ptr<Trade::AbstractImporter> importer =
        manager.loadAndInstantiate("AssimpImporter");
    importer->configuration().group("postprocess")->setValue("PreTransformVertices", true);

This is just the first spring flower, expect more functionality being exposed
through plugin-specific configuration in the future --- ability to control
output quality of image converters, controlling advanced text layouting
functionality of font plugins, ...

`Automagic import of static plugins`_
=====================================

There are platforms on which using :cpp:`dlopen()` and equivalents is either
straight impossible or too annoying. For such cases there are static plugins.
If you are using CMake, all you need to do is list the required plugins in a
:cmake:`find_package()` call (which was *not* needed for dynamic plugins) and
then :cmake:`target_link_libraries()` them to your final executable:

.. code:: cmake

    find_package(MagnumPlugins REQUIRED
        TinyGltfImporter
        StbTrueTypeFont)

    add_executable(my-app my-app.cpp)
    target_link_libraries(my-app PRIVATE
        MagnumPlugins::TinyGltfImporter
        MagnumPlugins::StbTrueTypeFont)

That's all, you don't need to change a single bit of your C++ code, a CMake
:cmake:`target_sources()` command does all the magic behind. If you're not
using CMake but for example Visual Studio with `Vcpkg <https://github.com/Microsoft/vcpkg>`_,
you need to explicitly :cpp:`#include` the "static plugin import" file instead:

.. code:: c++

    /* No need to do this if you use CMake */
    #include <MagnumPlugins/TinyGltfImporter/importStaticPlugin.cpp>
    #include <MagnumPlugins/StbTrueTypeFont/importStaticPlugin.cpp>

`Now, where do I get this?`_
============================

If you are already happily using Magnum, just grab latest ``master`` revisions
and don't forget to update your local copies of ``FindCorrade.cmake``,
``FindMagnum.cmake`` and ``FindMagnumPlugins.cmake`` modules to make the new
features work.

If you are not using Magnum yet, welcome! We probably already have a package
for your platform to get you started in no time. Head over to the
:dox:`Getting Started <getting-started>` guide and *have fun*!

`There's more!`_
================

Thanks to an exceptional first-time contribution from :gh:`NussknackerXXL` and
help from :gh:`Squareys` there's now a fresh :dox:`TinyGltfImporter <Trade::TinyGltfImporter>`
plugin for importing both text and binary glTF 2.0 files. Besides that --- and
besides the :gh:`ongoing Vulkan work <mosra/magnum#234>` --- a lot of effort
went into the documentation, see for example the new extensive
:dox:`Android development guide <platforms-android>`.

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/978676980349587457>`_,
    `Reddit r/cpp <https://www.reddit.com/r/cpp/comments/87k2os/plugin_workflow_improvements_in_the_magnum/>`_,
    `Reddit r/gamedev <https://www.reddit.com/r/gamedev/comments/87k3bv/plugin_workflow_improvements_in_the_magnum_c11/>`_
