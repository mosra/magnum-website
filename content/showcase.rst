Showcase
########

:cover: {filename}/img/cover-showcase.jpg
:summary: Interactive WebGL showcase of Magnum capabilities

Demos and examples on this page require `WebAssembly <https://webassembly.org/>`_-enabled
browser with at least WebGL 1 working. All recent versions of major browsers
should work, see the `introductory blog post <{filename}/blog/announcements/webassembly-support-and-more.rst>`__
for more information. You can also use the
`Magnum GL Info <{filename}/showcase/gl-info.rst>`_ utility to see if your
browser is capable of running the demos; if you are unlucky and your browser
isn't capable, you can at least try the
`asm.js version of the Triangle example <{filename}/showcase/triangle-asmjs.rst>`_.

.. block-info:: Spotted a bug?

    It's impossible to verify that all examples work on all browsers. If you
    have a problem running the demos or the website is behaving weirdly, please
    report a bug either for the :gh:`engine <mosra/magnum>`,
    :gh:`a particular example <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. .. container:: m-row m-container-inflate

    .. container:: m-col-m-6 m-push-m-3

        .. figure:: {filename}/showcase/push-the-box/screenshot.png
            :figclass: m-figure m-fullwidth
            :target: {filename}/showcase/push-the-box.rst

            Push the Box :label-danger:`webgl 2`

            Simple 3D reincarnation of Sokoban. Your target is to push all the
            boxes to their destination places. Made as an entry into
            `GitHub Game Off 2012 <https://github.com/blog/1303-github-game-off>`_.

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [box2d]
            :end-before: [/box2d]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [bullet]
            :end-before: [/bullet]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [picking]
            :end-before: [/picking]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [arealights]
            :end-before: [/arealights]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [ui-gallery]
            :end-before: [/ui-gallery]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [webvr]
            :end-before: [/webvr]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [audio]
            :end-before: [/audio]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [viewer]
            :end-before: [/viewer]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [primitives]
            :end-before: [/primitives]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [text]
            :end-before: [/text]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6 m-push-m-3

        .. include:: showcase-figures.rst.in
            :start-after: [mouseinteraction]
            :end-before: [/mouseinteraction]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [triangle]
            :end-before: [/triangle]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [textured-triangle]
            :end-before: [/textured-triangle]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [gl-info]
            :end-before: [/gl-info]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [al-info]
            :end-before: [/al-info]

*Looking for more?* Not all demos are ported to HTML5 yet. See
:dox:`Examples and Tutorials <example-index>` in the documentation --- there's
an Oculus VR demo, shadow mapping, shader effects and more.
