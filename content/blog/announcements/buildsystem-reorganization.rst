Buildsystem reorganization for smaller minimal builds
#####################################################

:date: 2018-04-05
:category: Announcements
:tags: CMake, OpenGEX, Emscripten, iOS, Android, Vcpkg, Vulkan
:summary: Magnum master branch received a few breaking changes to the
    buildsystem to further slim down the library in preparation for Vulkan
    support.

.. note-warning::

    Unlike changes in C++ APIs that are often made fully backwards compatible
    with some deprecation warnings, the following changes are isolated to the
    buildsystem. Due to their nature it's not really possible to provide
    backwards compatibility, which means you need to adapt your projects.

`Asset import functionality is now fully optional`_
===================================================

The :dox:`Trade` namespace was historically a part of the core Magnum library.
Based on recent feedback and new use cases popping up, it now made sense to
separate it into its own library. This allows for even smaller builds for
projects that don't need any asset management functionality. That also means
the :dox:`Corrade::PluginManager` library is no longer an implicit dependency
--- at the minimum you now need to link just to the :dox:`Corrade::Utility` and
core :dox:`Magnum` library.

If you are using CMake, be sure to update your local copy of
``FindMagnum.cmake`` and explicitly link to the ``Magnum::Trade`` target if you
are using any functionality from the :dox:`Trade` namespace. Chances are that
you are already linking to another library which depends on ``Magnum::Trade``
so it gets linked transitively, but it's better to be explicit:

.. code:: cmake

    find_package(Magnum REQUIRED Trade)

    # ...

    target_link_libraries(your-app PRIVATE Magnum::Trade)

If you're using Vcpkg with Visual Studio, the automagic will take care of this
behind the scenes. If you're using a custom buildsystem, you may need to add
``MagnumTrade`` to your linker command line. If you are deploying your app with
a shared Magnum build, don't forget to bundle the new shared ``MagnumTrade``
library as well, if your project uses it.

`AnyImporter plugins are moved to the core repository`_
=======================================================

In order to allow core functionality to have more flexible access to image
loading and other plugin capabilities, the
:dox:`AnyImageImporter <Trade::AnyImageImporter>`,
:dox:`AnyImageConverter <Trade::AnyImageConverter>`,
:dox:`AnyAudioImporter <Audio::AnyImporter>` and
:dox:`AnySceneImporter <Trade::AnySceneImporter>` plugins were moved to the
core repository.

If you are not using a package, this means that now you need to pass the
``WITH_ANY*`` CMake options to the core library build instead of the plugins
build in order to have the plugins enabled. Like before, these are *not*
enabled by default. See the :dox:`documentation <building-features>` for more
information.

If you are loading the plugins as dynamic, that's all you need to do. Otherwise
(for example when building your app for Emscripten, Android or iOS), you need
to update your local copies of ``FindMagnum.cmake`` and ``FindMagnumPlugins.cmake``,
ask the core repository for these plugins and link to e.g. ``Magnum::AnyImageImporter``
instead of ``MagnumPlugins::AnyImageImporter``.

.. code:: cmake

    find_package(Magnum REQUIRED AnyImageImporter)

    # ...

    target_link_libraries(your-app PRIVATE Magnum::AnyImageImporter)

If you are using a custom buildsystem, nothing significantly changes, as the
libraries stay practically the same, only are coming from a different project
now.

`Consistent flags for building static plugins`_
===============================================

Both the core Magnum repository and the Magnum Plugins repository now use the
``BUILD_PLUGINS_STATIC`` CMake option to build plugins as static. If you have
been explicitly compiling the plugins as static before you need to update your
build setup from the previous ``BUILD_STATIC`` option. If you are building for
Android, Emscripten or iOS, the static build is enabled implicitly and you
don't need to change anything. See the :dox:`documentation <building-plugins-features>`
for more information.

`Standalone OpenDDL parser library`_
====================================

The :dox:`OpenGexImporter <Trade::OpenGexImporter>` plugin is built around an
:dox:`OpenDDL parser <OpenDdl>` library. Since the beginning it was a private
part of the plugin and using it directly (for example to access OpenGEX
extension structures) meant one had to compile the plugin as static or go
through other extra steps to be able to call its APIs. This is now simpler, as
the library is completely standalone in the Magnum Plugin repository. It's
built implicitly whenever the OpenGEX importer plugin is built, but can be also
built explicitly by enabling ``WITH_OPENDDL``. The ``BUILD_STATIC`` flag was
repurposed to control shared/static build of this library. Again, see the
:dox:`documentation <building-plugins-features>` for more information.

Using it from CMake means simply updating your copy of ``FindMagnumPlugins.cmake``
and linking to it:

.. code:: cmake

    find_package(MagnumPlugins REQUIRED OpenDdl)

    # ...

    target_link_libraries(your-app PRIVATE MagnumPlugins::OpenDdl)

For consistency with the rest of the project, the includes were moved to a more
intuitive location --- instead of e.g.

.. code:: c++

    #include <MagnumPlugins/OpenGexImporter/OpenDdl/Document.h>

it's now just

.. code:: c++

    #include <Magnum/OpenDdl/Document.h>

The old header location is still available, but will issue deprecation
warnings.

If you don't need to use the library directly, you don't need to update
anything. Deploying an application that's using the
:dox:`OpenGexImporter <Trade::OpenGexImporter>` plugin now needs to supply the
``MagnumOpenDdl`` library as well, in case it's built as shared.

`Packages and related projects`_
================================

ArchLinux AUR Git, Homebrew, Debian and Gentoo packages are already updated to
reflect the above changes, simply update them
:dox:`as described in the documentation <building-packages>`. The Vcpkg package
update is currently :gh:`pending review <microsoft/vcpkg#3191>` and is expected
to land soon.

Example code that's directly or indirectly using the :dox:`Trade` library is
updated to reflect the change as well.

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/982002137428254720>`_
