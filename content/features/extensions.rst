Plugins & Extensions
####################

:breadcrumb:
    {filename}/features.rst Features
:cover: {filename}/img/cover-features-extensions.jpg
:summary: Plugins for asset import and conversion offered by the Magnum engine
:footer:
    .. note-dim::
        :class: m-text-center

        `« Extra functionality <{filename}/features/extras.rst>`_ | `Features <{filename}/features.rst>`_

.. raw:: html
    :file: extensions.svg

A lot of `Magnum addon libraries <{filename}/features/extras.rst>`_ define
interfaces for further extensibility with plugins. Advantage of using plugins
is the ability to defer decision about particular dependency into deploy stage
on given platform.

`Importers`_
============

Plugins importing general scene formats with mesh data, image data, scene
hierarchy, material, light and camera properties.

-   TGA, PNG, JPEG, GIF, EXR, HDR, BMP, DDS and other image formats
-   OBJ, Stanford PLY, COLLADA and `OpenGEX <http://opengex.org/>`_ scene
    format parsers
-   Plugin using `Assimp <http://assimp.org/>`_ for general file format support
-   "Any importer" plugin delegating to other image / scene importer plugins
    based on detected file format

.. note-dim::

    See the :dox:`Trade::AbstractImporter` class and ``*Importer`` classes in
    the :dox:`Trade` namespace for details.

`Image converters`_
===================

Plugins for converting image data between various formats.

-   Exporting raw image data as TGA, PNG, HDR, BMP or EXR
-   Interface for compressing raw image data into GPU-readable formats
-   "Any image converter" plugin delegating to other converter plugins based on
    output file extension

.. note-dim::

    See the :dox:`Trade::AbstractImageConverter` class and ``*ImageConverter``
    classes in the :dox:`Trade` namespace for details.

`Audio importers`_
==================

Plugins importing audio data.

-   OGG Vorbis, WAV and FLAC support
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

`Math converters`_
==================

Lastly, it's possible to implement routines for converting external data types
from and to Magnum builtin vector, matrix, complex, quaternion and other types.
Once implemented, conversion from external to internal types is a matter of an
explicit conversion.

.. note-dim::

    This functionality is implemented by the :dox:`BulletIntegration` and
    :dox:`OvrIntegration` libraries.
