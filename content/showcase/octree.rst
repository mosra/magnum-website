Octree Example
##############

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

A simple, single-threaded implementation of a `loose octree <https://anteru.net/blog/2008/loose-octrees/>`_
which is commonplace in computer graphics. In this example, octree is used for
collision detection.

.. topic:: Controls

    -   :label-default:`mouse drag` rotates the camera
    -   :label-warning:`Shift` :label-default:`mouse drag` pans the camera
    -   :label-default:`mouse wheel` zooms in/out
    -   :label-default:`R` resets the camera to its original transformation
    -   :label-default:`O` switches between a brute force and octree
        collision detection
    -   :label-default:`B` shows/hides tree node bounding boxes
    -   :label-default:`P` toggles frame profiling to the console using
        :dox:`DebugTools::FrameProfiler`
    -   :label-default:`Space` pauses/resumes particle simulation

    Additionally, various options can be set via GET parameters:

    -   ``spheres=N`` --- number of spheres to simulate (default: 2000)
    -   ``sphere-radius=R`` --- sphere radius (default: 0.0333)
    -   ``sphere-velocity=V`` ---  sphere velocity (default: 0.05)

    With the default setting, the octree collision detection is about twice as
    fast than the brute force method. In order to better see the octree
    visualization, append the following parameters to the URL
    `or click this link <?spheres=20&sphere-radius=0.1&sphere-velocity=1.0>`_::

        ?spheres=20&sphere-radius=0.1&sphere-velocity=1.0

.. raw:: html

    <div id="container">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="canvas" tabindex="0"></canvas>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{static}/showcase/octree/magnum-octree.js"></script>
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
    :dox:`in the documentation <examples-octree>`.
