WebAssembly support in Magnum --- and more
##########################################

:date: 2017-08-25
:category: Announcements
:cover: {filename}/img/blog/webassembly-support-and-more.jpg
:tags: C++, CMake, NaCl, Emscripten, WebAssembly, WebGL
:summary: Along with dropped support for NaCl, Magnum now has first-class
    WebAssembly support. I also took this opportunity to overhaul the outdated
    `Showcase <{filename}/showcase.rst>`_ page with WebAssembly builds and
    there is a bunch more Emscripten-related goodies all over the place!

.. role:: cmake(code)
    :language: cmake
.. role:: info(strong)
    :class: m-text m-info
.. role:: warning(strong)
    :class: m-text m-warning

As `(P)NaCl was deemed dead <https://blog.chromium.org/2017/05/goodbye-pnacl-hello-webassembly.html>`_
by its creators, I axed all support for it from all repos. Frankly, it was
quite a weird platform, something in-between OpenGL ES 2.0 and WebGL 1.0,
having very special ugly C++ API and sharing a lot of C++11 workarounds with
Android (sigh). In the Magnum repository alone,
:gh:`the amount of deleted code <mosra/magnum@2145ae9b969f058e39bcaa008c7e5f6bab57189e>`
was almost 5k lines.

On the other hand, adding support for `WebAssembly <http://webassembly.org/>`_
was almost too easy. With Emscripten 1.37.9 and newer it just boils down to
adding one new compiler and linker flag, different files are generated (but
they are still loaded automatically from the main ``*.js`` file), and
everything else stays the same as with the classic `asm.js <http://asmjs.org/>`_
compilation.

But how?
========

If you pull latest revisions of Magnum repositories, building for WebAssembly
instead of asm.js is just a matter of specifying different toolchain ---
``Emscripten-wasm.cmake`` instead of ``Emscripten.cmake``. Note that I had to
use some newer CMake functionality to make the difference, so you'll need at
least CMake 3.7 to compile for Emscripten now. I hope that's not a problem for
you --- in the worst case you can always download a prebuilt binary archive of
CMake, extract and run it from whatever location. More info about building for
WebAssembly is :dox:`in the docs <building-cross-emscripten>`. If you are on
ArchLinux like me, there are also new ``PKGBUILD-emscripten-wasm`` and
``PKGBUILD-emscripten-wasm-webgl2`` package scripts in ``package/archlinux``
directory of each repository to make your life easier.

In order to properly deploy your WebAssembly app, you'll need to change your
:cmake:`install()` step. With the :cmake:`OPTIONAL` keyword you can support
both the asm.js case (separate ``*.js.mem`` file) and WebAssembly case
(separate ``*.wasm`` file). Also, there is a new :dox:`MAGNUM_DEPLOY_PREFIX <cmake>`
variable that can be used to differentiate install location for the final app
from install location for system files (so, for example, you can set
``MAGNUM_DEPLOY_PREFIX`` to point to a location used by your webserver while
``CMAKE_INSTALL_PREFIX`` still points to system location where libraries are
stored):

.. code:: cmake

    if(CORRADE_TARGET_EMSCRIPTEN)
        install(FILES
            MyApplication.html EmscriptenApplication.js WebApplication.css
            DESTINATION ${MAGNUM_DEPLOY_PREFIX})
        install(TARGETS MyApplication DESTINATION ${MAGNUM_DEPLOY_PREFIX})
        install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/MyApplication.js.mem
            ${CMAKE_CURRENT_BINARY_DIR}/MyApplication.wasm
            DESTINATION ${MAGNUM_DEPLOY_PREFIX} OPTIONAL)
    endif()

Note that in order to make full use of all the new features, you might want to
update your copies of ``EmscriptenApplication.js`` and ``WebApplication.css``
from the repository.

Show me what you got
====================

The most exciting about WebAssembly and web things in general is that you
don't have to get up from your computer, go to the app store across the street
and tell them to download and install a new app on your machine. WebAssembly
demos are just one click away --- head over to the fully reworked
`Showcase <{filename}/showcase.rst>`_ page! Apart from all examples that were
already ported to Emscripten and now they are just recompiled in WebAssembly,
there is also a fresh port of the Audio example:

.. container:: m-row

    .. container:: m-col-m-6 m-push-m-3

        .. figure:: {filename}/showcase/audio/screenshot.png
            :alt: Audio example screenshot
            :figclass: m-figure m-fullwidth
            :target: {filename}/showcase/audio.rst

            Audio example

            Shows how to play spatialized audio with Magnum.
            :warning:`Warning: plays sound on load.`

I aimed to make all the demo pages responsive and mobile-firendly, but nobody's
perfect --- if you see something strange, don't hesitate to report a bug either
on the :gh:`website repository <mosra/magnum-website>` or for a
:gh:`particular example <mosra/magnum-examples>`. Feedback welcome, as always.

.. note-success::

    Previously, the showcase page had also a small game that was running on
    NaCl. I am in the middle of porting it to Emscripten, but couldn't make it
    in time for this article. Stay tuned, it will reappear --- and with it also
    other new demos!

Browser support
===============

Even though WebAssembly is not *that* new, it may happen that the above
examples didn't work on your machine. Here's a list of browsers that support
it:

.. class:: m-table m-center-t

=================== =============================================
Browser             Support state
=================== =============================================
Internet Explorer   Seriously? No. Sorry.
Edge                Since version 16 (Windows 10 Creators Update)
Firefox             Since version 52
Opera               Since version 45
Vivaldi             Since version 1.9
Chrome              Since version 58
Android Chrome      Since version 56
Safari              Since version 11 (macOS 10.13)
iOS Safari          Since version 11 (iOS 11)
=================== =============================================

Windowless applications
=======================

In order to make proper browser testing possible in the future, "windowless"
applications are now supported in Emscripten as well. In practice it means that
instead of showing a canvas, the web app is showing its textual output. This
also makes the Magnum Info utilities finally available on the web:

.. container:: m-row

    .. container:: m-col-m-6

        .. figure:: {filename}/showcase/magnum-info/screenshot.png
            :alt: Magnum Info screenshot
            :figclass: m-figure m-fullwidth
            :target: {filename}/showcase/magnum-info.rst

            Magnum Info

            Text utility printing out various information about Magnum and
            the OpenGL / WebGL implementation it's running on. Versions for
            `WebGL 1 <{filename}/showcase/magnum-info.rst>`__ and
            `WebGL 2 <{filename}/showcase/magnum-info-webgl2.rst>`__ are
            available.

    .. container:: m-col-m-6

        .. figure:: {filename}/showcase/magnum-al-info/screenshot.png
            :alt: Magnum AL Info screenshot
            :figclass: m-figure m-fullwidth
            :target: {filename}/showcase/magnum-al-info.rst

            Magnum AL Info

            Text utility printing out various information about Magnum and
            the OpenAL implementation it's running on.

Windowless applications for Emscripten are using the :dox:`Platform::WindowlessEglApplication`
class, see its documentation for detailed usage guide. There is also a new
:gh:`windowless-emscripten <mosra/magnum-bootstrap$windowless-emscripten>`
bootstrap project.

"Command-line" arguments
========================

Along with windowless applications being supported, it's now possible to pass
"command-line" arguments to apps running in the browser. This was possible
since ever when running command-line apps through Node.js, but now this can be
done with the browser apps as well. Just pass the arguments as URL GET
parameters. Only long named arguments and boolean options are supported. For
example, having the URL as

::

    /my-app/?enable-msaa&magnum-disable-extensions=GL_OES_vertex_array_object GL_EXT_texture_filter_anisotropic

is equivalent to calling the command-line version of the application as

.. code:: sh

    ./my-app --enable-msaa --magnum-disable-extensions "GL_OES_vertex_array_object GL_EXT_texture_filter_anisotropic"

The program name is prepended to the argument list (to become ``argv[0]``)
automatically by Emscripten and is hardcoded to ``./this.program``. All
``--magnum-*`` options :dox:`supported by the engine <Context>` are working on
Emscripten-compiled applications as well.
:dox:`More info in the docs. <Platform::WindowlessEglApplication>`

Size comparison to asm.js
=========================

The difference is very minor --- but that's largely due to the fact that all
binary data in the (optimized) asm.js version were in a separate binary file
(instead of being represented in text) and a very aggressive closure compiler
step was applied to the generated JS file to minify it. Compressing the data
makes the difference even smaller --- there's simply the same amount of
information, just encoded differently. Here's a table showing size of the
generated ``*.js`` and ``*.js.mem`` / ``*.wasm`` files for the
`Triangle example <{filename}/showcase/triangle.rst>`_:

.. class:: m-table m-center-t

=========================== ========
Triangle example build      Size
=========================== ========
asm.js, uncompressed        720.3 kB
wasm, uncompressed          590.1 kB
asm.js, gzipped             179.9 kB
wasm, gzipped               165.9 kB
=========================== ========

.. note-info::

    Note that the sizes are taken at the time of writing this article and
    there's still *a lot* to improve regarding code size in Magnum. Quite a few
    possible optimizations are explained in this excellent blog post:
    `10 simple diet tricks for asm.js <http://floooh.github.io/2016/08/27/asmjs-diet.html>`_
    (It's not relevant to just asm.js, most of the tricks apply to WebAssembly
    as well).

Because compressing the data really makes a difference, enabling it on the
server is crucial for fast download times. There's one problem, though: by
default, the servers are configured to compress only textual data such as
``*.js``, ``*.html`` or ``*.txt`` files, excluding the very-nicely-compressible
``*.wasm`` data. Because WebAssembly is quite new, relying on :gh:`its MIME-type <WebAssembly/design#981>`
might be problematic and so it's best to just rely on file extensions in your
Apache configuration or ``.htaccess`` file:

.. code:: apache

    AddOutputFilter DEFLATE html css js wasm

Another possibility is renaming the ``*.wasm`` files to e.g. ``*.wasm.txt``,
which is a solution when you don't have the possibility to override your server
configuration. But note that then the WebAssembly files won't get loaded
automagically and you need to supply your own async loading code.

.. transition:: ~ ~ ~

Okay, that's all! There's still quite a lot Emscripten-related features, demos
and improvements in my buffer, so expect another blog post later!
