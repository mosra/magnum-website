Viewer Example
##############

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

A viewer for 3D scene files, such as glTF or OpenGEX ones, deliberately
simplified for tutorial purposes. For a full-featured model viewer and
animation player see the `Magnum Player <{filename}/showcase/player.rst>`_ app.

.. topic:: Controls

    -   :label-default:`Mouse drag` --- rotate the model
    -   :label-default:`Mouse wheel` --- zooms in or out

.. raw:: html

    <div id="container">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="module" tabindex="0"></canvas>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{static}/showcase/viewer/magnum-viewer.js"></script>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
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
    :dox:`in the documentation <examples-viewer>`.
