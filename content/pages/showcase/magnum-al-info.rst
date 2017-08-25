Magnum AL Info
##############

:save_as: showcase/magnum-al-info/index.html
:url: /showcase/magnum-al-info/
:css: {filename}/showcase/showcase.css

Displays various information about Magnum and the OpenAL implementation it's
running on. As with the command-line :dox:`magnum-al-info` utility, it's
possible to specify additional options. Here they are, conveniently linked for
you:

-   `Default view <?>`_
-   `Show all extension strings <?extension-strings>`_

.. raw:: html

    <div id="wrapper3"><div id="wrapper2"><div id="wrapper"><div id="listener">
      <canvas id="module" class="hidden"></canvas>
      <pre id="log"></pre>
      <div id="status">Initialization...</div>
      <div id="status-description"></div>
      <script src="{filename}/showcase/WindowlessEmscriptenApplication.js"></script>
      <script async="async" src="magnum-al-info.js"></script>
    </div></div></div></div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <http://webassembly.org/>`_-capable
    browser. If you see a blank space instead of the output, the browser
    console might show some details about the error. See the
    `Showcase <{filename}/pages/showcase.rst>`_ page for more information; you
    can also report a bug either for the :gh:`utility itself <mosra/magnum>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Documentation

    All options for the Magnum AL Info utility are explained
    :dox:`in the documentation <magnum-al-info>`.
