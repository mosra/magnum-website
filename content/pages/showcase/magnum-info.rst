Magnum WebGL 1 Info
###################

:save_as: showcase/magnum-info/index.html
:url: showcase/magnum-info/
:css: {filename}/showcase/showcase.css

Displays various information about Magnum and the WebGL implementation it's
running on. As with the command-line :dox:`magnum-info` utility, it's possible
to specify additional options. Here they are, conveniently linked for you:

-   `Default view <?>`_
-   `Show limits <?limits>`_
-   `Show all extension strings <?extension-strings>`_

A `WebGL 2 version <{filename}/pages/showcase/magnum-info-webgl2.rst>`_ is also
available.

.. raw:: html

    <div id="wrapper3"><div id="wrapper2"><div id="wrapper"><div id="listener">
      <canvas id="module" class="hidden"></canvas>
      <pre id="log"></pre>
      <div id="status">Initialization...</div>
      <div id="status-description"></div>
      <script src="{filename}/showcase/WindowlessEmscriptenApplication.js"></script>
      <script async="async" src="magnum-info.js"></script>
      <!-- pelican:attach src="{attach}/showcase/magnum-info/magnum-info.js" -->
      <!-- pelican:attach src="{attach}/showcase/magnum-info/magnum-info.wasm" -->
    </div></div></div></div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <http://webassembly.org/>`_-capable
    browser with WebGL 1 enabled. If you see a blank space instead of the
    output, the browser console might show some details about the error. See
    the `Showcase <{filename}/pages/showcase.rst>`_ page for more information;
    you can also report a bug either for the :gh:`utility itself <mosra/magnum>`
    or :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Documentation

    All options for the Magnum Info utility are explained
    :dox:`in the documentation <magnum-info>`.
