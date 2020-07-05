Ray Tracing Example
###################

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

Implementation of a simple CPU ray tracer adapted from Peter Shirley's
book `Ray Tracing in One Weekend <https://raytracing.github.io/books/RayTracingInOneWeekend.html>`_.
The current implementation runs on single thread and performs iterative
rendering to refine the result. Typically, a high quality image can be achieved
after around 100 iterations.

.. topic:: Controls

    -   :label-default:`mouse drag` rotates the camera
    -   :label-warning:`Shift` :label-default:`mouse drag` pans the camera
    -   :label-default:`mouse wheel` zooms in/out
    -   :label-default:`R` resets the camera to its original transformation
    -   :label-default:`D` toggles Depth-of-Field
    -   :label-default:`M` toggles marking the next rendered block by a
        different color
    -   :label-default:`N` generates a new random scene
    -   :label-default:`Space` pauses/resumes rendering

    Additionally, various options can be set via URL GET parameters:

    -   ``block-size=PIXELS`` --- size of a block to render at a time (default:
        64)
    -   ``max-samples=COUNT`` --- max samples per pixel (default: 100)
    -   ``max-ray-depth=DEPTH`` ---  max ray depth (default: 16)

.. raw:: html

    <div id="container">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="canvas" tabindex="0"></canvas>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{static}/showcase/raytracing/magnum-raytracing.js"></script>
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

    You can find further information and source code of this example
    :dox:`in the documentation <examples-arcball>`.
