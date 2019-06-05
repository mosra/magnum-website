Triangle Example (asm.js)
#########################

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

So-called *Hello World* of 3D graphics. Displays simple triangle with different
colors for each vertex, which are interpolated over the triangle area using
minimal possible amount of code.

.. raw:: html

    <div id="container">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="canvas" tabindex="0"></canvas>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{static}/showcase/triangle-asmjs/magnum-triangle.js"></script>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires a browser with WebGL 1 enabled. Unlike the
    `WebAssembly version <{filename}/showcase/triangle.rst>`__, this example
    uses only `asm.js <http://asmjs.org/>`_, so it should work on more
    browsers. See the `Showcase <{filename}/showcase.rst>`_ page for more
    information; you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find a thorough tutorial together with source code of this example
    :dox:`in the documentation <examples-triangle>`.
