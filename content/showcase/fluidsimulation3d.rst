3D Fluid Simulation Example
###########################

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

A basic implementation of SPH (`Smoothed-Particle Hydrodynamics <https://en.wikipedia.org/wiki/Smoothed-particle_hydrodynamics>`_)
solver. The native version runs multithreaded (and in order to run real-time,
accuracy has been heavily sacrificed for performance), the web version is
single-threaded and thus is significantly slower. See also
`2D Fluid Simulation <{filename}fluidsimulation2d.rst>`_, which runs real-time
just in a single thread.

.. topic:: Controls

    -   :label-default:`mouse drag` rotates the view
    -   :label-warning:`Shift` :label-default:`mouse drag` pans the view
    -   :label-default:`mouse wheel` zooms in/out
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
        <script async="async" src="{static}/showcase/fluidsimulation3d/magnum-fluidsimulation3d.js"></script>
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
    :dox:`in the documentation <examples-fluidsimulation3d>`.
