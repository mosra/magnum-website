Primitives example
##################

:css: {filename}/showcase/showcase.css
:breadcrumb: {filename}/showcase.rst Showcase

Shows how to use builtin primitives and do simple input handling. Displays
colored cube which can be rotated using mouse.

.. topic:: Key controls

    -   **Mouse drag**: Rotate the cube
    -   **Mouse click**: Change cube color

.. raw:: html

    <div id="wrapper3"><div id="wrapper2"><div id="wrapper"><div id="listener">
      <canvas id="module"></canvas>
      <div id="status">Initialization...</div>
      <div id="status-description"></div>
      <script async="async" src="{filename}/showcase/primitives/magnum-primitives.js"></script>
      <script src="{filename}/showcase/EmscriptenApplication.js"></script>
    </div></div></div></div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <http://webassembly.org/>`_-capable
    browser with WebGL 1 enabled. If you see a black rectangle instead of a
    live example, the browser console might show some details about the error.
    See the `Showcase <{filename}/showcase.rst>`_ page for more information;
    you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find :gh:`source code of this example <mosra/magnum-examples$master/src/primitives>`
    on GitHub. This example is also thoroughly explained
    :dox:`in the documentation <examples-primitives>`.
