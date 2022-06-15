Artwork HOWTO
#############

quadtree.svg
============

Exporting:

#.  Enable layer-by-layer and "Save a copy" via Inkscape as Optimized SVG to
    ``/content/features.svg``, ``/content/features/extras.svg``,
    ``/content/features/extensions.svg`` and ``/content/features/community.svg``.
    Enable all possible options in the dialog. On fresh installations you
    need ``python2-lxml`` and ``python2-scour`` packages.
#.  Hand-edit each exported SVG and replace ``width``/``height`` on the
    top-level ``<svg>`` with::

        style="max-width: 384px; max-height: 384px;"

    (assuming the original width/height is this)

#.  Check git diff for suspicious changes

input.svg, packed.svg
=====================

Generated using the ``main.cpp`` file. See invocation at the top of the file.
