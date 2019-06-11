Artwork HOWTO
#############

architecture.svg
================

Exporting:

#.  Enable layer-by-layer and "Save a copy" via Inkscape as Optimized SVG to
    ``/content/features.svg``, ``/content/features/extras.svg``,
    ``/content/features/extensions.svg`` and ``/content/features/community.svg``.
    Enable all possible options in the dialog. On fresh installations you
    need ``python2-lxml`` and ``python2-scour`` packages.
#.  Hand-edit each exported SVG and:

    -   Remove all top-level ``<g display="none">`` elements (use editor code
        collapsing feature), there should be only four top-level ``<g>``
        elements left. Scour unfortunately doesn't implement this yet:
        https://github.com/scour-project/scour/issues/73
    -   Convert absolute URLs so ``{filename}`` is first (three times each)
    -   Replace ``width``/``height`` on the top-level ``<svg>`` with::

            style="max-width: 875px; max-height: 415px;"

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
