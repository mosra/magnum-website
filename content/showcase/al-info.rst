Magnum AL Info
##############

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase
:alias: /showcase/magnum-al-info/

Displays various information about Magnum and the OpenAL implementation it's
running on. As with the command-line :dox:`magnum-al-info` utility, it's
possible to specify additional options. Here they are, conveniently linked for
you:

-   `Default view <?>`_
-   `Show all extension strings <?extension-strings>`_

.. raw:: html

    <div id="container">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="canvas" class="hidden"></canvas>
        <pre id="log" class="m-console-wrap"></pre>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{static}/showcase/al-info/magnum-al-info.js"></script>
        <script src="{static}/showcase/WindowlessEmscriptenApplication.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser. See the `Showcase <{filename}/showcase.rst>`_ page for more
    information; you can also report a bug either for the
    :gh:`utility itself <mosra/magnum>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Documentation

    All options for the Magnum AL Info utility are explained
    :dox:`in the documentation <magnum-al-info>`.
