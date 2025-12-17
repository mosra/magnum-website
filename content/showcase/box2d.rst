Box2D Example
#############

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

Builds a pyramid out of cubes and allows you to expand or destroy it by adding
more. Uses the :dox:`SceneGraph` library directly with Box2D and renders
everything in a single draw call using instanced :dox:`Shaders::Flat`.

.. topic:: Key controls

    -   :label-default:`mouse click` adds a cube to cursor position

.. raw:: html

    <div class="mn-container">
      <div class="mn-sizer"><div class="mn-expander"><div class="mn-listener">
        <canvas class="mn-canvas" id="canvas" tabindex="0"></canvas>
        <div class="mn-status" id="status">Initialization...</div>
        <div class="mn-status-description" id="status-description"></div>
        <script async="async" src="{static}/showcase/box2d/magnum-box2d.js"></script>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 1 and the :webglext:`ANGLE_instanced_arrays` extension
    supported. See the `Showcase <{filename}/showcase.rst>`_ page for more
    information; you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find further information and source code of this example
    :dox:`in the documentation <examples-box2d>`.
