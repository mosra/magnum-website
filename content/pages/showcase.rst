Showcase
########

Demos and examples on this page require `WebAssembly <http://webassembly.org/>`_-enabled
browser with at least WebGL 1 working. All recent versions of major browsers
should work, see the `introductory blog post <{filename}/blog/announcements/webassembly-support-in-magnum.rst>`__
for more information. You can also use the `Magnum Info <{filename}/pages/showcase/magnum-info.rst>`_
utility to see if your browser is capable of running the demos; if you are
unlucky and your browser isn't, you can at least try `asm.js <http://asmjs.org/>`_
version the `Triangle example <{filename}/pages/showcase/triangle-asmjs.rst>`_.

.. block-info:: Spotted a bug?

    It's impossible to verify that all examples work on all browsers. If you
    have a problem running the demos or the website is behaving weirdly, please
    report a bug either for the :gh:`engine <mosra/magnum>`,
    :gh:`a particular example <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. role:: info(strong)
    :class: m-text m-info

.. role:: warning(strong)
    :class: m-text m-warning

.. .. container:: m-row m-container-inflate

    .. container:: m-col-m-6 m-push-m-3

        .. figure:: {filename}/showcase/push-the-box/screenshot.png
            :figclass: m-figure m-fullwidth
            :target: {filename}/pages/showcase/push-the-box.rst

            Push the Box

            Simple 3D reincarnation of Sokoban. Your target is to push all the
            boxes to their destination places. Made as an entry into
            `GitHub Game Off 2012 <https://github.com/blog/1303-github-game-off>`_.
            :info:`Requires WebGL 2.`

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. figure:: {filename}/showcase/audio/screenshot.png
            :figclass: m-figure m-fullwidth
            :target: {filename}/pages/showcase/audio.rst

            Audio example

            Shows how to play spatialized audio with Magnum.
            :warning:`Warning: plays sound on load.`

    .. container:: m-col-m-6

        .. figure:: {filename}/showcase/viewer/screenshot.png
            :figclass: m-figure m-fullwidth
            :target: {filename}/pages/showcase/viewer.rst

            Viewer example

            Uses OpenGEX importer plugin to load 3D scene for interactive
            viewing.

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. figure:: {filename}/showcase/primitives/screenshot.png
            :figclass: m-figure m-fullwidth
            :target: {filename}/pages/showcase/primitives.rst

            Primitives example

            Shows how to use builtin primitives and how to do simple input
            handling. Displays colored cube which can be rotated using mouse.

    .. container:: m-col-m-6

        .. figure:: {filename}/showcase/text/screenshot.png
            :figclass: m-figure m-fullwidth
            :target: {filename}/pages/showcase/text.rst

            Text example

            Showcase of Magnum text rendering using signed distance field
            technique. Full UTF-8 support. Buffer mapping is used for mutable
            text.

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. figure:: {filename}/showcase/triangle/screenshot.png
            :figclass: m-figure m-fullwidth
            :target: {filename}/pages/showcase/triangle.rst

            Triangle example

            Displays simple triangle with interpolated colors. For the unlucky,
            an `asm.js version <{filename}/pages/showcase/triangle-asmjs.rst>`__
            that doesn't require WebAssembly-capable browser is also available.

    .. container:: m-col-m-6

        .. figure:: {filename}/showcase/textured-triangle/screenshot.png
            :figclass: m-figure m-fullwidth
            :target: {filename}/pages/showcase/textured-triangle.rst

            Textured triangle example

            Slightly extended version of the triangle example; uses TGA
            importer plugin to load a stone image and displays a triangle with
            colored texture on it.

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. figure:: {filename}/showcase/magnum-info/screenshot.png
            :figclass: m-figure m-fullwidth
            :target: {filename}/pages/showcase/magnum-info.rst

            Magnum Info

            Text utility printing out various information about Magnum and
            the OpenGL / WebGL implementation it's running on. Versions for
            `WebGL 1 <{filename}/pages/showcase/magnum-info.rst>`__ and
            `WebGL 2 <{filename}/pages/showcase/magnum-info-webgl2.rst>`__ are
            available.

    .. container:: m-col-m-6

        .. figure:: {filename}/showcase/magnum-al-info/screenshot.png
            :figclass: m-figure m-fullwidth
            :target: {filename}/pages/showcase/magnum-al-info.rst

            Magnum AL Info

            Text utility printing out various information about Magnum and
            the OpenAL implementation it's running on.
