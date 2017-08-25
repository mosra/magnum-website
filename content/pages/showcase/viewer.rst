Viewer example
##############

:save_as: showcase/viewer/index.html
:url: /showcase/viewer/
:css: {filename}/showcase/showcase.css

This is simply an viewer for 3D scene files, such as OpenGEX or COLLADA ones.
Only triangle meshes with Phong shading with optional diffuse texture are
currently supported.

.. topic:: Controls

    -   **Mouse drag** rotates the model
    -   **Mouse wheel** zooms in or out

.. raw:: html

    <div id="wrapper3"><div id="wrapper2"><div id="wrapper"><div id="listener">
      <canvas id="module"></canvas>
      <div id="status">Initialization...</div>
      <div id="status-description"></div>
      <script src="{filename}/showcase/EmscriptenApplication.js"></script>
      <script async="async" src="magnum-viewer.js"></script>
    </div></div></div></div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <http://webassembly.org/>`_-capable
    browser with WebGL 1 enabled. If you see a black rectangle instead of a
    live example, the browser console might show some details about the error.
    See the `Showcase <{filename}/pages/showcase.rst>`_ page for more
    information; you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find :gh:`source code of this example <mosra/magnum-examples$master/src/viewer>`
    on GitHub. This example is also thoroughly explained
    :dox:`in the documentation <examples-viewer>`.
