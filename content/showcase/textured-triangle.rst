Textured Triangle Example
#########################

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

This example is just a slightly more complicated version of the
`basic triangle example <{filename}/showcase/triangle.rst>`_. It uses a TGA
importer plugin to load a stone image and displays a triangle with texture
on it.

.. raw:: html

    <div class="mn-container">
      <div class="mn-sizer"><div class="mn-expander"><div class="mn-listener">
        <canvas class="mn-canvas" id="canvas" tabindex="0"></canvas>
        <div class="mn-status" id="status">Initialization...</div>
        <div class="mn-status-description" id="status-description"></div>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
        <script async="async" src="{static}/showcase/textured-triangle/magnum-textured-triangle.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 1 enabled. See the `Showcase <{filename}/showcase.rst>`_
    page for more information; you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find a thorough tutorial together with source code of this example
    :dox:`in the documentation <examples-texturedquad>`.
