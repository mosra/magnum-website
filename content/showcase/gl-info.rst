Magnum WebGL 1 Info
###################

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase
:alias:
    /showcase/magnum-gl-info/
    /showcase/magnum-info/

Displays various information about Magnum and the WebGL implementation it's
running on. As with the command-line :dox:`magnum-gl-info` utility, it's
possible to specify additional options. Here they are, conveniently linked for
you:

-   `Default view <?>`_
-   `Show limits <?limits>`_
-   `Show all extension strings <?extension-strings>`_

An `asm.js WebGL 1 version <{filename}/showcase/gl-info-asmjs.rst>`_ and
`WebGL 2 version <{filename}/showcase/gl-info-webgl2.rst>`_ is also available.

.. raw:: html

    <div class="mn-container">
      <div class="mn-sizer"><div class="mn-expander"><div class="mn-listener">
        <canvas class="mn-canvas mn-hidden" id="canvas"></canvas>
        <pre class="mn-log m-console-wrap" id="log"></pre></pre>
        <div class="mn-status" id="status">Initialization...</div>
        <div class="mn-status-description" id="status-description"></div>
        <script async="async" src="{static}/showcase/gl-info/magnum-gl-info.js"></script>
        <script src="{static}/showcase/WindowlessEmscriptenApplication.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 1 enabled, but if you don't have a WebAssembly-capable
    browser, you can try to run the classic
    `asm.js version <{filename}/showcase/gl-info-asmjs.rst>`_ instead. See the
    `Showcase <{filename}/showcase.rst>`_ page for more information; you can
    also report a bug either for the :gh:`utility itself <mosra/magnum>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Documentation

    All options for the Magnum GL Info utility are explained
    :dox:`in the documentation <magnum-gl-info>`.
