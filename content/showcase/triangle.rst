Triangle Example
################

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

So-called *Hello World* of 3D graphics. Displays simple triangle with different
colors for each vertex, which are interpolated over the triangle area using
minimal possible amount of code.

.. raw:: html

    <div class="mn-container">
      <div class="mn-sizer"><div class="mn-expander"><div class="mn-listener">
        <canvas class="mn-canvas" id="canvas" tabindex="0"></canvas>
        <div class="mn-status" id="status">Initialization...</div>
        <div class="mn-status-description" id="status-description"></div>
        <script async="async" src="{static}/showcase/triangle/magnum-triangle.js"></script>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 1 enabled, but if you don't have a WebAssembly-capable
    browser, you can try to run the classic
    `asm.js version <{filename}/showcase/triangle-asmjs.rst>`_ instead. See the
    `Showcase <{filename}/showcase.rst>`_ page for more information; you can
    also report a bug either for the :gh:`example itself <mosra/magnum-examples>`
    or :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find a thorough tutorial together with source code of this example
    :dox:`in the documentation <examples-triangle>`.
