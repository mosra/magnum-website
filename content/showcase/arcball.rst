ArcBall Camera Example
######################

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

Implementation of `Ken Shoemake's arcball camera <https://www.talisman.org/~erlkonig/misc/shoemake92-arcball.pdf>`_
with a smooth navigation feature. Using arcball, the camera control is more
accurate and consistent. See also the `Mouse Interaction <{filename}mouseinteraction.rst>`_
example for an alternative approach.

.. topic:: Controls

    -   :label-default:`mouse drag` rotates the view
    -   :label-warning:`Shift` :label-default:`mouse drag` pans the view
    -   :label-default:`mouse wheel` zooms in/out
    -   :label-default:`R` resets the simulation
    -   :label-default:`L` toggles lagging (smooth camera navigation)

.. raw:: html

    <div class="mn-container">
      <div class="mn-sizer"><div class="mn-expander"><div class="mn-listener">
        <canvas class="mn-canvas" id="canvas" tabindex="0"></canvas>
        <div class="mn-status" id="status">Initialization...</div>
        <div class="mn-status-description" id="status-description"></div>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
        <script async="async" src="{static}/showcase/arcball/magnum-arcball.js"></script>
      </div></div></div>
    </div>

.. block-warning:: Doesn't work?

    This example requires `WebAssembly <https://webassembly.org/>`_-capable
    browser with WebGL 2 enabled. See the `Showcase <{filename}/showcase.rst>`_
    page for more information; you can also report a bug either for the
    :gh:`example itself <mosra/magnum-examples>` or
    :gh:`for the website <mosra/magnum-website>`. Feedback welcome!

.. block-info:: Source code and documentation

    You can find further information and source code of this example
    :dox:`in the documentation <examples-arcball>`.
