Bullet Physics Example
######################

:css: {static}/showcase/showcase.css
:highlight: showcase
:breadcrumb: {filename}/showcase.rst Showcase

Shows a rotating table full of cubes that you can shoot down, showcasing the
:dox:`BulletIntegration` library together with :dox:`SceneGraph` using
:dox:`BulletIntegration::MotionState`, visualizing various properties of the
Bullet physics world using :dox:`BulletIntegration::DebugDraw`. Everything is
rendered in at most three draw calls using instanced :dox:`Shaders::Phong`.

.. topic:: Key controls

    -   :label-default:`Arrow keys` rotate the camera around
    -   :label-default:`mouse click` shoots an object
    -   :label-default:`S` toggles between a box (larger, lighter) or a sphere
        (smaller but heavier) to shoot
    -   :label-default:`D` toggles draw mode (solid + wireframe debug overlay,
        just solid or just wireframe debug)

.. raw:: html

    <div id="container">
      <div id="sizer"><div id="expander"><div id="listener">
        <canvas id="canvas" tabindex="0"></canvas>
        <div id="status">Initialization...</div>
        <div id="status-description"></div>
        <script async="async" src="{static}/showcase/bullet/magnum-bullet.js"></script>
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
    :dox:`in the documentation <examples-bullet>`.
