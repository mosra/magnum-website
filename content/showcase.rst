Showcase
########

:cover: {filename}/img/cover-showcase.jpg
:summary: Interactive WebGL showcase of Magnum capabilities

Demos and examples on this page require `WebAssembly <http://webassembly.org/>`_-enabled
browser with at least WebGL 1 working. All recent versions of major browsers
should work, see the `introductory blog post <{filename}/blog/announcements/webassembly-support-and-more.rst>`__
for more information. You can also use the `Magnum Info <{filename}/showcase/magnum-info.rst>`_
utility to see if your browser is capable of running the demos; if you are
unlucky and your browser isn't, you can at least try
`asm.js version of the Triangle example <{filename}/showcase/triangle-asmjs.rst>`_.

.. block-info:: Spotted a bug?

    It's impossible to verify that all examples work on all browsers. If you
    have a problem running the demos or the website is behaving weirdly, please
    report a bug either for the :gh:`engine <mosra/magnum>`,
    :gh:`a particular example <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. role:: label-primary
    :class: m-label m-primary
.. role:: label-danger
    :class: m-label m-danger
.. role:: label-warning
    :class: m-label m-warning
.. role:: label-success
    :class: m-label m-success
.. role:: label-flat-success
    :class: m-label m-flat m-success
.. role:: label-info
    :class: m-label m-info
.. role:: label-flat-info
    :class: m-label m-flat m-info

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
            :start-after: [triangle]
            :end-before: [/triangle]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [textured-triangle]
            :end-before: [/textured-triangle]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [primitives]
            :end-before: [/primitives]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [viewer]
            :end-before: [/viewer]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [text]
            :end-before: [/text]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [audio]
            :end-before: [/audio]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [picking]
            :end-before: [/picking]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [magnum-ui-gallery]
            :end-before: [/magnum-ui-gallery]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [magnum-info]
            :end-before: [/magnum-info]

    .. container:: m-col-m-6

        .. include:: showcase-figures.rst.in
            :start-after: [magnum-al-info]
            :end-before: [/magnum-al-info]

*Looking for more?* Not all demos are ported to HTML5 yet. See
:dox:`Examples and Tutorials <example-index>` in the documentation --- there's
an Oculus VR demo, shadow mapping, Bullet Physics integration and more.
