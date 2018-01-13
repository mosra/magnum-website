Artwork HOWTO
#############

architecture.svg
================

Exporting:

#.  Enable layer-by-layer and "Save a copy" via Inkscape as Optimized SVG to
    ``/content/features.svg``, ``/content/features/extensions.svg`` and
    ``/content/features/extras.svg``. Enable all possible options in the
    dialog.
#.  Hand-edit each exported SVG and:

    -   Convert absolute URLs so ``{filename}`` is first
    -   Remove all top-level ``<g display="none">`` elements
    -   Replace ``width``/``height`` on the top-level ``<svg>`` with::

            style="max-width: 750px; max-height: 370px;"

        (assuming the original width/height is this)

#.  Check git diff for suspicious changes

favicon.svg
===========

Export each layer (+ the bg layer) as 16x16 PNG with the following naming and
convert to an ``*.ico``:

.. code:: sh

    convert favicon.{png,ico}
    convert favicon-blog.{png,ico}
    convert favicon-doc.{png,ico}
    convert favicon-static.{png,ico}
