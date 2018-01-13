Triangle Example (asm.js)
#########################

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
      <script async="async" src="{filename}/showcase/triangle-asmjs/magnum-triangle.js"></script>
      <script src="{filename}/showcase/EmscriptenApplication.js"></script>
    </div></div></div></div>

.. block-warning:: Doesn't work?

    This example requires a browser with WebGL 1 enabled. Unlike the
    `WebAssembly version <{filename}/showcase/triangle.rst>`__, this example
    uses only `asm.js <http://asmjs.org/>`_, so it should work on more
    browsers. If you see a black rectangle instead of a live example, the
    browser console might show some details about the error. See the
    `Showcase <{filename}/showcase.rst>`_ page for more information; you
    can also report a bug either for the :gh:`example itself <mosra/magnum-examples>`
    or :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find a thorough tutorial together with source code of this example
    :dox:`in the documentation <examples-triangle>`.
