Showcase
########

To run Native Client demos, you need Google Chrome 25 or higher with working
Native Client and OpenGL. Some GPUs are blacklisted, you can try to bypass it
with ``--disable-gpu-blacklist`` option.

To run Emscripten demos, you need Google Chrome, Opera or Mozilla Firefox with
WebGL enabled.

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. image:: {filename}/img/push-the-box.png

        **Push the Box.** Simple 3D reincarnation of Sokoban. Your target is to
        push all the boxes to their destination places. Made as an entry into
        GitHub Game Off 2012.

        .. class:: m-text m-dim

        -   *Platforms:* Linux, Windows, OSX, NaCl
        -   View :gh:`source code <nuclearwhales/push-the-box>`
        -   `Run using Native Client <http://nuclearwhales.github.io/push-the-box>`__

    .. container:: m-col-m-6

        .. image:: {filename}/img/viewer.png

        **Viewer.** Example using OpenGEX importer plugin to load 3D scene for
        interactive viewing.

        .. class:: m-text m-dim

        -   *Platforms:* Linux, Windows, OSX, Emscripten
        -   View :dox:`documentation <examples-viewer>`,
            :gh:`source code <mosra/magnum-examples$master/src/viewer>`
        -   `Run using Emscripten <http://mosra.cz/blog/magnum/viewer-emscripten.html>`__

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. image:: {filename}/img/primitives.png

        **Primitives.** Example showing how to use builtin primitives and do
        simple input handling. Displays colored cube which can be rotated using
        mouse.

        .. class:: m-text m-dim

        -   *Platforms:* Linux, Windows, OSX, NaCl, Emscripten
        -   View :dox:`documentation <examples-primitives>`, :gh:`source code <mosra/magnum-examples$master/src/primitives>`
        -   `Run using Native Client <http://mosra.cz/blog/magnum/primitives-nacl.html>`__
        -   `Run using Emscripten <http://mosra.cz/blog/magnum/primitives-emscripten.html>`__

    .. container:: m-col-m-6

        .. image:: {filename}/img/text.png

        **Text.** Showcase of Magnum text rendering using signed distance field
        technique. Full UTF-8 support. Buffer mapping is used for mutable text.

        .. class:: m-text m-dim

        -   *Platforms:* Linux, Windows, OSX, NaCl, Emscripten
        -   View :gh:`source code <mosra/magnum-examples$master/src/text>`
        -   `Run using Native Client <http://mosra.cz/blog/magnum/text-nacl.html>`__
        -   `Run using Emscripten <http://mosra.cz/blog/magnum/text-emscripten.html>`__

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. image:: {filename}/img/triangle.png

        **Triangle.** So-called Hello World of 3D graphics. Displays simple
        triangle with different colors for each vertex, which are interpolated
        over the triangle area using minimal possible amount of code.

        .. class:: m-text m-dim

        -   *Platforms:* Linux, Windows, OSX, NaCl, Emscripten
        -   View :dox:`documentation <examples-triangle>`,
            :gh:`source code <mosra/magnum-examples$master/src/triangle>`
        -   `Run using Native Client <http://mosra.cz/blog/magnum/triangle-nacl.html>`__
        -   `Run using Emscripten <http://mosra.cz/blog/magnum/triangle-emscripten.html>`__

    .. container:: m-col-m-6

        .. image:: {filename}/img/textured-triangle.png

        **Textured Triangle.** Example using TGA importer plugin to load stone
        image. Displays triangle with colored texture on it.

        .. class:: m-text m-dim

        -   *Platforms:* Linux, Windows, OSX, NaCl, Emscripten
        -   View :dox:`documentation <examples-textured-triangle>`,
            :gh:`source code <mosra/magnum-examples$master/src/textured-triangle>`
        -   `Run using Native Client <http://mosra.cz/blog/magnum/textured-triangle-nacl.html>`__
        -   `Run using Emscripten <http://mosra.cz/blog/magnum/textured-triangle-emscripten.html>`__

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. image:: {filename}/img/info.png

        **Magnum Info.** Text utility printing out various information about
        Magnum and OpenGL capabilities.

        .. class:: m-text m-dim

        -   *Platforms:* Linux, Windows, OSX, NaCl
        -   `Run using Native Client <http://mosra.cz/blog/magnum/magnum-info-nacl.html>`__
