Magnum WebGL 2 Info
###################

:css: {filename}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

Displays various information about Magnum and the WebGL implementation it's
running on. As with the command-line :dox:`magnum-info` utility, it's possible
to specify additional options. Here they are, conveniently linked for you:

-   `Default view <?>`_
-   `Show limits <?limits>`_
-   `Show all extension strings <?extension-strings>`_

A `WebGL 1 version <{filename}/showcase/magnum-info.rst>`_ is also available.

.. raw:: html

    <div id="wrapper3"><div id="wrapper2"><div id="wrapper"><div id="listener">
      <canvas id="module" class="hidden"></canvas>
      <pre id="log"></pre>
      <div id="status">Initialization...</div>
      <div id="status-description"></div>
      <script async="async" src="{filename}/showcase/magnum-info-webgl2/magnum-info.js"></script>
      <script src="{filename}/showcase/WindowlessEmscriptenApplication.js"></script>
    </div></div></div></div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <http://webassembly.org/>`_-capable
    browser with WebGL 2 enabled. If you see a blank space instead of the
    output, the browser console might show some details about the error. See
    the `Showcase <{filename}/showcase.rst>`_ page for more information; you
    can also report a bug either for the :gh:`utility itself <mosra/magnum>`
    or :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Documentation

    All options for the Magnum Info utility are explained
    :dox:`in the documentation <magnum-info>`.
