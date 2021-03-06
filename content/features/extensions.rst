Plugins & Extensions
####################

:breadcrumb:
    {filename}/features.rst Features
:cover: {static}/img/cover-features-extensions.jpg
:summary: Plugins for asset import and conversion offered by the Magnum engine
:footer:
    .. note-dim::
        :class: m-text-center

        `« Extra Functionality <{filename}/features/extras.rst>`_ | `Features <{filename}/features.rst>`_ | `Community Contributions » <{filename}/features/community.rst>`_

.. container:: m-container-inflate

    .. raw:: html
        :file: extensions.svg

A lot of `Magnum extra libraries <{filename}/features/extras.rst>`_ define
interfaces for further extensibility with plugins. Advantage of using plugins
is the ability to defer decision about particular dependency into deploy stage
on given platform.

`Importers`_
============

Plugins importing general scene formats with mesh data, image data, scene
hierarchy, material, light and camera properties.

-   TGA, PNG, JPEG, GIF, EXR, HDR, BMP, DDS and other image formats
-   `glTF <https://www.khronos.org/gltf/>`_, OBJ, Stanford PLY and
    `OpenGEX <https://opengex.org/>`_ scene format parsers
-   A plugin using `Assimp <http://assimp.org/>`_ for COLLADA, FBX and other
    general file format support
-   "Any importer" plugin delegating to other image / scene importer plugins
    based on detected file format

.. note-dim::

    See the :dox:`Trade::AbstractImporter` class and ``*Importer`` classes in
    the :dox:`Trade` namespace for details.

`Image converters`_
===================

Plugins for converting image data between various formats.

-   Exporting raw image data as JPEG, TGA, PNG, HDR, BMP or EXR
-   Interface for compressing raw image data into GPU-readable formats
-   "Any image converter" plugin delegating to other converter plugins based on
    output file extension

.. note-dim::

    See the :dox:`Trade::AbstractImageConverter` class and ``*ImageConverter``
    classes in the :dox:`Trade` namespace for details.

`Mesh converters`_
==================

*Coming soon.* Subscribe to :gh:`mosra/magnum#240` for updates.

`Audio importers`_
==================

Plugins importing audio data.

-   OGG Vorbis, MP3, AAC, WAV and FLAC support
-   "Any importer" plugin delegating to other audio importer plugins based on
    detected file format

.. note-dim::

    See the :dox:`Audio::AbstractImporter` class and ``*Importer`` classes in
    the :dox:`Audio` namespace for details.

`Fonts`_
========

Plugins providing parsing of font files and text shaping/layouting.

-   TTF and OTF support
-   Custom internal Magnum raster font format
-   `FreeType <https://www.freetype.org/>`_ and `HarfBuzz <https://harfbuzz.org>`_
    integration

.. note-dim::

    See the :dox:`Text::AbstractFont` class and ``*Font`` classes in the
    :dox:`Text` namespace for details.

`Font converters`_
==================

Plugins providing conversion between font formats and pre-rendering glyph cache
textures.

-   Converting subset of a TTF/OTF font to custom internal Magnum raster format

.. note-dim::

    See the :dox:`Text::AbstractFontConverter` class and ``*FontConverter``
    classes in the :dox:`Text` namespace for details.

`Shader converters`_
====================

*Coming soon.* Subscribe to :gh:`mosra/magnum#234` for updates.

`Math converters`_
==================

Lastly, it's possible to implement routines for converting external data types
from and to Magnum builtin vector, matrix, complex, quaternion and other types.
Once implemented, conversion from external to internal types is a matter of an
explicit conversion.

.. note-dim::

    There's a builtin integration with Bullet, Eigen, GLM, Vulkan and other
    third-party APIs. See :dox:`the full list <types-thirdparty-integration>`
    for a detailed overview.
