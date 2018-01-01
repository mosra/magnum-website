Triangle Example
################

:css: {filename}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

So-called *Hello World* of 3D graphics. Displays simple triangle with different
colors for each vertex, which are interpolated over the triangle area using
minimal possible amount of code.

.. raw:: html

    <div id="wrapper3"><div id="wrapper2"><div id="wrapper"><div id="listener">
      <canvas id="module"></canvas>
      <div id="status">Initialization...</div>
      <div id="status-description"></div>
      <script async="async" src="{filename}/showcase/triangle/magnum-triangle.js"></script>
      <script src="{filename}/showcase/EmscriptenApplication.js"></script>
    </div></div></div></div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <http://webassembly.org/>`_-capable
    browser with WebGL 1 enabled, but if you don't have a WebAssembly-capable
    browser, you can try to run the classic
    `asm.js version <{filename}/showcase/triangle-asmjs.rst>`_ instead. If you
    see a black rectangle instead of a live example, the browser console might
    show some details about the error. See the `Showcase <{filename}/showcase.rst>`_
    page for more information; you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find :gh:`source code of this example <mosra/magnum-examples$master/src/triangle>`
    on GitHub. This example is also thoroughly explained
    :dox:`in the documentation <examples-triangle>`.
