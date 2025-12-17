2D Fluid Simulation Example
###########################

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

A 2D fluid simulation using the APIC (`Affine Particle-in-Cell <https://dl.acm.org/citation.cfm?id=2766996>`_)
method. Compared to `3D Fluid Simulation <{filename}fluidsimulation3d.rst>`_,
the simulation is running in a single thread.

.. topic:: Controls

    -   :label-default:`mouse drag` interacts with the simulation
    -   :label-default:`E` emits more particles
    -   :label-default:`H` shows / hides the overlay
    -   :label-default:`R` resets the simulation
    -   :label-default:`Space` pauses the simulation

.. raw:: html

    <div class="mn-container">
      <div class="mn-sizer"><div class="mn-expander"><div class="mn-listener">
        <canvas class="mn-canvas" id="canvas" tabindex="0"></canvas>
        <div class="mn-status" id="status">Initialization...</div>
        <div class="mn-status-description" id="status-description"></div>
        <script src="{static}/showcase/EmscriptenApplication.js"></script>
        <script async="async" src="{static}/showcase/fluidsimulation2d/magnum-fluidsimulation2d.js"></script>
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
    :dox:`in the documentation <examples-fluidsimulation2d>`.
